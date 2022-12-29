# I-learn-TBAA
This is how I learn TBAA
## Introduction
TBAA (Type-Based Alias Analysis) in LLVM is an optimization that remove unneeded
computations (for example, from loop) based on type information. 

For example, consider following two functions:

void bar(int *x, long *y) {
    for (int i = 0; i < 1000; ++i)
        x[i] = *y + 42;
}
                                                                                                                             
                                                                                                               
void foo(int *x, int *y) {                                                                                                       
    for (int i = 0; i < 1000; ++i)                                                                                             
        x[i] = *y + 42;                                                                                                        
}  
## References
[1] https://stefansf.de/post/type-based-alias-analysis/
