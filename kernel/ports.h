#ifndef PORTS_H
#define PORTS_H

// Port I/O functions
unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char data);

#endif
