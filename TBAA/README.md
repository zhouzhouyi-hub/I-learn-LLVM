# I-learn-TBAA
This is how I learn TBAA
## Introduction
TBAA (Type-Based Alias Analysis) in LLVM is an optimization that remove unneeded
computations (for example, from loop) based on type information[1]. 

For example, consider following two functions:
```
1  void bar(int *x, long *y) {
2    for (int i = 0; i < 1000; ++i)
3        x[i] = *y + 42;
4  }
                                                                                                               
5  void foo(int *x, int *y) {  
6    for (int i = 0; i < 1000; ++i)
7        x[i] = *y + 42; 
8  }  
```
In function bar, because 'int' and 'long' are different types, store to x[i] in line 3 
can not affect the values in y, 
so we can optimize the load of y outside of the loop. But, in function foo, 
x and y have same type 'int', store to x[i] in line 7 may affect the values in y, we can
not optimize the load of y outside of the loop.
## References
[1] https://stefansf.de/post/type-based-alias-analysis/
## Acknowledgements
Thank MaskRay (Fangrui Song) for teaching me to use fenced code block which make the paper much better.
