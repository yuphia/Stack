# Protected stack, a data structure that uses LIFO method

## Table of contents

1. Starting and downloading
2. Concepts used in the program
3. Compatibility



# 1. Starting and downloading

First you have to download all the files in this repo except the build repository
    
Than you should run the program from the command line:
1) You have to change your current repository using "cd"
2) You create a log file using

``` bash
make createLogs 
```

3) You compile the program using the command

``` bash
make
```

4) Run it using command 
``` bash
make run 
```

IF make run gives you messages about errors (do in particular order)

1) Check log.txt and send me the errors (or try to debug yourself)
2) Compile using
``` bash
make debug
```
3) Run using
``` bash
make rundbg 
```
4) Try to debug or send me the compilation errors (if they exist)

# 2. Concepts used in the program

Protection is used as followed:

1)Checking for pointer to struct and pointers to buffer
2)Check hash of All the elements
3)Check canaries in the buffer and structure
4)Check stack for being destructed

# 3. Compatibility

The stack structure is a template, you can use it with most data types, but you need to provide:

1) A printing function that prints a single element of the stack buffer, which is put inside stack in the constructor as a pointer to function
2) Poison value that indicates that the element belongs to the bufferized region but not the stack itself

