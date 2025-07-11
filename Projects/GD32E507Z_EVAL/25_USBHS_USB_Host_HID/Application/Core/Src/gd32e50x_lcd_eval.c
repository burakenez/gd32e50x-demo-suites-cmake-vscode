/*!
    \file    gd32e50x_lcd_eval.c
    \brief   LCD driver functions

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

#include "gd32e50x_lcd_eval.h"
#include "drv_usb_hw.h"
#include <string.h>

#define LCD_X_LEN         ((uint32_t)240U)           /*!< LCD row max length */
#define LCD_Y_LEN         ((uint32_t)320U)           /*!< LCD column max length */

#define LCD_ILI9320       0x8989U
#define LCD_ILI9325       0x9325U

#define ABS(X)  ((X) > 0U ? (X) : -(X))

static font_struct *cur_fonts;

/* set the cursor of LCD */
static void lcd_cursor_set(uint16_t x, uint16_t y);

__IO uint16_t cur_text_color = 0x0000U;
__IO uint16_t cur_back_color = 0xFFFFU;

__IO uint16_t cur_text_direction = CHAR_DIRECTION_HORIZONTAL;
__IO uint16_t device_code;

/*!
    \brief      initializes the LCD of GD EVAL board
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gd_eval_lcd_init(void)
{
    __IO uint32_t lcd_id = 0U;

    exmc_lcd_init();

    usb_mdelay(50U);

    /* Read the LCD ID */
    lcd_id = lcd_register_read(R0);

    /* initialize the LCD */
    lcd_init();
}

/*!
    \brief      LCD peripheral initialize
    \param[in]  none
    \param[out] none
    \retval     none
*/
void exmc_lcd_init(void)
{
    static exmc_norsram_parameter_struct lcd_init_struct;
    static exmc_norsram_timing_parameter_struct lcd_timing_init_struct;

    /* EXMC clock enable */
    rcu_periph_clock_enable(RCU_EXMC);

    /* GPIO clock enable */
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOE);

    /* configure EXMC_D[0~15]*/
    /* PD14(EXMC_D0), PD15(EXMC_D1),PD0(EXMC_D2), PD1(EXMC_D3), PD8(EXMC_D13), PD9(EXMC_D14), PD10(EXMC_D15) */
    gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 | \
              GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15);

    /* PE7(EXMC_D4), PE8(EXMC_D5), PE9(EXMC_D6), PE10(EXMC_D7), PE11(EXMC_D8), PE12(EXMC_D9),
       PE13(EXMC_D10), PE14(EXMC_D11), PE15(EXMC_D12) */
    gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | \
              GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | \
              GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

    /* configure PE2(EXMC_A23) */
    gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);

    /* configure PD4(NOE) and PD5(NWE) */
    gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4 | GPIO_PIN_5);

    /* configure EXMC NE */
    gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);

    lcd_timing_init_struct.asyn_access_mode = EXMC_ACCESS_MODE_A;
    lcd_timing_init_struct.syn_data_latency = EXMC_DATALAT_2_CLK;
    lcd_timing_init_struct.syn_clk_division = EXMC_SYN_CLOCK_RATIO_DISABLE;
    lcd_timing_init_struct.bus_latency = 2U;
    lcd_timing_init_struct.asyn_data_setuptime = 15U;
    lcd_timing_init_struct.asyn_address_holdtime = 3U;
    lcd_timing_init_struct.asyn_address_setuptime = 8U;

    lcd_init_struct.write_mode = EXMC_ASYN_WRITE;
    lcd_init_struct.extended_mode = DISABLE;
    lcd_init_struct.asyn_wait = DISABLE;
    lcd_init_struct.nwait_signal = DISABLE;
    lcd_init_struct.memory_write = ENABLE;
    lcd_init_struct.nwait_config = EXMC_NWAIT_CONFIG_BEFORE;
    lcd_init_struct.wrap_burst_mode = DISABLE;
    lcd_init_struct.nwait_polarity = EXMC_NWAIT_POLARITY_LOW;
    lcd_init_struct.burst_mode = DISABLE;
    lcd_init_struct.databus_width = EXMC_NOR_DATABUS_WIDTH_16B;
    lcd_init_struct.memory_type = EXMC_MEMORY_TYPE_SRAM;
    lcd_init_struct.address_data_mux = DISABLE;
    lcd_init_struct.read_write_timing = &lcd_timing_init_struct;
    lcd_init_struct.write_timing = &lcd_timing_init_struct;

    exmc_norsram_init(&lcd_init_struct);

    exmc_norsram_enable(EXMC_BANK0_NORSRAM_REGION1);
}


