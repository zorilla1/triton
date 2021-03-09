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
LENGTH must be at least 3 and POSITION must be at least 1. Copy commands use between 15 and 31 bits in the compressed stream, so no point in copying anything smaller than 3 characters.
