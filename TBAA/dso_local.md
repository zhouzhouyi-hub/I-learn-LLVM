# My Little Exploration of 'dso_local'
This is my little exploration of dso_local
## Introduction
Official document of LLVM says "'dso_local' means this function will resolve to a symbol within the same linkage unit". As a beginner, I think I need some example to understand that definition, so I did some exploration.
## Example LLVM IR
Firstly I construct a C program:

```
void hello() {
}
int bar()
{
        hello();
        return 0;
}
```
