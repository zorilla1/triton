# triton

A manual DEFLATE encoder for very tiny TIC-80 cartridges. Triton reads from standard input and writes the resulting TIC-80 cartridge to standard output.
```
./triton < README.lua > test.tic
```
Text will be encoded as a ZLIB stream wrapped in a TIC-80 cart. The copy command is
```
%LENGTH,POSITION;
```
where length and position are integers. The copy command copies LENGTH bytes from the specified POSITION in the output stream. POSITION is counted backwards from the end of the stream, so
```
Hello, World!%3,6;
```
will decompress to
```
Hello, World!Wor
```
LENGTH must be at least 3 and no more than 256. POSITION must be at least 1 and no more than 32768. If you need to use a % character in your stream, specify it with %% (two percentage signs).

Run-length encoding can be achieved in the following way
```
H%99,1;
```
will decompress to 100 H's and
```
Hello%100,5;
```
will decompress into 21 "Hello"'s

## Estimating output size

* TIC-80, ZLib, and DEFLATE headers use a combined 51 bits.
* ASCII characters use 8 bits each.
* Extended ASCII bytes (144 and above, technically) use 9 bits.
* Copy commands use between 15 and 31 bits. Smaller numbers have smaller encodings.

Because copy commands use about 3 bytes minimum, no point in copying anything smaller than 3 characters.
