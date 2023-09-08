; constants for multiboot header 
%define MAGIC      0xE85250D6 
%define ARCH       0 
%define HEADERLEN  mb2_header_end - mb2_header_begin
%define CHECKSUM   -(MAGIC + ARCH + HEADERLEN)  





; 
; Declare a multiboot header that marks the program as a kernel. These magic values are 
; documented in the multiboot standard. The bootloader will search for this signature in
; the first 8kb of the kernel file, aligned at a 32-bit boundary. The sig is in its own
; section so the header can be forced to be within the first 8kb of the kernel file.
; 

section .multiboot2_header
mb2_header_begin:
    align 8
    dd MAGIC
    dd ARCH
    dd HEADERLEN
    dd CHECKSUM

; framebuffer tag
mb2_framebuffer_req:
        dw 5 ; tag 5 indicates that this is for a framebuffer
        dw 0 ; don't ignore this tag if it is not supported
        dd mb2_framebuffer_end - mb2_framebuffer_req
        ; preferred width/height/depth, leave as 0 to indicate we don't care
        dd 800
        dd 600
        dd 0
mb2_framebuffer_end:
; end tag
    dd 8
    dd 0
mb2_header_end:


;
; The multiboot standard does not define the value of the stack pointer register (esp)
; and it is up to the kernel to provide a stack. This allocates room for a small stack
; by creating a symbol at the bottom of it, the allocating 16384 bytes for it, and finally 
; creating a symbol at the top. The stack grows downwards on x86. The stack is in its own
; section so it can be marked nobits, which means the kernel file is smaller because it 
; does not contain an uninitialized stack. The stack on x86 must be 16-byte aligned
; according to the System V ABI standard and de-facto extensions. The compiler will assume
; the stack is properly aligned and failure to align the stack results in undefined behavoir.
;


section .bss
align 16
stack_bottom:
resb 16384 ; 16 kb
stack_top:


;
; The linker script specifies _start as the entry point to the kernel and the bootloader
; will jump to this position once the kernel has been loaded. It doesn't make sense to return
; from this function as the bootloader will be gone.
;


section .text
global _start:function (_start.end - _start)
_start:
    ; disable interupts so that we can setup a GDT
    cli

    %define port 0x3f8

    mov ax, 0x00
    out port + 1, ax
    mov ax, 0x80
    out port + 3, ax
    mov ax, 0x03
    out port + 0, ax
    mov ax, 0x00
    out port + 1, ax
    mov ax, 0x03
    out port + 3, ax
    mov ax, 0xc7
    out port + 2, ax
    mov ax, 0x0b
    out port + 4, ax
    mov ax, 0x1e
    out port + 4, ax
    mov ax, 0xae
    out port + 0, ax

    in ax, port
    cmp ax, 0xae
    je .good
    hlt
.good:
    mov ax, 0x0f
    out port + 4, ax

    ;
    ; To setup a stack, we set the esp register to point to the top of the stack. This is 
    ; done in ASM as languages such as C cannot function without a stack.
    ;

    mov esp, stack_top
    
    ;
    ; Enter the high-level kernel, pushing some arguments for working 
    ; with the multiboot header.
    ;

    push rbx ; push the pointer to the multiboot info struct
    push rax ; push the magic value 
    extern kernel_main
    call kernel_main
 
    
    ;
    ; If the system has no more to do, put the computer into an infinite loop.
    ; 1.) Disable interrupts with cli
    ; 2.) Wait for the next interrupt to arrive with hlt, locking up the computer.
    ; 3.) Jump to the hlt instruction if we ever wake due to a non-maskable interupt 
    ;     or due to system management mode
    ;

    cli
.loop: 
    hlt
    jmp .loop
.end:

gdtr dw 0 ; limit storage
     dd 0 ; for base storage

; this function takes 2 arguments: 
;   limit (esp + 4)
;   base  (esp + 8)
global set_gdt:function (set_gdt.end - set_gdt)
set_gdt: 
    mov ax, [esp + 4]
    mov [gdtr], ax 
    mov eax, [esp + 8]
    mov [gdtr + 2], eax
    lgdt [gdtr]
    ret
.end:
