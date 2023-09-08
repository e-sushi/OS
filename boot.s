/* constants for multiboot header */
.set MAGIC,     0xE85250D6 // magic lets bootloader find the header
.set ARCH,      0 
.set HEADERLEN, mb2_header_end - mb2_header_begin
.set CHECKSUM,  -(MAGIC + ARCH + HEADERLEN)  

/*

    Declare a multiboot header that marks the program as a kernel. These magic values are 
    documented in the multiboot standard. The bootloader will search for this signature in
    the first 8kb of the kernel file, aligned at a 32-bit boundary. The sig is in its own
    section so the header can be forced to be within the first 8kb of the kernel file.

*/

.section .multiboot2_header
mb2_header_begin:
    .align 8
    .long MAGIC
    .long ARCH
    .long HEADERLEN
    .long CHECKSUM

# framebuffer tag
.set FBHEADERLEN, mb2_framebuffer_end - mb2_framebuffer_req
mb2_framebuffer_req:
        .short 5 # tag 5 indicates that this is for a framebuffer
        .short 1 # don't ignore this tag if it is not supported
        .long FBHEADERLEN
        # preferred width/height/depth, leave as 0 to indicate we don't care
        .long 800
        .long 600
        .long 0
mb2_framebuffer_end:
# end tag
    .long 0
    .long 0
mb2_header_end:


/*

    The multiboot standard does not define the value of the stack pointer register (esp)
    and it is up to the kernel to provide a stack. This allocates room for a small stack
    by creating a symbol at the bottom of it, the allocating 16384 bytes for it, and finally 
    creating a symbol at the top. The stack grows downwards on x86. The stack is in its own
    section so it can be marked nobits, which means the kernel file is smaller because it 
    does not contain an uninitialized stack. The stack on x86 must be 16-byte aligned
    according to the System V ABI standard and de-facto extensions. The compiler will assume
    the stack is properly aligned and failure to align the stack results in undefined behavoir.

*/


.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 kb
stack_top:

/*

    The linker script specifies _start as the entry point to the kernel and the bootloader
    will jump to this position once the kernel has been loaded. It doesn't make sense to return
    from this function as the bootloader will be gone.

*/

.section .text
.global _start
.type _start, @function
_start:
    /*

        To setup a stack, we set the esp register to point to the top of the stack. This is 
        done in ASM as languages such as C cannot function without a stack.

    */

    mov $stack_top, %esp

    /*

        Enter the high-level kernel, pushing some arguments for working 
        with the multiboot header.

    */

    pushl %ebx // push the pointer to the multiboot info struct
    pushl %eax // push the magic value 
    call kernel_main
 
    /*

        If the system has no more to do, put the computer into an infinite loop.
        1.) Disable interrupts with cli
        2.) Wait for the next interrupt to arrive with hlt, locking up the computer.
        3.) Jump to the hlt instruction if we ever wake due to a non-maskable interupt 
            or due to system management mode

    */

    cli
1:  hlt
    jmp 1b

/*

    Set the size of the _start symbol to the current location '.' minus its start.
    Useful for debugging or when call tracing is implemented.

*/

.size _start, . - _start