/*!
    \brief      write data to the selected LCD register
    \param[in]  register_id: the selected register id
    \param[in]  value: the register value to be written
    \param[out] none
    \retval     none
*/

void lcd_register_write(uint16_t register_id, uint16_t value)
{
    *(__IO uint16_t *)(BANK0_LCD_C) = register_id;
    *(__IO uint16_t *)(BANK0_LCD_D) = value;
}

/*!
    \brief      read the value of LCD register
    \param[in]  register_id: the register id
    \param[out] none
    \retval     the register value
*/
uint16_t lcd_register_read(uint8_t register_id)
{
    uint16_t data;
    *(__IO uint16_t *)(BANK0_LCD_C) = register_id;
    data = *(__IO uint16_t *)(BANK0_LCD_D);
    return  data;
}

/*!
    \brief      write command to LCD register
    \param[in]  value: the register value to be written
    \param[out] none
    \retval     none
*/
void lcd_command_write(uint16_t value)
{
    /* write 16-bit index, then write reg */
    *(__IO uint16_t *)(BANK0_LCD_C) = value;
}

/*!
    \brief      prepare to write to the LCD GRAM register(R22h)
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lcd_gram_write_prepare(void)
{
    *(__IO uint16_t *)(BANK0_LCD_C) = 0x0022U;
}

/*!
    \brief      write RGB code to the LCD GRAM register
    \param[in]  rgb_code: the pixel color in RGB mode (5-6-5)
    \param[out] none
    \retval     none
*/
void lcd_gram_write(uint16_t rgb_code)
{
    /* write 16-bit GRAM register */
    *(__IO uint16_t *)(BANK0_LCD_D) = rgb_code;
}

/*!
    \brief      read data from GRAM
    \param[in]  none
    \param[out] none
    \retval     GRAM value
*/
uint16_t lcd_gram_read(void)
{
    uint16_t data;

    /* write GRAM register (R22h) */
    *(__IO uint16_t *)(BANK0_LCD_C) = 0x0022U;
    /* dummy read (invalid data) */
    *(__IO uint16_t *)(BANK0_LCD_D);

    data = *(__IO uint16_t *)(BANK0_LCD_D);
    return data;
}

