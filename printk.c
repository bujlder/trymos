/*
   Copyright (c) 1987,1997,2001 Prentice Hall
   All rights reserved.

   Redistribution and use of the MINIX operating system in source and
   binary forms, with or without modification, are permitted provided
   that the following conditions are met:

      * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

      * Redistributions in binary form must reproduce the above
        copyright notice, this list of conditions and the following
        disclaimer in the documentation and/or other materials provided
        with the distribution.

      * Neither the name of Prentice Hall nor the names of the software
        authors or contributors may be used to endorse or promote
        products derived from this software without specific prior
        written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS, AUTHORS, AND
   CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL PRENTICE HALL OR ANY AUTHORS OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdarg.h>
#include <utility.h>

#define NULL ((void *)0)

void raw_write8(unsigned char data, void *addr)
{
    *(volatile unsigned char *)(addr) = data;
}

void raw_write32(unsigned int data, void *addr)
{
    *(volatile unsigned int *)(addr) = data;
}

unsigned int raw_read32(void *addr)
{
    return *(volatile unsigned int *)addr;
}

void kputc(int c)
{
    raw_write32(c, (void *)0x10009000);
}

void printk(const char *fmt, ...)
{
    int c;
    int d;
    unsigned long u;
    int base;
    int negative;
    static char x2c[] = "0123456789ABCDEF";
    char ascii[8 * sizeof(long) / 3 + 2];
    char *s;
    va_list argp;

    va_start(argp, fmt);

    while((c=*fmt++) != 0) {

	if (c == '%') {
	    negative = 0;
	    s = NULL;
	    switch(c = *fmt++) {
	    case 'd':
		d = va_arg(argp, signed int);
		if (d < 0) { 
		    negative = 1; 
		    u = -d; 
		}
		else
		    u = d;
		base = 10;
		break;

	    case 'u':
		u = va_arg(argp, unsigned long);
		base = 10;
		break;

	    case 'x':
		u = va_arg(argp, unsigned long);
		base = 0x10;
		break;

	    case 's':
		s = va_arg(argp, char *);
		if (s == 0) s = "(null)";
		break;

	    case '%':
		s = "%";				 
		break;			

	    default:
		s = "%?";
		s[1] = c;
	    }

	    if (s == NULL) {
		s = ascii + sizeof(ascii)-1;
		*s = 0;			
		do { 
		    *--s = x2c[u % base];
		} while ((u /= base) > 0);
	    }

	    if (negative) 
		kputc('-');

	    while(*s != 0)
		kputc(*s++);
	}
	else if (c == '\n') {
	    kputc('\n');
	    kputc('\r');
	} 
	else {
	    kputc(c);
	}
    }
    va_end(argp);
}
