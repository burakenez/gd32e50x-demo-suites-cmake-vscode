/*!
    \file    hid_keyboard_itf.c
    \brief   standard HID keyboard interface driver

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

#include "standard_hid_core.h"

typedef enum
{
    CHAR_A = 1,
    CHAR_B,
    CHAR_C
} key_char;

static void key_config(void);
static uint8_t key_state(void);
static void hid_key_data_send(usb_dev *udev);

hid_fop_handler fop_handler = {
    .hid_itf_config = key_config,
    .hid_itf_data_process = hid_key_data_send
};

/*!
    \brief      configure the keys
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void key_config (void)
{
    /* keys configuration */
    /* configure the wakeup key in EXTI mode to remote wakeup */
    gd_eval_key_init(KEY_A, KEY_MODE_EXTI);

    gd_eval_key_init(KEY_B, KEY_MODE_GPIO);

    gd_eval_key_init(KEY_C, KEY_MODE_GPIO);

    exti_interrupt_flag_clear(KEY_A_EXTI_LINE);
}

/*!
    \brief      get keyboard state
    \param[in]  none
    \param[out] none
    \retval     the char
*/
static uint8_t key_state (void)
{
    /* have pressed tamper key */
    if (!gd_eval_key_state_get(KEY_A)) {
        return CHAR_A;
    }

    /* have pressed wakeup key */
    if (!gd_eval_key_state_get(KEY_B)) {
        return CHAR_B;
    }

    /* have pressed user key */
    if (!gd_eval_key_state_get(KEY_C)) {
        return CHAR_C;
    }

    /* no pressed any key */
    return 0U;
}

/*!
    \brief      send the data of key
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void hid_key_data_send(usb_dev *udev)
{
    standard_hid_handler *hid = (standard_hid_handler *)udev->class_data[USBD_HID_INTERFACE];

    if (hid->prev_transfer_complete) {
        switch (key_state()) {
        case CHAR_A:
            hid->data[2] = 0x04U;
            break;
        case CHAR_B:
            hid->data[2] = 0x05U;
            break;
        case CHAR_C:
            hid->data[2] = 0x06U;
            break;
        default:
            break;
        }

        if (0U != hid->data[2]) {
            hid_report_send(udev, hid->data, HID_IN_PACKET);
        }
    }
}
