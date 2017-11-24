/*
 * libc_functions.c
 *
 *  Created on: 20 Nov 2017
 *      Author: leonard.bise
 *  Description: Toolchain does not define the following functions that
 *  are needed to use various IO functions of the libc.
 *  _write and _read are used by printf and scanf respectively.
 *  _sbrk is used for malloc
 */
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "bcm2835_miniuart.h"

#ifdef __DEBUG_LIBC_FUNCS__
#include <stdio.h>
char buffer[80];
#endif

/* Set in linker script */
extern caddr_t _heap_start_addr;
caddr_t* _heap_start = &_heap_start_addr;

/* Write cnt byte from the buffer buf to the stream associated
 * with the file descriptor fd. */
int _write (int fd, const void *buf, size_t count) {
#ifdef __DEBUG_LIBC_FUNCS__
	sprintf(buffer, "_write count %d\n\r", count);
	bcm2835_miniuart_sendstr(buffer);
#endif
	bcm2835_miniuart_send_blocking(buf, count);
	/* We assume we wrote everything */
	return count;
}

/* Read cnt byte from the stream associated with the file
 * descriptor fd and put them into the buffer buf. */
int _read (int fd, void *buf, size_t count) {
	bcm2835_miniuart_receive_blocking(buf, count);
	return count;
}

/* sbrk() increments the available data space by incr bytes and
 * returns a pointer to the start of the new area. */
caddr_t _sbrk(int incr) {
	caddr_t* curr_heap = _heap_start;
	_heap_start += incr / sizeof(caddr_t);
#ifdef __DEBUG_LIBC_FUNCS__
	sprintf(buffer, "_sbrk _head_start 0x%x curr_heap 0x%x count %d\n\r", (unsigned int)_heap_start, (unsigned int)curr_heap, incr);
	bcm2835_miniuart_sendstr(buffer);
#endif
    return (caddr_t) curr_heap;
}

/* The close() call deletes a descriptor from the per-process object reference table. */
int _close(int fildes) {
#ifdef __DEBUG_LIBC_FUNCS__
	bcm2835_miniuart_sendstr("_close\n\r");
#endif
	return -1;
}

/* The fstat() obtains the same information about an open file known by the file descriptor
 * fildes. */
int _fstat(int fildes, struct stat *buf) {
#ifdef __DEBUG_LIBC_FUNCS__
	bcm2835_miniuart_sendstr("_fstat\n\r");
#endif
	return -1;
}

/* The isatty() function determines if the file descriptor fd refers to a valid terminal type
 * device. */
int _isatty(int fd) {
#ifdef __DEBUG_LIBC_FUNCS__
	bcm2835_miniuart_sendstr("_isatty\n\r");
#endif
	return 0;
}

/* The lseek() function repositions the offset of the file descriptor fildes to the argument
 * offset, according to the directive whence. */
off_t _lseek(int fildes, off_t offset, int whence) {
#ifdef __DEBUG_LIBC_FUNCS__
	bcm2835_miniuart_sendstr("_lseek\n\r");
#endif
	return -1;
}
