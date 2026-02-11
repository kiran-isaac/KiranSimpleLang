# Bask
Bask is a compiled, statically-typed systems language targeting the LLVM-14 backend. While syntactically a (loose) subset of C, the project serves as a deep dive into compiler architecture, Abstract Syntax Tree (AST) construction, and the mapping of high-level semantics to LLVM Intermediate Representation (IR).

The name is a nod to the Basking shark, my favorite! 

## The language
Supported:
- Declaration and assignment of integers, floats and boolean variables
- Declaration and definition of functions, and declaration of external functions which can be linked to later
- Operations on integers and floats
- If statements
- function calls
- Recursion
- Including files

## Examples

Here is a program that calculates the factorial of 15:
```c
use "io.bsk";

int no_body();

int factorial(int n) {
  if (n == 0) {
    return 1;
  }
  return n * factorial(n - 1);
}

int main() {
  put_int(factorial(15));
  return 0;
}
```

Finally, here is a program that calculates the 10th fibonacci number:
```c
use "io.bsk";

int fib(int n) {
  if (n == 0) {
    return 0;
  }
  if (n == 1) {
    return 1;
  }
  return fib(n - 1) + fib(n - 2);
}

int main() {
  put_int(fib(10));
  return 0;
}
```

## The compiler
The compiler is written in C++ and uses the LLVM library to generate the IR code. The compiler can generate the AST, the IR code, or compile and link to an executable. 

The compiler can be run using the `baskc` binary. This is the help message:
```
Usage: bask [options] [file]
Options:
  -o <file>  Output file
  -l <path>  Library path
  -O <level> Optimization level
  -c         Compile
  -r         Run
  -i         Generate IR
  -a         Generate AST
  -h         Display this help message
```


## Building
This project depends on llvm-14.0.0. You can install it using your package manager. For example, on Ubuntu, you can run:
```bash
sudo apt-get install llvm-14
```

Then, you can build the project using CMake:
```bash
mkdir build
cd build
cmake ..
make
```

You must also build the standard library:
```bash
cd lib
make
```
You should put a simlink to the standard library in the /usr/local/lib directory so it can be found by the compiler:
```bash
sudo ln -s lib/build /usr/local/lib/bask
```
Or you can specify the path to the standard library in the cli using the `-l` flag.

## Running the tests
The unit tests are written using gtest. You can run them by executing the `baskctest` binary in the build directory.

The other tests can be run by running the "all.sh" script in the tests directory, or a specific suite (labled TESTSUITE_*.sh). The testsuites will run the compiler on the test files in the corresponding directory and compare the output to the expected output.

