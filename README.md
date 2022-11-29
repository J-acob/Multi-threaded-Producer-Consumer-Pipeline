# Multi-threaded-Producer-Consumer-Pipeline
An implementation to solve the "Producer Consumer" problem. Build with C and uses p(osix) threads library.


---
# Building

## Requirements
- GCC, the Makefile expects this
- Compiling on a linux/unix system. Undefined behavior may occur if the windows variant of pthreads is used. The program expects unix characters so `^M` and such will cause issues
---

# Running/Testing
- The makefile supports testing by using the command `make test<n>` where <n> can be 1, 2, or 3. The outputted data will be in the `data/output` directory.
--- 