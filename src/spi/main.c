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

/* Local header */
#include "VoSPI.h"

/* Main 								  */
int main(int argc, char **argv)
{
	int		fd;
	clock_t     launch, done; 
        double      diff;
	
        fd = SpiOpen();
	if(fd != 1){
	    SpiConfig(fd);
            launch = clock();
	    readspi(fd);
            done = clock();
	    SpiClose(fd);	
	}
        else 
            printf("SPI error \n");

	diff = (double )(done - launch)  /CLOCKS_PER_SEC;
//	printf("Time lapsed %.12f \n", diff);

	return 0;
}


