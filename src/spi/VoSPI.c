#include "VoSPI.h"

/*-------------- Global Variables -------------*/
/** input and output buffers (arrays) of the size BSIZE*SEGMENTS_IN_FRAME               */
uint8	 framebuf[SEGMENTS_IN_FRAME][BSIZE];
uint8    pkbuf[164];
uint8    sbuf[164*59];
/*GPIO file handler*/
int ff;
int max=1600;
/*--------------- Constants------------------- */

/**
 * Definition of the input device
*/
const int8 spidev[]="/dev/spidev1.0";
const int8 path[] = "/sys/class/gpio/gpio115/value";
/*------------ Funtions ----------------------*/

/**
 * SPI port open function 
 *
 */
int  SpiOpen(){

	int fd, ff;
	fd = open(spidev, O_RDWR);
	if (fd < 0) {
		perror("open");
		return 1;
	}
	else {
    GpioInit();
    //write(ff, "1", 1);
		return fd;
  }
}

int GpioInit(){

 ff = open(path, O_WRONLY);
 return ff;
}

void GpioClose(int ff){

 close(ff);
}

/**
 * SPI Close Port
 */
int SpiClose(int fd){

	close(fd);
}

/**
 * SPI Config Port 
 */
int SpiConfig(int fd){

	uint8   bits = 8;
	uint32    speed = 16000000;
	uint32    mode = 3;
	int		ret;

	ret = ioctl(fd, SPI_IOC_WR_MODE32, &mode);
	if (ret == -1)
		pabort("can't set spi mode wr");

	ret = ioctl(fd, SPI_IOC_RD_MODE32, &mode);
	if (ret == -1)
		pabort("can't get spi mode rd");

	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");

	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");
/*
	printf("spi mode: 0x%x\n", mode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
*/
}

/**
 * Debug Output
 *
 */
void pabort(const int8 *s)
{
	perror(s);
	abort();
}

/**
 *  readspi
 */
void readspi(int fd)
{
        int status;
        int count=0;
        unsigned short  pkHead;
        unsigned short  Descarted;
        unsigned short  nSegment;
        uint8 flag=1;

	memset(framebuf, 0, sizeof framebuf);


        /*Assert CS Line*/
	write(ff, "0", 1);
        /* Get valid package */
	while(flag == 1){
	     status = read(fd, pkbuf, BYTES_IN_PACKET);
             if (status < 0) {
		perror("SPI_IOC_MESSAGE");
		return;
	     }

        pkHead = (unsigned short)((unsigned short )(pkbuf[0] << 8) |(unsigned short)( pkbuf[1]));
        Descarted = (pkHead & 0x0F00) >> 8;
        count=(count+1)%60;
        if(Descarted != 0x0F)
                flag = 0;

	} /* end of while */

        /* Deassert CS Line*/
	write(ff, "1", 1);
        /* Assert CS Line*/
        write(ff, "0", 1);
        if(count != 0){
		status = read(fd, sbuf , (59)*164);
		if (status < 0) {
			perror("SPI_IOC_MESSAGE");
			return;
		}
	}
        /* Deassert CS Line*/
	write(ff, "1", 1);
        /* Time Synchronization*/
        usleep(max);
        /* Assert CS Line*/
        write(ff, "0", 1);
        for(int n=0; n < SEGMENTS_IN_FRAME; n++){
                status = read(fd, framebuf[n] , 60*164);
		if (status < 0) {
			perror("SPI_IOC_MESSAGE");
			return;
		}
        /* Deassert CS Line*/
        write(ff, "1", 1);
        /* Time Synchronization*/
        usleep(max);
        /* Assert CS Line*/
        write(ff, "0", 1);
	}
        /* Deassert CS Line*/ 
	write(ff, "1", 1);


       /*Segment Detection Valid*/
        for(int k=0 ;k<20; k++){
	     nSegment =(unsigned short)(framebuf[k][0+20*164]);
	     if (nSegment == 0x10){
                 /*Valid Pixel Function*/
	          LeptonPixel(k);
                  k=21;
	         }
            }

            /*SPI Reading Print*/
/*      printf("First valid packet \n");
        for (int j=0; j < BYTES_IN_PACKET; j++)
	      	printf("%02x ", pkbuf[j]);

                printf("\n");

         printf("Rest of the first segment  \n");
         for (int j=0; j < PACKETS_IN_SEGMENT-1; j++){
  		for (int i=0; i < BYTES_IN_PACKET; i++)
	      	printf("%02x ", sbuf[i+j*164]);
                printf("\n");
              }

         printf("\n");

         printf("Next segments - full frame  \n");
         for(int k=0; k < SEGMENTS_IN_FRAME; k++){
  	       for (int j=0; j < PACKETS_IN_SEGMENT; j++){
  		     for (int i=0; i < BYTES_IN_PACKET; i++)
	      	           printf("%02x ", framebuf[k][i+j*164]);
  	                   printf("\n");
                   }
                printf("-------Frame %d --------\n", k);
              }

         printf("\n");
         printf("count:%d\n",count);                            */
}

void  test(void){

}

/*Valid Pixel Function*/
void LeptonPixel(int k) {

unsigned short pixel_1;
int jota=k+4;
int p=0;
printf("\n");

       /*valid Pixel Printing*/
	for(int m=k ;m<(jota);m++){
	     for(int j=0;j<60;j++){
		  for(int i=4;i<164;i++){
			pixel_1=((unsigned short )(framebuf[m][(i)+(j)*164] << 8) |(unsigned short )framebuf[m][((i)+1)+(j)*164]);
                        printf("%02x ",pixel_1);
                        i=i+1;
                        p=p+1;
                        if(p == 160){
                            p=0;
                            printf("\n");
                          }
                      }
	        }
           }

return;
}
