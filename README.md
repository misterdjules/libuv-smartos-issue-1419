This repository is simple reproduction of [joyent/libuv](https://github.com/joyent/libuv/)'s issue [joyent/libuv#1419](https://github.com/joyent/libuv/issues/1419).

How to build it?
================

To build this code, you will first need to clone joyent/libuv and build it.
Then you can simply use a C compiler to build the binary executable file that reproduces the issue:
```
gcc -o repro read-stdin-pipe-and-close.c -I libuv/include libuv/.libs/libuv.a
```

How to reproduce the issue?
===========================

Simply run:
```
./repro
```

If the issue happens, the program will assert, otherwise it will exit gracefully.
