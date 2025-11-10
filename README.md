# Cache Lab

## Setup

See the canvas page for setup information. At a minimum you'll need to make sure
that your system has valgrind, make, and a C compiler (preferably gcc) installed.

## Building The Code

To build your code just run the following:

```shell
make
```

This will build the `csim` executable, the `test-trans` executable, and the
`trace-gen` executable. You don't necessarily need to run these executables
directly, instructions on running the grading script is given below.

When you build you code there will be several warning flags set. These flags
cause `gcc` to emit more precise error messages than usual. For this assignment
the code that you submit must not build with any errors.

When you submit your code you will run `make submit`, which will turn on a flag
in `gcc` that causes any warning messages to be treated as errors, which means
that you build will fail if you have warnings in your code. Make sure to fix all
the warnings before submitting.

## Editing the Code:

You will edit the two following files:

1. `csim.c`: Your cache simulator
2. `trans.c`: Your transpose function

See the canvas page for more information about updating these files and what are you
are supposed to build for this lab.

### Help on Parsing

`C` is not a great language for parsing strings. However, there are a few standard
library functions that you can use to help with this that are in the `scanf` family.

I would recommend taking a look through [cppreference's scanf](https://en.cppreference.com/w/c/io/fscanf)
page on the functions, as it has a good explanation of how it works and how to use
it. Also remember that you can assume the input is well formed, you don't really
need to handle corner cases or poorly formatted input.

## Running the autograders:

Before running the autograders, make sure to compile your code:

```shell
make
```

To check the correctness of your simulator:

```shell
./test-csim
```

Check the correctness and performance of your transpose functions:

```shell
./test-trans -M 32 -N 32
./test-trans -M 64 -N 64
./test-trans -M 61 -N 67
```

Check everything at once (this is the program that your instructor runs):

```shell
./driver.py    
```

# Overview of Each File in the Handout

- `Makefile`: Builds the simulator and tools

- `CMakeLists.txt`: The build system generator

- `README.md` This file

- `driver.py*`: The driver program, runs test-csim and test-trans

- `cachelab.c`: Required helper functions

- `cachelab.h`: Required header file

- `csim-ref*`: The executable reference cache simulator

- `test-csim`: Tests your cache simulator

- `test-trans.c`: Tests your transpose function

- `tracegen.c`: Helper program used by test-trans

- `traces/`: Trace files used by test-csim.c
