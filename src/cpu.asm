section .text

; global get_cpu_features:function (get_cpu_features.end - get_cpu_features)
; get_cpu_features:
;     mov eax, 0x1
;     cpuid
;     mov rax, rbx
;     ret
; .end:

global enable_sse:function (enable_sse.end - enable_sse)
enable_sse:
    mov rax, cr0
    and ax, 0xfffb
    or  ax, 0x2
    mov cr0, rax
    mov rax, cr4
    or  ax, 3 << 9
    mov cr4, rax
    ret
.end:

idtr dw 0
     dq 0

global load_idt: function (load_idt.end - load_idt) 
load_idt: 
    mov  [rel idtr], rdi
    mov  [rel idtr+2], rsi
    lidt [rel idtr]
    ret 
.end:

%macro isr_err_stub 1
isr_stub_%+%1:
    call exception_handler
    iret q
%endmacro
%macro isr_no_err_stub 1
isr_stub_%+%1:
    call exception_handler
    iretq
%endmacro

extern exception_handler
isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

global isr_stub_table
isr_stub_table:
%assign i 0 
%rep    32 
    dq isr_stub_%+i 
%assign i i+1 
%endrep