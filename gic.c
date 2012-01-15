#include <utility.h>

#define GICC_BASE	0x1f000100
#define GICC_CTLR	0x0000
#define GICC_PMR	0x0004 
#define GICC_IAR	0x000C
#define GICC_EOIR	0x0010
#define GICC_RPR	0x0014
#define GICC_HPPIR	0x0018
#define GICC_DIR	0x1000

#define GICD_BASE	0x1f001000
#define GICD_CTLR	0x000
#define GICD_TYPER	0x004
#define GICD_ISENABLER	0x100	/* to 0x17C */
#define GICD_ICENABLER	0x180	/* to 0x1FC */
#define GICD_ISPENDRn	0x200	/* to 0x27C */
#define GICD_ICPENDRn	0x280	/* to 0x2FC */
#define GICD_ISACTIVER	0x300	/* to 0x37C */
#define GICD_ICACTIVER	0x380	/* to 0x3FC */
#define GICD_IPRIORITYR	0x400	/* to 0x7F8 */
#define GICD_ITARGETSR	0x800	/* to 0x81C */
#define GICD_ICFGR	0xC00	/* to 0xCFC */
#define GICD_NSACR	0xE00	/* to 0xEFC */
#define GICD_SGIR	0xF00 
#define GICD_CPENDSGIR	0xF10	/* to 0xF1C */
#define GICD_SPENDSGIR	0xF20	/* to 0xF2C */

void gic_cpu_write(unsigned int data, unsigned int offset)
{
    raw_write32(data, (void *)(GICC_BASE + offset));
}

unsigned int gic_cpu_read(unsigned int offset)
{
    return raw_read32((void *)(GICC_BASE + offset));
}

void gic_dist_write(unsigned int data, unsigned int offset)
{
    raw_write32(data, (void *)(GICD_BASE + offset));
}

unsigned int gic_dist_read(unsigned int offset)
{
    return raw_read32((void *)(GICD_BASE + offset));
}

int gic_enable_irq(unsigned int irq, unsigned int type)
{
    int reg_num;

    reg_num = irq / 32;

    gic_dist_write(0x1 << (irq % 32), GICD_ISENABLER + reg_num * 4);

    return 0;
}

int gic_disable_irq(unsigned int irq)
{
    int reg_num;

    reg_num = irq / 32;

    gic_dist_write(0x1 << (irq % 32), GICD_ICENABLER + reg_num * 4);

    return 0;
}

int gic_init()
{
    unsigned int type;
    int cpu_num, irq_num, reg_num;
    int i;

    /* disable distributer block */
    gic_dist_write(0x0, GICD_CTLR);

    /* disable cpu interface block */
    gic_cpu_write(0x0, GICC_CTLR);

    /* get gic types */
    type = gic_dist_read(GICD_TYPER);
    reg_num = (type & 0x1f) + 1;
    irq_num = reg_num * 32;
    cpu_num = ((type >> 5) & 0x7) + 1;
    printk("CPU:%d, IRQ:%d\n", cpu_num, irq_num);
    
    /* set the same priority to all interrupt */
    for (i = 8; i < reg_num * 8; i++) {
	gic_dist_write(0xa0a0a0a0a0, GICD_IPRIORITYR + i * 4);
    }

    /* set */
    for (i = 4; i < reg_num * 4; i++) {
	gic_dist_write(0x0, GICD_IPRIORITYR + i * 4);
    }
    
    /* disable all interrupt except PPI and SGI*/
    for (i = 1; i < reg_num; i++) {
	gic_dist_write(0xffffffff, GICD_ICENABLER + i * 4);
    }

    for (i = 8; i < reg_num * 8; i++) {
	gic_dist_write(0x0101010101, GICD_ITARGETSR + i * 4);
    }

    /* set cpu interface priority fileter */
    gic_cpu_write(0xf0, GICC_PMR);

    /* enable distributer block */
    gic_dist_write(0x1, GICD_CTLR);

    /* disable cpu interface block */
    gic_cpu_write(0x1, GICC_CTLR);

    gic_enable_irq(36, 0);
    gic_enable_irq(42, 0);

    return 0;
}
