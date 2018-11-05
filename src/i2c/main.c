#include<stdio.h>
#include "bbb_I2C.h"

#define NUM 1

void main(void){

        LEP_UINT16  Brate = 400;
        LEP_UINT8   deviceAddress = 0x2A;
        LEP_UINT16  regAddress = 0x0002;
        LEP_UINT16  readDataPtr[NUM];
        LEP_UINT16  writeDataPtr[NUM];
        LEP_UINT16  wordsToRead = NUM;
        LEP_UINT16  numWordsRead;
        LEP_UINT16  status;

        printf("Dev init %d \n",DEV_I2C_MasterInit(0, &Brate));

        DEV_I2C_MasterReadData(0, deviceAddress, regAddress, readDataPtr, wordsToRead, &numWordsRead, &status);
        printf("First read = 0x%X \n", readDataPtr[0]);

        /*comand*/
        writeDataPtr[0] = 0x0000;
        DEV_I2C_MasterWriteData(0, deviceAddress, regAddress, writeDataPtr, wordsToRead, &numWordsRead, &status);

        DEV_I2C_MasterReadData(0, deviceAddress, regAddress, readDataPtr, wordsToRead, &numWordsRead, &status);
        printf("Second read = 0x%X \n", readDataPtr[0]);
}
