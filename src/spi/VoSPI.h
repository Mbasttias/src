/*
 * SPI buffer size change 

 * https://e2e.ti.com/support/arm/sitara_arm/f/791/t/343692?Changing-SPI-Bufsiz-on-Beaglebone-Black  */


#ifndef VOSPI_H_
#define VOSPI_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

/* Important Macro Definitions */
#define BYTES_IN_PACKET        164
#define PACKETS_IN_SEGMENT      60
#define ROWS                   120  /* this macro determines row numbre of a frame in basic mode     */
#define COLS                   160  /* this macro determines column number of a frame in basic mode  */
#define BSIZE               164*60  /* this macro determines a buffer length in one message */
#define SEGMENTS_IN_FRAME       20

typedef unsigned char uint8;
typedef unsigned int  uint32;
typedef char          int8;

/* Function declarations                           */
void 	pabort(const int8 *s);
void 	do_msg(int fd);
int 	SpiOpen();
int 	SpiClose(int fd);
int 	SpiConfig(int fd);
void 	readspi(int fd);
void    test(void);
int     GpioInit();
void    GpioClose(int ff);
void    LeptonPixel(int k);

#endif 
