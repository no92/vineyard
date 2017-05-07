#include <cpu/ports.h>
#include <driver/uart.h>
#include <stdbool.h>

/* UART ports */
#define UART0	0x3F8
#define UART1	0x2F8
#define UART2	0x3E8
#define UART3	0x2E8

/* Transmitter Holding Buffer (DLAB = 0, I/O = W) */
#define THR		0
/* Receiver Buffer (DLAB = 0, I/O = R) */
#define RBR		0
/* Divisor Latch Low Byte (DLAB = 1) */
#define DLL		0
/* Interrupt Enable Register (DLAB = 0) */
#define IER		1
/* Divisor Latch High Byte (DLAB = 1) */
#define DLH		1
/* Interrupt Identification Register (I/O = R) */
#define IIR		2
/* FIFO Control Register (I/O = W) */
#define FCR		2
/* Line Control Register */
#define LCR		3
/* Modem Control Register */
#define MCR		4
/* Line Status Register (I/O = R) */
#define LSR		5
/* Modem Status Register (I/O = R) */
#define MSR		6
/* Scratch Register */
#define SR		7

void uart_init(void) {
	/* disable interrupts */
	outb(UART0 + IER, 0x00);

	/* set the Divisor Latch Access Bit (DLAB) for baud rate setting */
	outb(UART0 + LCR, 0x80);

	/* set low divisor to one */
	outb(UART0 + DLL, 0x01);
	/* set high divisor to zero */
	outb(UART0 + DLH, 0x00);

	/* set no parity (0bXX000XXX), one stop bit (0bXXXXX0XX) and 8-bit word length (0bXXXXXX11) and leave set break disabled (0bX0XXXXXX) */
	outb(UART0 + LCR, 0x03);

	/* enable FIFOs (bit 0), clear FIFOs (bits 1 & 2), 14-byte threshold (bits 6 & 7) */
	outb(UART0 + FCR, 0xC7);

	/* enable Auxiliary Output 2 to enable interrupts (bit 3), set Request To Send (bit 1) and set Data Terminal Ready (bit 0) */
	outb(UART0 + MCR, 0x0B);
}

static bool uart_transmit_empty(void) {
	/* if bit 5 is clear, the Transmit Buffer is empty */
	return inb(UART0 + LSR) & 0x20;
}

void uart_putc(uint8_t c) {
	while(!uart_transmit_empty());

	outb(UART0 + THR, c);
}
