
// 2 MiB Stack
#define STACK_SIZE 0x200000

.text
.code64

.extern __elk_crt__get_argv
.type __elk_crt__get_argv,@function

.extern __elk_crt__init_stdstreams
.type __elk_crt__init_stdstreams,@function

.global _start
_start: 

// mmap An Anonymous Stack
    movq $7, %rax // mmap
    movq $0, %rdi // ASPACE_CURRENT
    movq $0, %rsi // NULL_FD
    movq $0, %rdx // file_offset

    // Pass in a pointer to __mmap_where to stash the address of our stack
    movabsq $__mmap_where, %r8

    movq $STACK_SIZE, %r9 // size
    movq $(0b10), %r10 // MMAP_ANON
    orq $(1ULL<<3), %r10 // MMAP_PROT_READ
    orq $(1ULL<<4), %r10 // MMAP_PROT_WRITE
    syscall

    testq %rax, %rax
    jnz __do_fail_exit

    // set the stack pointer
    movq __mmap_where, %r8
    addq $STACK_SIZE, %r8
    movq %r8, %rsp

    // Setup stdout, stdin and stderr
    callq __elk_crt__init_stdstreams

    testq %rax, %rax
    jnz __do_fail_exit

    // Setup ARGV
    subq $16, %rsp
    // argc ptr
    leaq 0(%rsp), %rdi
    // argv ptr
    leaq 8(%rsp), %rsi
    
    callq __elk_crt__get_argv

    // argc
    movq 0(%rsp), %rdi
    // argv
    movq 8(%rsp), %rsi
    addq $16, %rsp

    // call the main function
    callq main

    // Exit using the value returned from main
    movq %rax, %rdi
    movq $0, %rax
    syscall

__do_fail_exit:
    movq %rax, %rdi // exitcode of mmap
    movq $0, %rax // exit
    syscall


.global _thread_start
_thread_start:

    // Stash our argument (thread func)
    movq %rdi, %rbx

    // mmap An Anonymous Stack
    movq $7, %rax // mmap
    movq $0, %rdi // ASPACE_CURRENT
    movq $0, %rsi // NULL_FD
    movq $0, %rdx // file_offset

    movabsq $__mmap_where, %r8 // where

    movq $STACK_SIZE, %r9 // size
    movq $(0b10), %r10 // MMAP_ANON
    orq $(1ULL<<3), %r10 // MMAP_PROT_READ
    orq $(1ULL<<4), %r10 // MMAP_PROT_WRITE
    syscall

    testq %rax, %rax
    jnz __thread_start_stack_map_fail 

    // set up the thread stack
    movq __mmap_where, %r8
    addq $STACK_SIZE, %r8
    movq %r8, %rsp

    subq $8, %rsp

    callq *%rbx

    addq $8, %rsp

    movq %rax, %rdi
    movq $0, %rax
    syscall

__thread_start_stack_map_fail:
    movq %rax, %rdi // exitcode of mmap
    movq $0, %rax
    syscall

.section .data

__mmap_where:
.quad 0

