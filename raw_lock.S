.globl raw_lock
raw_lock:
pushq %rbp
movq  %rsp, %rbp
movb %dl, %al
movq %rdi, %rcx
movb %sil, %dl
lock cmpxchg %dl,(%rcx)
popq  %rbp
retq