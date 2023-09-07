ENTRY(_start)

SECTIONS {
    /* begin putting sections at 1 Mb */
    . = 1M;

    .text BLOCK(4K) : ALIGN(4K) {
        *(.multiboot2_header)
        *(.text)
    }

}