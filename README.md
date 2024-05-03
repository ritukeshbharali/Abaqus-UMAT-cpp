# Abaqus-UMAT-cpp

This repository demonstrates how an Abaqus user material (UMAT) can be called from a C++ program. The UMAT file (umat.f) is compile as a shared library. Thereafter, it dynamically loaded into the C++ program using 'dlfcn' library utilities.

### How to compile and run
On terminal, execute:
```sh
gfortran -shared -o libumat.so umat.f
g++ main.cpp -o main
./main
```