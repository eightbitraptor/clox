# clox

A VM for the lox language from the Crafting Interpreters book, in C

## Setup

This builds using `meson` and `ninja` by default. 

### macOS build steps

```
brew install meson ninja

git clone https://github.com/eightbitraptor/clox
cd clox
meson setup build
meson compile -C build
```

The project can then be recompiled using

```
meson compile -C build
``` 

from the source directory. And run using

```
./build/clox
```

### Debugging VM instructions

Instruction dissassembly is controlled using the flag
`DEBUG_TRACE_INSTRUCTIONS` and is disabled by default. To enable configure it
using meson as a `c_arg` for your build directory

```
# to enable:
meson configure -Dc_arg=-DDEBUG_TRACE_INSTRUCTIONS build

# to disable again:
meson configure -Dc_arg=-DDEBUG_TRACE_INSTRUCTIONS=0 build

```