/*!
    \brief      initialize the LCD
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lcd_init(void)
{
    __IO uint16_t i;

    /* read the LCD controller device code */
    device_code = lcd_register_read(0x0000U);

    if(0x8989U == device_code) {            // SSD1289
        lcd_register_write(0x0000U, 0x0001U);
        lcd_register_write(0x0003U, 0xA8A4U);
        lcd_register_write(0x000CU, 0x0000U);
        lcd_register_write(0x000DU, 0x080CU);
        lcd_register_write(0x000EU, 0x2B00U);
        lcd_register_write(0x001EU, 0x00B0U);
        lcd_register_write(0x0001U, 0x2B3FU);
        lcd_register_write(0x0002U, 0x0600U);
        lcd_register_write(0x0010U, 0x0000U);
        lcd_register_write(0x0011U, 0x6070U);
        lcd_register_write(0x0005U, 0x0000U);
        lcd_register_write(0x0006U, 0x0000U);
        lcd_register_write(0x0016U, 0xEF1CU);
        lcd_register_write(0x0017U, 0x0003U);
        lcd_register_write(0x0007U, 0x0233U);
        lcd_register_write(0x000BU, 0x0000U);
        lcd_register_write(0x000FU, 0x0000U);
        lcd_register_write(0x0041U, 0x0000U);
        lcd_register_write(0x0042U, 0x0000U);
        lcd_register_write(0x0048U, 0x0000U);
        lcd_register_write(0x0049U, 0x013FU);
        lcd_register_write(0x004AU, 0x0000U);
        lcd_register_write(0x004BU, 0x0000U);
        lcd_register_write(0x0044U, 0xEF00U);
        lcd_register_write(0x0045U, 0x0000U);
        lcd_register_write(0x0046U, 0x013FU);
        lcd_register_write(0x0030U, 0x0707U);
        lcd_register_write(0x0031U, 0x0204U);
        lcd_register_write(0x0032U, 0x0204U);
        lcd_register_write(0x0033U, 0x0502U);
        lcd_register_write(0x0034U, 0x0507U);
        lcd_register_write(0x0035U, 0x0204U);
        lcd_register_write(0x0036U, 0x0204U);
        lcd_register_write(0x0037U, 0x0502U);
        lcd_register_write(0x003AU, 0x0302U);
        lcd_register_write(0x003BU, 0x0302U);
        lcd_register_write(0x0023U, 0x0000U);
        lcd_register_write(0x0024U, 0x0000U);
        lcd_register_write(0x0025U, 0x8000U);
        lcd_register_write(0x004eU, 0U);
        lcd_register_write(0x004fU, 0U);
    } else if((0x9320U == device_code) || (0x9300U == device_code)) {   //ILI9320
        lcd_register_write(0x01U, 0x0100U);        //driver output control
        lcd_register_write(0x02U, 0x0700U);        //lcd driver waveform control
        lcd_register_write(0x03U, 0x1020U);        //entry mode set

        lcd_register_write(0x04U, 0x0000U);        //resizing control
        lcd_register_write(0x08U, 0x0202U);        //display control 2
        lcd_register_write(0x09U, 0x0000U);        //display control 3
        lcd_register_write(0x0aU, 0x0000U);        //frame cycle control
        lcd_register_write(0x0cU, (1U << 0U));     //extern display interface control 1
        lcd_register_write(0x0dU, 0x0000U);        //frame maker position
        lcd_register_write(0x0fU, 0x0000U);        //extern display interface control 2

        for(i = 50000U; i > 0U; i--);
        lcd_register_write(0x07U, 0x0101U);        //display control
        for(i = 50000U; i > 0U; i--);

        lcd_register_write(0x10U, (1U << 12U) | (0U << 8U) | (1U << 7U) | (1U << 6U) | (0U << 4U)); //power control 1
        lcd_register_write(0x11U, 0x0007U);                                    //power control 2
        lcd_register_write(0x12U, (1U << 8U) | (1U << 4U) | (0U << 0U));       //power control 3
        lcd_register_write(0x13U, 0x0b00U);                                    //power control 4
        lcd_register_write(0x29U, 0x0000U);                                    //power control 7
        lcd_register_write(0x2bU, (1U << 14U) | (1U << 4U));
        lcd_register_write(0x50U, 0U);             //set x start
        lcd_register_write(0x51U, 239U);           //set x end
        lcd_register_write(0x52U, 0U);             //set y start
        lcd_register_write(0x53U, 319U);           //set y end

        lcd_register_write(0x60U, 0x2700U);        //driver output control
        lcd_register_write(0x61U, 0x0001U);        //driver output control
        lcd_register_write(0x6aU, 0x0000U);        //vertical srcoll control

        lcd_register_write(0x80U, 0x0000U);        //display position? partial display 1
        lcd_register_write(0x81U, 0x0000U);        //ram address start? partial display 1
        lcd_register_write(0x82U, 0x0000U);        //ram address end-partial display 1
        lcd_register_write(0x83U, 0x0000U);        //display position? partial display 2
        lcd_register_write(0x84U, 0x0000U);        //ram address start? partial display 2
        lcd_register_write(0x85U, 0x0000U);        //ram address end? partial display 2

        lcd_register_write(0x90U, (0U << 7U) | (16U << 0U)); //frame cycle control
        lcd_register_write(0x92U, 0x0000U);        //panel interface control 2
        lcd_register_write(0x93U, 0x0001U);        //panel interface control 3
        lcd_register_write(0x95U, 0x0110U);        //frame cycle control
        lcd_register_write(0x97U, (0U << 8U));
        lcd_register_write(0x98U, 0x0000U);        //frame cycle control
        for(i = 50000U; i > 0U; i--);
        lcd_register_write(0x07U, 0x0173U);
        for(i = 50000U; i > 0U; i--);

    } else {
        return;
    }

    for(i = 50000U; i > 0U; i--);
}

