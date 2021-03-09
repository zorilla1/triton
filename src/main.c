#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include <stdint.h>

/*

	Everything I'm using here I got ffrom RFC1950 and RFC1951
	Orby/dSr

*/

int main(int argc, char** argv)
{
	FILE* f = stdout;
	int i;

	// TIC-80 Cart Header
	uint8_t tic_hdr = 0x10;
	fwrite(&tic_hdr, 1, 1, f);
	tic_hdr = 0xFF;
	fwrite(&tic_hdr, 1, 1, f);
	tic_hdr = 0xFF;
	fwrite(&tic_hdr, 1, 1, f);
	tic_hdr = 0x00;
	fwrite(&tic_hdr, 1, 1, f);

	// ZLIB Header
	uint16_t hdr = 0xDA78;
	fwrite(&hdr, 2, 1, f);

	// DEFLATE Header

	// Final Block
	write_bit(f, 1);

	// Static Hufffman Compressed
	write_bit(f, 1);
	write_bit(f, 0);

	// Start compressed stream

	unsigned char c;
	int preprocessor = 0;
	while (!feof(stdin))
	{
		int ret = fread(&c, 1, 1, stdin);
		if (c == '%' && ret == 1)
		{
			fread(&c, 1, 1, stdin);
			if (c == '%')
			{
				write_literal(f, c);
			}
			else
			{
				char lenstr[256];
				int idx = 0;
				while (c != ',' && !feof(stdin))
				{
					lenstr[idx++] = c;
					fread(&c, 1, 1, stdin);
				}
				int len = atoi(lenstr);
				write_length(f, len);
				fread(&c, 1, 1, stdin);
				char diststr[256];
				idx = 0;
				while (c != ';' && !feof(stdin))
				{
					diststr[idx++] = c;
					fread(&c, 1, 1, stdin);
				}
				int dist = atoi(diststr);
				write_distance(f, dist);
			}
		}
		else if (ret == 1)
		{
			write_literal(f, c);
		}
	}

	write_literal(f, 256);
	
	flush(f);

	fclose(f);
}