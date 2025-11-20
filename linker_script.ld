/* W blog post
 * https://blog.thea.codes/the-most-thoroughly-commented-linker-script/
 */

ENTRY(Reset_Handler)

_min_stack_size = 0x400;

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000,  LENGTH = 64K
    RAM   (rwx) : ORIGIN = 0x20000000,  LENGTH = 20K
}

SECTIONS
{
    /* Interrupt vector table */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH

    /* Program code */
    .text :
    {
        . = ALIGN(4);
        *(.text .text* .gnu.linkonce.t.*)
        KEEP (*(.init))
        KEEP (*(.fini))
        . = ALIGN(4);
        /* Variable for the end address of the .text section */
        _etext = .;
    } > FLASH

    /* Constant data */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata .rodata* .gnu.linkonce.r.*)
        . = ALIGN(4);
    } > FLASH

    /* C's designated constructors, i.e.
     *  void init(void) __attribute__((constructor));
     */
    .preinit_array (READONLY) :
    {
        . = ALIGN(4);
        KEEP(*(.init))
        . = ALIGN(4);
        __preinit_array_start = .;
        KEEP (*(.preinit_array))
        __preinit_array_end = .;
        . = ALIGN(4);
    } > FLASH

    /* C's designated constructors, i.e.
     *  void init(void) __attribute__((constructor));
     */
    .init_array (READONLY) :
    {
        . = ALIGN(4);
        __init_array_start = .;
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        __init_array_end = .;
        . = ALIGN(4);
    } > FLASH

    /* C's designated destructors, i.i.
     *  void destroy(void) __attribute__((destructor));
     */
    .fini_array (READONLY) :
    {
        . = ALIGN(4);
        KEEP(*(.fini))
        . = ALIGN(4);
        __fini_array_start = .;
        KEEP (*(.fini_array))
        KEEP (*(SORT(.fini_array.*)))
        __fini_array_end = .;
        . = ALIGN(4);
    } > FLASH

    _sidata = LOADADDR(.data);

    /* Initialized data to move from flash to ram at runtime */
    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data .data*)
        *(.ramfunc .ramfunc*)
        . = ALIGN(4);
        _edata = .;
    } > RAM AT> FLASH

    /* Uninitialized data */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss .bss.*)
        . = ALIGN(4);
        _ebss = .;
    } > RAM

    .stack (NOLOAD) :
    {
        . = ALIGN(8);
        end = .;
        _sstack = .;
        . = . + _min_stack_size;
        /* . = ORIGIN(RAM) + LENGTH(RAM); */
        . = ALIGN(8);
        _estack = .;
    } > RAM
}
