/*!
    \file    main.c
    \brief   ADC0 ADC1 regular parallel mode demo

    \version 2025-02-13, V1.6.0, demo for GD32E50x
*/

/*
    Copyright (c) 2025, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32e50x.h"
#include <stdio.h>
#include "systick.h"
#include "gd32e503z_eval.h"

uint32_t adc_value[2];

void rcu_config(void);
void gpio_config(void);
void dma_config(void);
void timer_config(void);
void adc_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* peripheral clocks configuration */
    rcu_config();
    /* configure systick */
    systick_config();  
    /* GPIO configuration */
    gpio_config();
    /* TIMER configuration */
    timer_config();
    /* DMA configuration */
    dma_config();
    /* ADC configuration */
    adc_config();
    /* configure COM port */
    gd_eval_com_init(EVAL_COM0);

    while(1){
        delay_ms(2000);
        printf(" the data adc_value[0] is 0x%08X \r\n", adc_value[0]);
        printf(" the data adc_value[1] is 0x%08X \r\n", adc_value[1]);
        printf("\r\n");
    }
}

/*!
    \brief      configure the different system clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOC);
    /* enable ADC0 clock */
    rcu_periph_clock_enable(RCU_ADC0);
    /* enable ADC1 clock */
    rcu_periph_clock_enable(RCU_ADC1);
    /* enable DMA0 clock */
    rcu_periph_clock_enable(RCU_DMA0);
    /* enable timer1 clock */
    rcu_periph_clock_enable(RCU_TIMER1);
    /* configure ADC clock */
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6);
}

/*!
    \brief      configure the GPIO peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* configure the GPIO as analog mode */
    gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_MAX, GPIO_PIN_2 | GPIO_PIN_3);
}

/*!
    \brief      configure the DMA peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dma_config(void)
{
    /* ADC_DMA_channel configuration */
    dma_parameter_struct dma_data_parameter;
    
    /* ADC DMA_channel configuration */
    dma_deinit(DMA0, DMA_CH0);
    
    /* initialize DMA data mode */
    dma_data_parameter.periph_addr  = (uint32_t)(&ADC_RDATA(ADC0));
    dma_data_parameter.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    dma_data_parameter.memory_addr  = (uint32_t)(&adc_value);
    dma_data_parameter.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
    dma_data_parameter.periph_width = DMA_PERIPHERAL_WIDTH_32BIT;
    dma_data_parameter.memory_width = DMA_MEMORY_WIDTH_32BIT;  
    dma_data_parameter.direction    = DMA_PERIPHERAL_TO_MEMORY;
    dma_data_parameter.number       = 2;
    dma_data_parameter.priority     = DMA_PRIORITY_HIGH;
    dma_init(DMA0, DMA_CH0, &dma_data_parameter);
    /* enable DMA0 circulation mode */
    dma_circulation_enable(DMA0, DMA_CH0);
  
    /* enable DMA channel */
    dma_channel_enable(DMA0, DMA_CH0);
}

/*!
    \brief      configure the TIMER peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void timer_config(void)
{
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    /* TIMER1 configuration */
    timer_initpara.prescaler         = 8999;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 9999;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER1, &timer_initpara);

    /* CH1 configuration in PWM0 mode */
    timer_ocintpara.ocpolarity  = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_channel_output_config(TIMER1, TIMER_CH_1, &timer_ocintpara);

    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_1, 3999);
    timer_channel_output_mode_config(TIMER1, TIMER_CH_1, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER1, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);
    
    /* TIMER1 primary output enable */
    timer_primary_output_config(TIMER1, ENABLE);
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER1); 
    
    /* enable TIMER1 */
    timer_enable(TIMER1);
}

/*!
    \brief      configure the ADC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_config(void)
{
    /* ADC continous function enable */
    adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE);
    adc_special_function_config(ADC1, ADC_SCAN_MODE, ENABLE);
    adc_special_function_config(ADC1, ADC_CONTINUOUS_MODE, DISABLE);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    adc_data_alignment_config(ADC1, ADC_DATAALIGN_RIGHT);  
    /* ADC mode config */
    adc_mode_config(ADC_DAUL_REGULAL_FOLLOWUP_FAST);
    
    /* ADC channel length config */
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 2);
    adc_channel_length_config(ADC1, ADC_REGULAR_CHANNEL, 2);
    /* ADC regular channel config */
    adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_12, ADC_SAMPLETIME_55POINT5);
    adc_regular_channel_config(ADC0, 1, ADC_CHANNEL_13, ADC_SAMPLETIME_55POINT5);
    adc_regular_channel_config(ADC1, 0, ADC_CHANNEL_13, ADC_SAMPLETIME_55POINT5);
    adc_regular_channel_config(ADC1, 1, ADC_CHANNEL_12, ADC_SAMPLETIME_55POINT5);
    /* ADC trigger config */
    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_EXTTRIG_REGULAR_T1_CH1);
    adc_external_trigger_source_config(ADC1, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
    /* ADC external trigger enable */
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);
    adc_external_trigger_config(ADC1, ADC_REGULAR_CHANNEL, ENABLE);

    /* enable ADC interface */
    adc_enable(ADC0);
    delay_ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);
    /* enable ADC interface */
    adc_enable(ADC1);
    delay_ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC1);

    /* ADC DMA function enable */
    adc_dma_mode_enable(ADC0);
}

#ifdef __GNUC__
/* retarget the C library printf function to the USART, in Eclipse GCC environment */
int __io_putchar(int ch)
{
    usart_data_transmit(EVAL_COM0, (uint8_t) ch);
    while(RESET == usart_flag_get(EVAL_COM0, USART_FLAG_TBE));
    return ch;
}
#else
/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM0, (uint8_t)ch);
    while(RESET == usart_flag_get(EVAL_COM0, USART_FLAG_TBE));

    return ch;
}
#endif /* __GNUC__ */
