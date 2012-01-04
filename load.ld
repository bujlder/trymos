ENTRY(start)

SECTIONS
{
	. = 0x10000;

	image_start = .;

	.text : 
	{
		startup.o
		*(.text)
	}

	.data : 
	{ 
		*(.data) 
	}

	.rodata :
	{
		*(.rodata)
	}

	.bss : 
	{
		*(.bss) 
	}

	image_end = .;

	. = . + 0x2000;
	stack_top = .;
}
