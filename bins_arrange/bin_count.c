/*
 *   HW Assignment
 *   EEL-4746 Fall 2025
 *   Alan Bernal
 *   Date: 11/6/2025
 */

#include "driverlib.h"
#include <stdio.h>
#include <stdint.h>
#include <msp430.h>
#include "HAL_UART_4746.h"

#define SEED = 0x42 //ASCII value of b

extern uint8_t myData(uint8_t cmd, uint8_t *dataValue);



void main (void)
{
 char buffer[60];
 uint8_t dataValue;
 uint8_t vCount = 1;
 uint8_t bins0[100], bins1[100], bins2[100], bins3[100], bins4[100];
 uint8_t count0 = 0, count1 = 0, count2 = 0, count3 = 0, count4 = 0;

 //WDT
 WDT_A_hold(WDT_A_BASE);

 //Initialize LED0 and set it low
 GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
 GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

 //Initialize and Configure UART
 UART_initGPIO();
 UART_init();

 //Activate New Port Configurations
 PMM_unlockLPM5();

 sprintf(buffer, "Alan Bernal \r\n");
 UART_transmitString(buffer);

 sprintf(buffer, "FALL 2025 \r\n");
 UART_transmitString(buffer);

 sprintf(buffer, "CHM #3 \r\n");
 UART_transmitString(buffer);

 sprintf(buffer, "My Seed Number is b\r\n");
 UART_transmitString(buffer);

 sprintf(buffer, "N:#,Avg,Var \r\n");
 UART_transmitString(buffer);

//------------------CODE--------------------//
 myData(0x42, &dataValue);

 while (1){
     int x = myData(vCount, &dataValue);

     if (x == 0){
         break;
     }

     //assign value to bin
     if (x >= 1 && x <= 49) bins0[count0++] = x;
     else if (x >= 50 && x <= 99) bins1[count1++] = x;
     else if (x >= 100 && x <= 149) bins2[count2++] = x;
     else if (x >= 150 && x <= 199) bins3[count3++] = x;
     else if (x >= 200 && x <= 255) bins4[count4++] = x;

     vCount++;
 }


int i, j, k;
for (i = 0; i < 5; i++){
    uint8_t count;
    int mean = 0, variance = 0;
    uint8_t *current_bin;

    // select the bin
    switch(i){
        case 0: current_bin = bins0; count = count0; break;
        case 1: current_bin = bins1; count = count1; break;
        case 2: current_bin = bins2; count = count2; break;
        case 3: current_bin = bins3; count = count3; break;
        case 4: current_bin = bins4; count = count4; break;
    }

    if (count > 0){
        // calculate sum
        int sum = 0;
        for(j = 0; j < count; j++){
            sum += current_bin[j];
        }
        mean = sum / count;


        // calculate variance
        int var_sum = 0;
        for(k = 0; k < count; k++){
            int diff = current_bin[k] - mean;
            var_sum += diff * diff;
        }
        variance = var_sum / count;

    }

    sprintf(buffer, "%d: = %d,  %d, %d   \r\n", i, count, mean, variance);
    UART_transmitString(buffer);

}



 //Turn LED ON.. done
 GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);

 //Spin Loop
 while(1){
 // Nothing here.
 }
}
