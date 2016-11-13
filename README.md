# ListOpenDescriptors #

ListOpenDescriptors is a tool to assist in finding file descriptor leaks. Similar to Valgrind's *--track-fds=yes* flag it prints all open descriptors at the application exit. It works by defining a global destructor with the highest priority and enumerating the */proc/self/fd/* directory inside it.

By default, ListOpenDescriptors will print its output to *stdout*. You can override this behavior by providing the *FD_OUTPUT_PATH* environment variable.

### Compilation ###

ListOpenDescriptors must be compiled as a shared library:

```
#!bash

gcc -fPIC -shared -Wall -Wextra list_open_fds.c -o liblist_open_fds.so -ldl
```


### Usage ###

Preload *liblist_open_fds.so* into your executable (optionally, provide a path to an output file):

```
#!bash

LD_PRELOAD=./liblist_open_fds.so target_executable
# or:
FD_OUTPUT_PATH=/tmp/fd_out LD_PRELOAD=./liblist_open_fds.so target_executable
```