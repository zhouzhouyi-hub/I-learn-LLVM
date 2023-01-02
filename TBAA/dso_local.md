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

Then I use clang to generate LLVM IR: clang -S -emit-llvm hello.c, I get [hello.ll](examples/hello.ll))

```
define dso_local void @hello() #0 {
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @bar() #0 {
  call void @hello()
  ret i32 0
}
```

Then I copy hello.ll to hello1.ll: cp hello.ll hello1.ll, and remove dso_local definition from function hello, I get [hello1.ll](examples/hello1.ll)


```
define void @hello() #0 {
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @bar() #0 {
  call void @hello()
  ret i32 0
}
```
## Compile LLVM IR to assembling file
By invoking: llc  -relocation-model=pic hello.ll, I get hello.s

By invoking: llc  -relocation-model=pic hello1.ll, I get hello1.s

I can see the difference between hello.s and hello1.s, diff -u hello.s hello1.s:
```
 hello:                                  # @hello
-.Lhello$local:
 	.cfi_startproc
 # %bb.0:
 	pushq	%rbp
@@ -31,7 +30,7 @@
 	.cfi_offset %rbp, -16
 	movq	%rsp, %rbp
 	.cfi_def_cfa_register %rbp
-	callq	.Lhello$local
+	callq	hello@PLT
 	xorl	%eax, %eax
 	popq	%rbp
 	.cfi_def_cfa %rsp, 8
```
PLT mean position 'procedure linkage table' 
