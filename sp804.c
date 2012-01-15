#include <utility.h>

#define SP804_BASE	0x10011000
#define Timer1Load	0x00
#define Timer1Value	0x04
#define Timer1Control	0x08
#define Timer1IntClr	0x0C
#define Timer1RIS	0x10
#define Timer1MIS	0x14
#define Timer1BGLoad	0x18

void sp804_write(unsigned int data, unsigned int offset)
{
    raw_write32(data, (void *)(SP804_BASE + offset));
}

unsigned int sp804_read(unsigned int offset)
{
    return raw_read32((void *)(SP804_BASE + offset));
}

void start_timer()
{
    sp804_write(0xfffff, Timer1Load);
    sp804_write(0xa1, Timer1Control);
}