/*!
    \brief      set the text color
    \param[in]  color: LCD color
      \arg        LCD_COLOR_WHITE
      \arg        LCD_COLOR_BLACK
      \arg        LCD_COLOR_GREY
      \arg        LCD_COLOR_BLUE
      \arg        LCD_COLOR_BLUE2
      \arg        LCD_COLOR_RED
      \arg        LCD_COLOR_MAGENTA
      \arg        LCD_COLOR_GREEN
      \arg        LCD_COLOR_CYAN
      \arg        LCD_COLOR_YELLOW
    \param[out] none
    \retval     none
*/
void lcd_text_color_set(__IO uint16_t color)
{
    cur_text_color = color;
}

/*!
    \brief      get the current text color
    \param[in]  none
    \param[out] none
    \retval     LCD color
      \arg        LCD_COLOR_WHITE
      \arg        LCD_COLOR_BLACK
      \arg        LCD_COLOR_GREY
      \arg        LCD_COLOR_BLUE
      \arg        LCD_COLOR_BLUE2
      \arg        LCD_COLOR_RED
      \arg        LCD_COLOR_MAGENTA
      \arg        LCD_COLOR_GREEN
      \arg        LCD_COLOR_CYAN
      \arg        LCD_COLOR_YELLOW
*/
uint16_t lcd_text_color_get(void)
{
    return cur_text_color;
}

/*!
    \brief      set the background color
    \param[in]  color: LCD color
      \arg        LCD_COLOR_WHITE
      \arg        LCD_COLOR_BLACK
      \arg        LCD_COLOR_GREY
      \arg        LCD_COLOR_BLUE
      \arg        LCD_COLOR_BLUE2
      \arg        LCD_COLOR_RED
      \arg        LCD_COLOR_MAGENTA
      \arg        LCD_COLOR_GREEN
      \arg        LCD_COLOR_CYAN
      \arg        LCD_COLOR_YELLOW
    \param[out] none
    \retval     none
*/
void lcd_background_color_set(__IO uint16_t color)
{
    cur_back_color = color;
}

/*!
    \brief      get the current background color
    \param[in]  none
    \param[out] none
    \retval     LCD color
      \arg        LCD_COLOR_WHITE
      \arg        LCD_COLOR_BLACK
      \arg        LCD_COLOR_GREY
      \arg        LCD_COLOR_BLUE
      \arg        LCD_COLOR_BLUE2
      \arg        LCD_COLOR_RED
      \arg        LCD_COLOR_MAGENTA
      \arg        LCD_COLOR_GREEN
      \arg        LCD_COLOR_CYAN
      \arg        LCD_COLOR_YELLOW
*/
uint16_t lcd_background_color_get(void)
{
    return cur_back_color;
}

/*!
    \brief      set the text font
    \param[in]  font: the text font
    \param[out] none
    \retval     none
*/
void lcd_font_set(font_struct *fonts)
{
    cur_fonts = fonts;
}

