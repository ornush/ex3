.section .text
.globl switch2
switch2:
movq $0 ,%rax
subq $21,%rdx
cmpq $10,%rdx
ja .Default
jmp *.JumpTable(,%rdx,8)
.L7:
movq (%rdi),%rcx 
movq %rcx, (%rsi) 
movq (%rsi),%rax 
jmp .Done
.L5:
movq (%rsi),%rcx 
addq %rcx, (%rdi) 
movq (%rdi),%rax 
jmp .Done
.L9:
.L10:
subq $59,(%rdi) 
movq (%rdi),%rcx 
subq %rcx, (%rsi) 
jmp .Done
.L3:
addq $60,(%rdi) 
.L1:
movq (%rsi),%rax 
imulq %rax ,%rax
jmp .Done
.L11:
movq (%rsi),%rax 
movq (%rdi),%rcx 
sar %cl,%rax 
jmp .Done
.Default:
movq $12,%rax 
salq $3,%rax 
.Done:
ret
.section .rodata
.align 8
.JumpTable:	
.quad .L1
.quad .Default
.quad .L3
.quad .Default
.quad .L5
.quad .Default
.quad .L7
.quad .Default
.quad .L9
.quad .L10
.quad .L11
.quad .Default
