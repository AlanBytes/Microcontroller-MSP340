/*
 *   HW Assignment: CPGM0
 *   EEL-4746 Fall 2025
 *   Alan Bernal
 *   Date: 10/8/2025
 *   This is my first EEL-4746 C Program (fixed version)
 */

// Standard Includes

#include "driverlib.h"
#include <stdint.h>
#include <stdio.h>

//Include file for BCUART function
#include "HAL_UART_4746.h"


// Function Prototypes
void GPIO_init();

//Function object file
extern uint8_t myData(uint8_t cmd, uint8_t *dataValue);


//  Main Function
void main(void){

    //Define Local Variable
    char buffer[100];
    uint8_t dataValue;
    uint16_t i, j;
    uint16_t vCount = 0;
    uint32_t vSum = 0;
    uint8_t vMax = 0;
    uint8_t vAvg = 0;
    uint8_t vMedian = 0;
    uint8_t vMin = 255;
    uint8_t dataArray[256];


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

    sprintf(buffer, "My Seed Number is z\r\n");
    UART_transmitString(buffer);

    sprintf(buffer, "FALL 2025 \r\n");
    UART_transmitString(buffer);

    sprintf(buffer, "Problem 2-A \r\n");
    UART_transmitString(buffer);

    sprintf(buffer, "The results are: \r\n");
    UART_transmitString(buffer);
    //--------------CODE------------------//


    //Get Data from file.
    while (1)
    {

        if (vCount < 256)
        {
            dataArray[vCount] = myData(vCount, &dataValue);
            vSum += dataArray[vCount];

            if (dataArray[vCount] > vMax) vMax = dataArray[vCount];
            if (dataArray[vCount] < vMin) vMin = dataArray[vCount];


            vCount++;
        }
        else
        {
            // array full
            break;
        }
    }


    // Compute average only if we read at least one value
    if (vCount > 0) {
        vAvg = (uint8_t)(vSum / vCount);
    } else {
        vAvg = 0;
    }

    //Sort Values

    if (vCount > 0) {
        // Sort dataArray (use 16-bit indices to avoid overflow when vCount==256)
        for(i = 0; i < (uint16_t)(vCount - 1); i++){
            for(j = i + 1; j < vCount; j++){
                if (dataArray[i] > dataArray[j]) {
                    uint8_t temp = dataArray[i];
                    dataArray[i] = dataArray[j];
                    dataArray[j] = temp;
                }
            }
        }
    }

    if (vCount > 0) {
        if (vCount % 2 == 0) {
            vMedian = (dataArray[vCount/2 - 1] + dataArray[vCount/2]) / 2;
        } else {
            vMedian = dataArray[vCount/2];
        }
    } else {
        vMedian = 0;
    }

    //-----------Writing UART------------------//

    sprintf(buffer, "vCount = %d \r\n", vCount);
    UART_transmitString(buffer);

    sprintf(buffer, "vSum = %d \r\n",  vSum);
    UART_transmitString(buffer);

    sprintf(buffer, "vMax = %d \r\n",  vMax);
    UART_transmitString(buffer);

    sprintf(buffer, "vMin = %d \r\n", vMin);
    UART_transmitString(buffer);

    sprintf(buffer, "vAvg = %d \r\n",  vAvg);
    UART_transmitString(buffer);

    sprintf(buffer, "vMedian = %d \r\n",  vMedian);
    UART_transmitString(buffer);

    //Turn LED ON.. done
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);

    //Spin Loop
    while(1){
        // Nothing here.
    }


}