/*!
    \brief      get the text font
    \param[in]  none
    \param[out] none
    \retval     the text font
*/
font_struct *lcd_font_get(void)
{
    return cur_fonts;
}

/*!
    \brief      set the cursor of LCD
    \param[in]  x: the row-coordinate
    \param[in]  y: the column-coordinate
    \param[out] none
    \retval     none
*/
static void lcd_cursor_set(uint16_t x, uint16_t y)
{
    lcd_register_write(0x004eU, x);
    lcd_register_write(0x004fU, y);

}

/*!
    \brief      clear the LCD screen to the specified color
    \param[in]  color: specified screen color
    \param[out] none
    \retval     none
*/
void lcd_clear(uint16_t color)
{
    uint32_t index = 0U;
    if(0x8989U == device_code) {            // SSD1289
        lcd_cursor_set(0U, 0U);
        /* prepare to write GRAM */
        lcd_gram_write_prepare();
        for(index = 0U; index < LCD_X_LEN * LCD_Y_LEN; index++) {
            *(__IO uint16_t *)(BANK0_LCD_D) = color;
        }
    } else if((0x9320U == device_code) || (0x9300U == device_code)) {   //ILI9320
        lcd_register_write(0x20U, 0U);
        lcd_register_write(0x21U, 0U);
        lcd_command_write(0x22U);
        for(index = 0U; index < LCD_X_LEN * LCD_Y_LEN; index++) {
            *(__IO uint16_t *)(BANK0_LCD_D) = color;
        }
    }
}

/*!
    \brief      set the point according to the specified position and color
    \param[in]  x: the row-coordinate
    \param[in]  y: the column-coordinate
    \param[in]  point: specified color of the point
    \param[out] none
    \retval     none
*/
void lcd_point_set(uint16_t x, uint16_t y, uint16_t point)
{
    if((x > LCD_X_LEN) || (y > LCD_Y_LEN)) {
        return;
    }
    if(0x8989U == device_code) {            // SSD1289
        lcd_cursor_set(x, y);
        lcd_gram_write_prepare();
        lcd_gram_write(point);
    } else if((0x9320U == device_code) || (0x9300U == device_code)) {   //ILI9320
        lcd_register_write(0x20U, x);
        lcd_register_write(0x21U, y);
        lcd_register_write(0x22U, point);
    }
}

/*!
    \brief      get point GRAM according to the specified position
    \param[in]  x: the row-coordinate
    \param[in]  y: the column-coordinate
    \param[out] none
    \retval     GRAM value of point
*/
uint16_t lcd_point_get(uint16_t x, uint16_t y)
{
    uint16_t data;

    if((x > LCD_X_LEN) || (y > LCD_Y_LEN)) {
        return 0U;
    }

    lcd_cursor_set(x, y);
    data = lcd_gram_read();

    return  data;
}

/*!
    \brief      draw a horizontal line on LCD screen
    \param[in]  x: the row-coordinate
    \param[in]  start_y: the start column-coordinate
    \param[in]  end_y: the end column-coordinate
    \param[in]  color: specified color of the point
    \param[in]  width: line width
    \param[out] none
    \retval     none
*/
void lcd_hline_draw(uint16_t x, uint16_t start_y, uint16_t end_y, uint16_t color, uint16_t width)
{
    uint16_t i, y;

    for(i = 0U; i < width; i++) {
        uint16_t sx = x + i;

        for(y = start_y; y < end_y; y++) {
            lcd_point_set(sx, y, color);
        }
    }
}

