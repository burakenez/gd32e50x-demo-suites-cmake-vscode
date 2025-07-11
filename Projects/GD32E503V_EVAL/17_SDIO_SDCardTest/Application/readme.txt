/*!
    \file    readme.txt
    \brief   description of SD card read and write test

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

  This demo is based on the GD32E503V-EVAL-V1.0 board, it shows how to use SDIO to read or
write to SD card. Firstly, all the LEDs are turned on and off for test. If initialization 
of the card is successful, print out the detailed information of the card by USART. Then 
write a block of data to the card and read. If any error occurs, print the error message 
and turn on LED1, LED3 and turn off LED2, LED4. After that, do the lock and unlock operation 
test. Lock the card first and try to erase the data of the card. Then unlock the card and 
erase the card. If any error occurs, print the error message and turn on LED1, LED3 and 
turn off LED2, LED4. Last is the multiple blocks operation test. If no error occurs, turn 
on all the LEDs.

  Uncomment the macro DATA_PRINT to print out the data and display them through HyperTerminal.
Set bus mode(1-bit or 4-bit) and data transfer mode(polling mode or DMA mode) by commenting 
and uncommenting the related statements.
