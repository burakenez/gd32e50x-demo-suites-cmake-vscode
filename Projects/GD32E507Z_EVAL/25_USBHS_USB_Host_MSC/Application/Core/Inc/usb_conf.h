/*!
    \file    usb_conf.h
    \brief   general low level driver configuration

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

#ifndef __USB_CONF_H
#define __USB_CONF_H

#include <stdlib.h>
#include "gd32e50x.h"
#include "gd32e507z_eval.h"
#include "gd32e50x_lcd_eval.h"
#include "lcd_log.h"
#include "usb_lcd_conf.h"

#define USBH_KEEP_CFG_DESCRIPTOR           0U

/* USB OTG HS PHY CONFIGURATION */

/* on-chip full-speed USB PHY */
#ifdef USE_USB_FS
    #define OC_FS_PHY
#endif

/* on-chip high-speed USB PHY */
#ifdef USE_USB_HS
    #define OC_HS_PHY
#endif /* USE_USB_HS */

/* USB FIFO size config */

/****************** USB OTG FS CONFIGURATION **********************************/

#define USB_RX_FIFO_SIZE                      512U
#define USB_HTX_NPFIFO_SIZE                   256U
#define USB_HTX_PFIFO_SIZE                    256U

#ifdef OC_FS_PHY
     #define USB_EMBEDDED_FS_PHY_ENABLED
#elif defined(OC_HS_PHY)
     #define USB_EMBEDDED_HS_PHY_ENABLED
#else
     #error "PHY is not selected"
#endif

#define USB_INTERNAL_DMA_ENABLED

#define USB_SOF_OUTPUT                        0U
#define USB_LOW_POWER                         0U /* if you want to test the host suspend and wakeup function, please set 1 */

/* if uncomment it, need jump to USB JP */
//#define VBUS_SENSING_ENABLED

/****************** USB MODE CONFIGURATION ********************************/
#define USE_HOST_MODE
//#define USE_DEVICE_MODE
//#define USE_OTG_MODE

#ifndef OC_FS_PHY
    #ifndef OC_HS_PHY
        #error  "OC_FS_PHY or OC_HS_PHY should be defined!"
    #endif
#endif

#ifndef USE_DEVICE_MODE
    #ifndef USE_HOST_MODE
        #error  "USE_DEVICE_MODE or USE_HOST_MODE should be defined"
    #endif
#endif

#ifndef USE_USB_HS
    #ifndef USE_USB_FS
        #error  "USE_USB_HS or USE_USB_FS should be defined"
    #endif
#endif

/****************** C Compilers dependant keywords ****************************/
/* all variables and data structures during the transaction process should be 4-bytes aligned */

#if defined   (__GNUC__)            /* GNU Compiler */
    #define __ALIGN_END __attribute__ ((aligned (4)))
    #define __ALIGN_BEGIN
#else
    #define __ALIGN_END
    #if defined   (__CC_ARM)        /* ARM Compiler */
        #define __ALIGN_BEGIN __align(4)
    #elif defined (__ICCARM__)      /* IAR Compiler */
        #define __ALIGN_BEGIN
    #elif defined  (__TASKING__)    /* TASKING Compiler */
        #define __ALIGN_BEGIN __align(4) 
    #endif /* __CC_ARM */
#endif /* __GNUC__ */

/* __packed keyword used to decrease the data type alignment to 1-byte */
#if defined   ( __GNUC__ )   /* GNU Compiler */
    #ifndef __packed
        #define __packed __attribute__ ((__packed__))
    #endif
#elif defined   (__TASKING__)  /* TASKING Compiler */
    #define __packed __unaligned
#endif /* __GNUC__ */

#endif /* __USB_CONF_H */
