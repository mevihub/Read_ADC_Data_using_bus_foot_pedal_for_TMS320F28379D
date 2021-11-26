/*
 * main.c
 *
 *  Created on: 26-Nov-2021
 *      Author: Devilal
 */





#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "F28x_Project.h"
#include "F2837xD_device.h"
#include "F2837xD_Examples.h"

#include "device.h"
#include "driverlib.h"

#define EX_ADC_RESOLUTION 12

void ConfigADC(uint32_t ADC_BASE);
void initADC_SOC(void);


uint16_t Adc_Result_1;
float ADCINA0_1A;

void main(void)
{
    Device_init();
    Device_initGPIO();

    Interrupt_initModule();
    Interrupt_initVectorTable();

    ConfigADC(ADCA_BASE);
    initADC_SOC();
    EINT;
    ERTM;

    while(1)
    {
        ADC_forceSOC(ADCA_BASE, ADC_SOC_NUMBER1);

        while(ADC_getInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1) == false)
          {

          }
        ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
        Adc_Result_1 = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER1);

        ADCINA0_1A = Adc_Result_1*(3.3/4096);

    }



}



void ConfigADC(uint32_t ADC_BASE)
{
    EALLOW;

    ADC_setPrescaler(ADCA_BASE, ADC_CLK_DIV_4_0);

#if(EX_ADC_RESOLUTION == 12)
    {
        ADC_setMode(ADC_BASE, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);
    }
#elif(EX_ADC_RESOLUTION == 16)
    {
      ADC_setMode(ADCA_BASE, ADC_RESOLUTION_16BIT, ADC_MODE_DIFFERENTIAL);
    }
#endif
    ADC_setInterruptPulseMode(ADC_BASE, ADC_PULSE_END_OF_CONV);
    ADC_enableConverter(ADC_BASE);
    DEVICE_DELAY_US(1000);
    EDIS;
}




void initADC_SOC(void)
{
#if(EX_ADC_RESOLUTION == 12)
    {
        ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN1, 15);

    }
#elif(EX_ADC_RESOLUTION == 16)
    {
        ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN0,64);
    }
#endif
    ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER1);

    ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);

    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);

}


