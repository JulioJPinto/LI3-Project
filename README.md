# LI3 PROJECT

### Final Grade: 19/20

- __1st Phase__: 18.68
- __2nd Phase__: 20.00 

## About the Project

This project was developed for the LI3 curricular unit of the Software Engineering degree at U.Minho for the 2022/2023 year.

## How to run the project

__Compiling the program__
```
$ make
```
__Running the program__
```
#interactive mode
$ ./programa-principal 

#or

#batch mode
$ ./programa-principal <dataset-folder> <input-file>
```
__Compiling the program for debugging__
```
$ make build-debug
```
__Compiling the program for tests¹__
```
$ make test
```
__Compiling the program for memory check²__
```
$ make leaks
```
__Deleting what the Makefile compiles__
```
$ make clean
```
__Formatting the code__
```
$ make format
```

__[1] :__ This command has been hard-coded therefore might not work unless. 
__[2] :__ This command works either with valgrind if you arent in MacOS or with leaks if you are in MacOS.

## Contributors
- [__Francisco Ferreira__](https://github.com/chicoferreira)
- [__Júlio Pinto__](https://github.com/JulioJPinto)


