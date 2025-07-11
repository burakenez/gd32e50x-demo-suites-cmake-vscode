/*!
    \file    main.c
    \brief   NAND flash read and write demo

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
#include "gd32e503z_eval.h"
#include "exmc_nandflash.h"
#include "systick.h"

#define BUFFER_SIZE                 (0x200U)
#define NAND_GD_MAKERID             (0xC8U)
#define NAND_GD_DEVICEID            (0xF1U)

nand_id_struct nand_id;
uint8_t txbuffer[BUFFER_SIZE], rxbuffer[BUFFER_SIZE];
__IO uint32_t writereadstatus = 0, status= 0;
uint32_t j = 0;
uint32_t k = 0;
uint32_t writereadaddr;
uint16_t zone, block, page, pageoffset;

/* initialize test status led */
static void test_status_led_init(void);
/* flash the LED for test */
static void led_flash(uint8_t times);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* systick configuration */
    systick_config();
    /* configure the led GPIO */
    test_status_led_init();
    /* flash the LED for test */
    led_flash(1);

    /* configure the USART */
    gd_eval_com_init(EVAL_COM0);
    /* configure the EXMC access mode */
    exmc_nandflash_init();
    /* read NAND ID */
    nand_read_id(&nand_id);

    printf("read NAND ID");
    /* print NAND ID */
    printf("\r\nNand flash ID:0x%X 0x%X 0x%X 0x%X\r\n",nand_id.maker_id,nand_id.device_id,
                                               nand_id.third_id,nand_id.fourth_id);

    if((NAND_GD_MAKERID == nand_id.maker_id) && (NAND_GD_DEVICEID == nand_id.device_id)){
        /* set the read and write the address */
        zone = 0;
        block = 10;
        page = 0;
        pageoffset = 1100;
        writereadaddr = ((zone * NAND_ZONE_SIZE + block) * NAND_BLOCK_SIZE + page) 
                        * NAND_PAGE_SIZE + pageoffset;

        /* whether address cross-border */
        if((writereadaddr + BUFFER_SIZE ) > NAND_MAX_ADDRESS){
            printf("\r\naddress cross-border");

            /* failure, light on LED3 */
            gd_eval_led_on(LED3);
            while(1);
        }

        /* fill writebuffer with 0x00.. */
        fill_buffer_nand(txbuffer, BUFFER_SIZE , 0x00);

        /* write data to nand flash */
        status = nand_write(writereadaddr, txbuffer, BUFFER_SIZE);
        if(NAND_OK == status){
            printf("\r\nwrite data successfully!");
        }else{
            printf("\r\nwrite data failure!");

            /* failure, light on LED3 */
            gd_eval_led_on(LED3);
            while(1);
        }

        /* read data from nand flash */
        status = nand_read(writereadaddr, rxbuffer, BUFFER_SIZE);
        if(NAND_OK == status){
            printf("\r\nread data successfully!");
        }else{
            printf("\r\nread data failure!\n\r");

            /* failure, light on LED3 */
            gd_eval_led_on(LED3);
            while(1);
        }

        /* read and write data comparison for equality */
        writereadstatus = 0;
        for(j = 0; j < BUFFER_SIZE; j++){
            if(txbuffer[j] != rxbuffer[j]){
                writereadstatus++;
                break;
            }
        }

        printf("\r\nthe result to access the nand flash:");
        if (writereadstatus == 0){ 
            printf("\r\naccess NAND flash successfully!");

            gd_eval_led_on(LED2);
        }else{ 
            printf("\r\naccess NAND flash failure!");
            
            /* failure, light on LED3 */
            gd_eval_led_on(LED3);
            while(1);
        }
        printf("\r\nprintf data to be read:\r\n");
        for(k = 0; k < BUFFER_SIZE; k ++){
            printf("0x%02X ",rxbuffer[k]);
        }
    }else{
        printf("\n\rread NAND ID failure!\n\r");
        
        /* failure, light on LED3 */
        gd_eval_led_on(LED3);
        while(1);
    }

    while (1);
}

/*!
    \brief      initialize test status led
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void test_status_led_init(void)
{
    /* initialize the leds */
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);
    gd_eval_led_init(LED3);
    gd_eval_led_init(LED4);

    /* close all of leds */
    gd_eval_led_off(LED1);
    gd_eval_led_off(LED2);
    gd_eval_led_off(LED3);
    gd_eval_led_off(LED4);
}

/*!
    \brief      flash the LED for test
    \param[in]  times: times to flash the LEDs
    \param[out] none
    \retval     none
*/
static void led_flash(uint8_t times)
{
    uint8_t i;
    for (i = 0;i < times;i++){
        /* delay 500 ms */
        delay_ms(500);
        /* turn on the LEDs */
        gd_eval_led_on(LED1);
        gd_eval_led_on(LED2);
        gd_eval_led_on(LED3);
        gd_eval_led_on(LED4);

        /* delay 500 ms */
        delay_ms(500);
        /* turn off the LEDs */
        gd_eval_led_off(LED1);
        gd_eval_led_off(LED2);
        gd_eval_led_off(LED3);
        gd_eval_led_off(LED4);
    }
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
