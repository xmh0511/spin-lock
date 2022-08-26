.globl raw_lock
raw_lock:
pushq %rbp
movq  %rsp, %rbp
movl %edx, %eax
movq %rdi, %rcx
movl %esi, %edx
lock cmpxchg %edx,(%rcx)
popq  %rbp
retq