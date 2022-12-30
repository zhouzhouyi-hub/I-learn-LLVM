void bar(int *x, long *y) {
	for (int i = 0; i < 1000; ++i)
		x[i] = *y + 42;
}
                                                                                                               
void foo(int *x, int *y) {  
	for (int i = 0; i < 1000; ++i)
		x[i] = *y + 42; 
}  
