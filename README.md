# BmmP
A small program to load .bmp file and output to console and file.

compile with gcc.
compile as "bmp" file

Usage:
  bmp <options> <files>

Options:
  -z: don't skip 00 at every end of the rows (if haven't)
  -h: show help

And there is a bug. It may lost the first row and output only row 2th~end.
