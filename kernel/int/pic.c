#include <cpu/ports.h>
#include <int/pic.h>

#define MASTER_COMMAND	0x20
#define MASTER_DATA		0x21
#define SLAVE_COMMAND	0xA0
#define SLAVE_DATA		0xA1

#define PIC_EOI			0x20

#define ICW1_ICW4		0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE		0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL		0x08		/* Level triggered (edge) mode */
#define ICW1_INIT		0x10		/* Initialization - required! */

#define ICW4_8086		0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO		0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM		0x10		/* Special fully nested (not) */

void pic_init(void) {
	outb(MASTER_COMMAND, ICW1_INIT | ICW1_ICW4);
	outb(SLAVE_COMMAND, ICW1_INIT | ICW1_ICW4);
	outb(MASTER_DATA, 32);
	outb(SLAVE_DATA, 40);
	outb(MASTER_DATA, 4);
	outb(SLAVE_DATA, 2);

	outb(MASTER_DATA, ICW4_8086);
	outb(SLAVE_DATA, ICW4_8086);

	outb(MASTER_DATA, 0x00);
	outb(SLAVE_DATA, 0x00);
}

void pic_eoi(uint8_t irq) {
	if(irq > 7) {
		outb(SLAVE_COMMAND, PIC_EOI);
	}

	outb(MASTER_COMMAND, PIC_EOI);
}
