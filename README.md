# text-editor

A (very WIP) simple terminal-based text editor written in C.
Currently, reading/writing to/from files is not supported.

## prerequisites:
You'll need to have `make` and `gcc` installed.

## Building:
Run `make`

** Currently, the Makefile is not great, and expects you to use `gcc` as your compiler.

## Running:

### Linux:
Run `./build/text-editor`
This program relies heavily on ANSI escape sequences,
so make sure to use a terminal with ANSI escape sequence support.

### Windows:
Not supported.
