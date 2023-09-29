/*
 * @Author       : Alvin
 * @Date         : 2023-09-24 23:36:46
 * @LastEditTime : 2023-09-24 23:37:58
 * @LastEditors  : Alvin
 * @FilePath     : \TMC-EvalSystem\hal\Landungsbruecke_GD32F303VGT6\tmc\usbd_conf.h
 * @Description  : 
 */
/*!
    \file    usbd_conf.h
    \brief   usb device driver basic configuration

    \version 2022-06-18, V2.0.0, demo for GD32F30x
*/

/*
    Copyright (c) 2022, GigaDevice Semiconductor Inc.

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

#ifndef __USBD_CONF_H
#define __USBD_CONF_H

#include "gd32f30x.h"

#define USBD_CFG_MAX_NUM                   1U
#define USBD_ITF_MAX_NUM                   1U

#define CDC_COM_INTERFACE                  0U

/* define if low power mode is enabled; it allows entering the device into DEEP_SLEEP mode
   following USB suspend event and wakes up after the USB wakeup event is received. */
//#define USB_DEVICE_LOW_PWR_MODE_SUPPORT

/* USB feature -- Self Powered */
/* #define USBD_SELF_POWERED */

/* endpoint count used by the CDC ACM device */
#define CDC_CMD_EP                         EP_IN(2U)
#define CDC_IN_EP                          EP_IN(1U)
#define CDC_OUT_EP                         EP_OUT(3U)

/* endpoint0, Rx/Tx buffers address offset */
#define EP0_RX_ADDR                        (0x40U)
#define EP0_TX_ADDR                        (0x80U)

/* CDC data Tx buffer address offset */
#define BULK_TX_ADDR                       (0x140U)

/* CDC data Rx buffer address offset */
#define BULK_RX_ADDR                       (0x100U)

/* CDC command Tx buffer address offset */
#define INT_TX_ADDR                        (0xC0U)

#define CDC_ACM_CMD_PACKET_SIZE            8U
#define CDC_ACM_DATA_PACKET_SIZE           64U

/* endpoint count used by the CDC ACM device */
#define EP_COUNT                           (4U)

#define USB_STRING_COUNT                   4U

/* base address offset of the allocation buffer, used for buffer descriptor table and packet memory */
#define BTABLE_OFFSET                      (0x0000U)

#if 0 // dev board
#define USB_PULLUP                         GPIOA
#define USB_PULLUP_PIN                     GPIO_PIN_8
#define RCC_AHBPeriph_GPIO_PULLUP          RCU_GPIOA
#endif 

#if 1 // dev board
#define USB_PULLUP                         GPIOD
#define USB_PULLUP_PIN                     GPIO_PIN_6
#define RCC_AHBPeriph_GPIO_PULLUP          RCU_GPIOD
#endif /* dev board*/
#endif /* __USBD_CONF_H */