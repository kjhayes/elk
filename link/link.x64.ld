
ENTRY(_start)

SECTIONS {

    . = 0x1000;

    .text : AT(0x1000)
    {
        *(.text)
        *(.rodata)
    }

    .data ALIGN(0x1000) :
    {
        *(.data)
    }

    .bss ALIGN(0x1000) :
    {
        *(.bss)
        *(COMMON)
    }

    /DISCARD/ : {
        *(.eh_frame* .eh_frame.*)
        *(.comment* .comment.*)
    }
}

