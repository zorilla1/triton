
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

void write_bits(FILE* f, int value, size_t len);
void write_bit(FILE* f, int bit);
void write_literal(FILE* f, unsigned int literal);
void write_string(FILE* f, const char* str);
void write_length(FILE* f, unsigned int length);

static uint8_t buffer = 0;
static int size = 0;

void flush(FILE* f)
{
	while (size != 8)
		write_bit(f, 0);
	write_bit(f, 0);
}

void write_bits(FILE* f, unsigned int value, size_t len)
{
	int i;
	for (i = len - 1; i >= 0; i--)
	{
		write_bit(f, value >> i);
	}
}

void write_bit(FILE* f, unsigned int bit)
{
	if (size < 8)
	{
		size++;
		buffer = (buffer >> 1) | ((bit & 1) << 7);
	}
	else
	{
		fprintf(f, "%c", buffer);
		size = 1;
		buffer = ((bit & 1) << 7);
	}
}

void write_literal(FILE* f, unsigned int literal)
{
	if (literal < 144)
	{
		write_bits(f, 48 + literal, 8);
	}
	else if (literal < 256)
	{
		write_bits(f, 0b110010000 + literal - 144, 9);
	}
	else if (literal < 280)
	{
		write_bits(f, literal - 256, 7);
	}
	else if (literal < 288)
	{
		write_bits(f, 0b11000000 + literal - 280, 8);
	}
}

void write_string(FILE* f, const char* str)
{
	while (*str)
	{
		write_literal(f, *str);
		str++;
	}
}

void write_length(FILE* f, unsigned int length)
{
	assert(length >= 3);
	length -= 3;
	if (length < 8)
	{
		write_literal(f, length + 257);
	}
	else if (length < 16)
	{
		write_literal(f, (length >> 1) + 261);
		write_bits(f, length, 1);
	}
	else if (length < 32)
	{
		write_literal(f, (length >> 2) + 265);
		write_bits(f, length, 2);
	}
	else if (length < 64)
	{
		write_literal(f, (length >> 3) + 269);
		write_bits(f, length, 3);
	}
	else if (length < 128)
	{
		write_literal(f, (length >> 4) + 273);
		write_bits(f, length, 4);
	}
	else if (length < 255)
	{
		write_literal(f, (length >> 5) + 277);
		write_bits(f, length, 5);
	}
	else if (length == 256)
	{
		write_literal(f, 285);
	}
}

void write_distance(FILE* f, unsigned int dist)
{
	assert(dist >= 1);
	assert(dist <= 32768);

	dist--;
	if (dist < 4)
	{
		write_bits(f, dist, 5);
	}
	else if (dist < 8)
	{
		int e = 1;
		int code = ((((dist - (1 << (e + 1))) / (1 << e)) + 2 * e + 2) << e) | (dist & ((1 << e) - 1));
		write_bits(f, code, e + 5);
	}
	else if (dist < 16)
	{
		int e = 2;
		int code = ((dist - (1 << (e + 1))) / (1 << e)) + 2 * e + 2;
		int extra_bits = (dist & ((1 << e) - 1));
		write_bits(f, code, 5);
		write_bits(f, extra_bits, e);
	}
	else if (dist < 32)
	{
		int e = 3;
		int code = ((dist - (1 << (e + 1))) / (1 << e)) + 2 * e + 2;
		int extra_bits = (dist & ((1 << e) - 1));
		write_bits(f, code, 5);
		write_bits(f, extra_bits, e);
	}
	else if (dist < 64)
	{
		int e = 4;
		int code = ((dist - (1 << (e + 1))) / (1 << e)) + 2 * e + 2;
		int extra_bits = (dist & ((1 << e) - 1));
		write_bits(f, code, 5);
		write_bits(f, extra_bits, e);

	}
	else if (dist < 128)
	{
		int e = 5;
		int code = ((dist - (1 << (e + 1))) / (1 << e)) + 2 * e + 2;
		int extra_bits = (dist & ((1 << e) - 1));
		write_bits(f, code, 5);
		write_bits(f, extra_bits, e);
	}
	else if (dist < 256)
	{
		int e = 6;
		int code = ((dist - (1 << (e + 1))) / (1 << e)) + 2 * e + 2;
		int extra_bits = (dist & ((1 << e) - 1));
		write_bits(f, code, 5);
		write_bits(f, extra_bits, e);
	}
	else if (dist < 512)
	{
		int e = 7;
		int code = ((dist - (1 << (e + 1))) / (1 << e)) + 2 * e + 2;
		int extra_bits = (dist & ((1 << e) - 1));
		write_bits(f, code, 5);
		write_bits(f, extra_bits, e);
	}
	else if (dist < 1024)
	{
		int e = 8;
		int code = ((dist - (1 << (e + 1))) / (1 << e)) + 2 * e + 2;
		int extra_bits = (dist & ((1 << e) - 1));
		write_bits(f, code, 5);
		write_bits(f, extra_bits, e);
	}
	else if (dist < 2048)
	{
		int e = 9;
		int code = ((dist - (1 << (e + 1))) / (1 << e)) + 2 * e + 2;
		int extra_bits = (dist & ((1 << e) - 1));
		write_bits(f, code, 5);
		write_bits(f, extra_bits, e);
	}
	else if (dist < 4096)
	{
		int e = 10;
		int code = ((dist - (1 << (e + 1))) / (1 << e)) + 2 * e + 2;
		int extra_bits = (dist & ((1 << e) - 1));
		write_bits(f, code, 5);
		write_bits(f, extra_bits, e);
	}
	else if (dist < 8192)
	{
		int e = 11;
		int code = ((dist - (1 << (e + 1))) / (1 << e)) + 2 * e + 2;
		int extra_bits = (dist & ((1 << e) - 1));
		write_bits(f, code, 5);
		write_bits(f, extra_bits, e);
	}
	else if (dist < 16384)
	{
		int e = 12;
		int code = ((dist - (1 << (e + 1))) / (1 << e)) + 2 * e + 2;
		int extra_bits = (dist & ((1 << e) - 1));
		write_bits(f, code, 5);
		write_bits(f, extra_bits, e);
	}
	else
	{
		int e = 13;
		int code = ((dist - (1 << (e + 1))) / (1 << e)) + 2 * e + 2;
		int extra_bits = (dist & ((1 << e) - 1));
		write_bits(f, code, 5);
		write_bits(f, extra_bits, e);
	}
}