/*!
    \brief      draw a rectangle according to the specified position and color
    \param[in]  start_x: the start position of row-coordinate
    \param[in]  start_y: the start position of column-coordinate
    \param[in]  end_x: the end position of row-coordinate
    \param[in]  end_y: the end position of column-coordinate
    \param[in]  point: specified color of the point
    \param[out] none
    \retval     none
*/
void lcd_rectangle_draw(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y, uint16_t point)
{
    uint16_t x, y;
    x = start_x;
    y = start_y;
    /* draw four lines */
    for(x = start_x; x < end_x; x++) {
        /* draw a point */
        lcd_point_set(x, y, point);
    }
    for(y = start_y; y < end_y; y++) {
        lcd_point_set(x, y, point);
    }
    for(x = end_x; x > start_x; x--) {
        lcd_point_set(x, y, point);
    }
    for(y = end_y; y > start_y; y--) {
        lcd_point_set(x, y, point);
    }
}

/*!
    \brief      fill the specified color to a rectangle
    \param[in]  start_x: the start position of row-coordinate
    \param[in]  start_y: the start position of column-coordinate
    \param[in]  end_x: the end position of row-coordinate
    \param[in]  end_y: the end position of column-coordinate
    \param[in]  color: specified color
    \param[out] none
    \retval     none
*/
void lcd_rectangle_fill(uint16_t start_x, uint16_t start_y, uint16_t width, uint16_t height)
{
    uint16_t x, y;
    x = start_x;
    y = start_y;

    for(x = start_x; x < start_x + width; x++) {
        for(y = start_y; y < start_y + height; y++) {
            lcd_point_set(x, y, cur_text_color);
        }
    }
}

/*!
    \brief      display a char on LCD screen according to the specified position
    \param[in]  x: the start position of row-coordinate
    \param[in]  y: the start position of column-coordinate
    \param[in]  c: the char
    \param[in]  char_color: the color of char
    \param[in]  c_format: the structure of char format
                  font: CHAR_FONT_8_16 or CHAR_FONT_16_24
                  direction: CHAR_DIRECTION_HORIZONTAL or CHAR_DIRECTION_VERTICAL
                  char_color: the color of char
                  bk_color: the color of background
    \param[out] none
    \retval     none
*/
void lcd_char_display(uint16_t x, uint16_t y, uint8_t c)
{
    uint16_t i = 0U, j = 0U;
    uint8_t temp_char = 0U;

    for(i = 0U; i < cur_fonts->height; i++) {
        temp_char = cur_fonts->table[((c - 0x20U) * 16U) + i];
        if(CHAR_DIRECTION_HORIZONTAL == cur_text_direction) {
            for(j = 0U; j < cur_fonts->width; j++) {
                if(0x01U == ((temp_char >> (7U - j)) & 0x01U)) {
                    /* set point of char */
                    lcd_point_set(x - i, y + j, cur_text_color);
                } else {
                    /* set point of background */
                    lcd_point_set(x - i, y + j, cur_back_color);
                }
            }
        } else {
            for(j = 0U; j < cur_fonts->width; j++) {
                if(0x01U == ((temp_char >> (7U - j)) & 0x01U)) {
                    /* set point of char */
                    lcd_point_set(x + j, y + i, cur_text_color);
                } else {
                    /* set point of background */
                    lcd_point_set(x + j, y + i, cur_back_color);
                }
            }
        }
    }
}

/*!
    \brief      display the vertical character on LCD
    \param[in]  line: the line number
    \param[in]  column: the column number
    \param[in]  ascii: content
    \param[out] none
    \retval     none
*/
void lcd_vertical_char_display(uint16_t line, uint16_t column, uint8_t ascii)
{
    lcd_char_display(line, column, ascii);
}

/*!
    \brief      display the string on LCD
    \param[in]  stringline: display line number
    \param[in]  offset: offset of display
    \param[in]  ptr: content
    \param[out] none
    \retval     none
*/
void lcd_vertical_string_display(uint16_t stringline, uint16_t offset, uint8_t *ptr)
{
    uint16_t i = 0U;
    int len = strlen((const char *)ptr);

    for(i = 0U; i < len; i ++) {
        lcd_char_display(stringline, (offset + 8U * i), *ptr++);
    }
}
