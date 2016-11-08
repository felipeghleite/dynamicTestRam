/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "MKE02Z2.h"
#include "gpio/gpio.h"
#include "uart/uart.h"
#include "commProtocol.h"
#include "rtc/rtc.h"
#include "crc/crc.h"
#include "wdog/wdog.h"


static protocolContextTag protocolContext = { 0 };


#define RAM_START 0x1FFFFC00 + data_size 	// the first address of SRAM under test
#define RAM_SIZE  4096-data_size 			// the size of SRAM under test
#define data_size 128*4 					// the RAM size left for the program

/* Pointer to the first address of SRAM under test */
char* addrPtr = RAM_START;
static int crcPattern;
/* global variable of the CRC result */
static int crcResult;
const int lastCRC;

void decode(void);

int main(void)
{

	//	UART_ConfigType uartConfig;
	//	RTC_ConfigType rtcConfig;
	//	CRC_ConfigType crcConfig;
	//	WDOG_ConfigType wdogConfig;

	//	/* UART Initialization */
	//	uartConfig.u32Baudrate = 115200;
	//	uartConfig.u32SysClkHz = BUS_CLK_HZ;
	//	delay();
	//	UART_Init(UART1, &uartConfig);
	//	delay();
	//	UART_EnableInterrupt(UART1, UART_RxBuffFullInt);
	//	enable_irq(UART1_IRQn);
	//	protocolSend("UART initialized",strlen("UART initialized"));

	_uartInit();

	//	/* CRC Initialization */
	//	crcConfig.bFinalXOR				= 1;
	//	crcConfig.bTransposeReadType	= 0;
	//	crcConfig.bTransposeWriteType	= 0;
	//	crcConfig.bWidth				= 0;
	//	crcConfig.u32PolyData			= 0x8005;
	//	delay();
	//	CRC_Init(&crcConfig);
	//	delay();
	//	protocolSend("CRC engine initialized",strlen("CRC engine initialized"));

	_crcInit();

	/* RTC Initialization */
	//	rtcConfig.bClockPresaler	= 5;
	//	rtcConfig.bClockSource		= 2;
	//	rtcConfig.bFlag				= 0;
	//	rtcConfig.bInterruptEn		= 1;
	//	rtcConfig.bRTCOut			= 0;
	//	rtcConfig.u16ModuloValue	= 1000;
	//	delay();
	//	RTC_Init(&rtcConfig);
	//	delay();
	//	protocolSend("RTC initialized",strlen("RTC initialized"));

	//	_rtcInit();

	/* WatchDog Initialization */
	//	wdogConfig.sBits.bClkSrc 		= 1;
	//	wdogConfig.sBits.bDbgEnable		= 1;
	//	wdogConfig.sBits.bDisable		= 0;
	//	wdogConfig.sBits.bIntEnable		= 0;
	//	wdogConfig.sBits.bPrescaler		= 0;
	//	wdogConfig.sBits.bStopEnable	= 0;
	//	wdogConfig.sBits.bUpdateEnable	= 0;
	//	wdogConfig.sBits.bWaitEnable	= 1;
	//	wdogConfig.sBits.bWinEnable		= 0;
	//	wdogConfig.u16TimeOut 			= 5000;
	//	delay();
	//	WDOG_Init(&wdogConfig);
	//	delay();
	//	protocolSend("Wdog initialized",strlen("Wdog initialized"));

	//	_watchdogInit();

	//	/* GPIO Initialization */
	//	GPIO_Init(GPIOB, GPIO_PTH1_MASK|GPIO_PTH2_MASK|GPIO_PTE7_MASK, GPIO_PinOutput);
	//	protocolSend("GPIO initialized",strlen("GPIO initialized"));

	_gpioInit();

	//	/* RAM Initialization */
	//	fill_sram(addrPtr, RAM_SIZE, 0x55);
	//	protocolSend("SRAM programmed",strlen("SRAM programmed"));

	while(1){
		/* Wait for Interrupt routine:
		 * It stops the processor and waits for the next interrupt
		 * */
		__WFI();

		/* Calls the decode function:
		 * It checks the decode success flag and execute the function received on uart
		 * */
		decode();
	}
	/* Never leave main */
	return 0;
}

void UART1_IRQHandler(void){
	protocolRXHandler(&protocolContext);
}

//int crc16(char *datablock, int size)
//{
//	int i, j;
//	unsigned short byte, crc, mask;
//
//	i = 0;
//	crc = 0xFFFF;
//
//	for(i=0;i<size;i++){
//
//		byte = datablock[i]; // Get next byte.
//		crc = crc ^ byte;
//
//		for (j = 7; j >= 0; j--) { // Do eight times.
//			mask = -(crc & 1);
//			crc = (crc >> 1) ^ (0x8005 & mask);
//		}
//	}
//
//	return ~crc;
//}

int crc(int seed, char* datablock, int size){
	int temp;
	temp = CRC_Cal16(seed, datablock, size);
	return temp;
}


void fill_sram(char *mem, int size, int pattern) // Função para preencher a SRAM
{

	int z;
	for( z=0; z<size; z++) //loop for para alocar valores na memória SRAM
	{
		*mem = pattern;
		mem++;
	}

}

void decode(){
	if(protocolContext.decodeSuccess)
	{
		protocolContext.decodeSuccess = 0;
		if(!strcmp(protocolContext.payload,"FILLSRAMFF")){
			protocolSend("ACK",strlen("ACK"));
			fill_sram(addrPtr, RAM_SIZE, 0xFF);
			protocolSend("OK",strlen("OK"));
			crcPattern = 0xC07B ;
			//			*(int *)&lastCRC = 0x8683;
		}
		if(!strcmp(protocolContext.payload,"FILLSRAMAA")){
			protocolSend("ACK",strlen("ACK"));
			fill_sram(addrPtr, RAM_SIZE, 0xAA);
			protocolSend("OK",strlen("OK"));
			crcPattern = 0xC088 ;
			//			*(int *)&lastCRC = 0x0B80;
		}
		if(!strcmp(protocolContext.payload,"FILLSRAM55")){
			protocolSend("ACK",strlen("ACK"));
			fill_sram(addrPtr, RAM_SIZE, 0x55);
			protocolSend("OK",strlen("OK"));
			crcPattern = 0xC19D ;
			//			*(int *)&lastCRC = 0x1C80;
		}
		if(!strcmp(protocolContext.payload,"FILLSRAM00")){
			protocolSend("ACK",strlen("ACK"));
			fill_sram(addrPtr, RAM_SIZE, 0x00);
			protocolSend("OK",strlen("OK"));
			crcPattern = 0xC16E ;
			//			*(int *)&lastCRC = 0x9183;
		}
		if(!strcmp(protocolContext.payload,"CHECKSRAM")){
			protocolSend("ACK",strlen("ACK"));
			crcResult = crc(0xFFFF, addrPtr, RAM_SIZE);

			if(crcResult != crcPattern){

				checkMemoryPattern();

			}

			protocolSend("OK",strlen("OK"));
			printCRC(crcResult);
		}
		if(!strcmp(protocolContext.payload,"TOGGLELED")){
			protocolSend("ACK",strlen("ACK"));
			toggleLed();
			protocolSend("OK",strlen("OK"));
		}
		if(!strcmp(protocolContext.payload,"ADD1TORAM")){
			protocolSend("ACK",strlen("ACK"));
			*addrPtr += 1;
			protocolSend("OK",strlen("OK"));
		}
		if(!strcmp(protocolContext.payload,"ADD1TOMEM")){
			protocolSend("ACK",strlen("ACK"));
			addrPtr += 1;
			protocolSend("OK",strlen("OK"));
		}
		if(!strcmp(protocolContext.payload,"RESETMEM")){
			protocolSend("ACK",strlen("ACK"));
			addrPtr = RAM_START;
			protocolSend("OK",strlen("OK"));
		}
		if(!strcmp(protocolContext.payload,"RTCDEINIT")){
			protocolSend("ACK",strlen("ACK"));
			_rtcDeInit();
			protocolSend("OK",strlen("OK"));
		}
		if(!strcmp(protocolContext.payload,"RTCINIT")){
			protocolSend("ACK",strlen("ACK"));
			_rtcInit();
			protocolSend("OK",strlen("OK"));
		}
		if(!strcmp(protocolContext.payload,"WDOGDEINIT")){
			protocolSend("ACK",strlen("ACK"));
			_watchdogDeInit();
			protocolSend("OK",strlen("OK"));
		}
		if(!strcmp(protocolContext.payload,"WDOGINIT")){
			protocolSend("ACK",strlen("ACK"));
			_watchdogInit();
			protocolSend("OK",strlen("OK"));
		}

	}
}

void checkMemoryPattern(){
	unsigned short ramPattern;
	unsigned short size;
	unsigned short temp;

	if(crcPattern == 0xC07B){
		ramPattern = 0xFF;
	}
	if(crcPattern == 0xC088){
		ramPattern = 0xAA;
	}
	if(crcPattern == 0xC19D){
		ramPattern = 0x55;
	}
	if(crcPattern == 0xC16E){
		ramPattern = 0x00;
	}
	for(size = 0; size < RAM_SIZE; size++){
		temp = *(addrPtr + size) & 0xFF;
		if(temp != ramPattern){
			printMemoryPosition(addrPtr+size, temp);
		}
	}
}

void delay(void){
	int j;
	for(j=0;j<1000;j++);
}

//void checkRam(int crc){
//	int j;
//	int pattern;
//	char string[20];
//	protocolSend("ERROR!",strlen("ERROR!"));
//	if(crc == 0x8683){
//		pattern = 0xFF;
//	}
//	if(crc == 0x0B80){
//		pattern = 0xAA;
//	}
//	if(crc == 0x1C80){
//		pattern = 0x55;
//	}
//	if(crc == 0x9183){
//		pattern = 0x00;
//	}
//	for(j = 0; j < RAM_SIZE; j++){
//		if(*(addrPtr + j) != pattern){
//			sprintf(string, "ERROR ADDR: %.2x", addrPtr + j);
//			protocolSend(string,strlen(string));
//		}
//	}
//}

void toggleLed(void){

	GPIO_PinToggle(GPIO_PTH1);
	GPIO_PinToggle(GPIO_PTH2);
	GPIO_PinToggle(GPIO_PTE7);

}

void printCRC(int crcResult){

	int index, indexString;
	char crcString[5];
	char string[12];
	char temp;

	indexString = 0;

	for(index = 3; index > -1; index--){
		temp = ((crcResult >> (index*4)) & 0x0F);

		if(temp > 0x09){
			crcString[indexString++] = temp + 0x37;
		}
		else{
			crcString[indexString++] = temp + 0x30;
		}

	}
	crcString[4] = 0;

	strcpy(string,"CRC : ");
	strcat(string,crcString);
	protocolSend(string,strlen(string));

}

void printMemoryPosition(int Address, char value){

	int index, indexString;
	char memoryString[5];
	char string[16];
	char temp;

	indexString = 0;

	for(index = 3; index > -1; index--){
		temp = ((Address >> (index*4)) & 0x0F);

		if(temp > 0x09){
			memoryString[indexString++] = temp + 0x37;
		}
		else{
			memoryString[indexString++] = temp + 0x30;
		}
	}

	memoryString[4] = 0;

	strcpy(string,"Mem : ");
	strcat(string,memoryString);

	indexString = 0;

	for(index = 1; index > -1; index--){
			temp = ((value >> (index*4)) & 0x0F);

			if(temp > 0x09){
				memoryString[indexString++] = temp + 0x37;
			}
			else{
				memoryString[indexString++] = temp + 0x30;
			}
		}
	memoryString[2] = 0;
	strcat(string," (");
	strcat(string,memoryString);
	strcat(string,")");
	protocolSend(string,strlen(string));

}

void RTC_IRQHandler(void){
	RTC_ClrFlags();
	toggleLed();
	GPIO_PinToggle(GPIO_PTH2);
	WDOG_Feed();
}

void _watchdogInit(){
	WDOG_ConfigType wdogConfig;

	wdogConfig.sBits.bClkSrc 		= 1;
	wdogConfig.sBits.bDbgEnable		= 1;
	wdogConfig.sBits.bDisable		= 0;
	wdogConfig.sBits.bIntEnable		= 0;
	wdogConfig.sBits.bPrescaler		= 0;
	wdogConfig.sBits.bStopEnable	= 0;
	wdogConfig.sBits.bUpdateEnable	= 0;
	wdogConfig.sBits.bWaitEnable	= 1;
	wdogConfig.sBits.bWinEnable		= 0;
	wdogConfig.u16TimeOut 			= 5000;
	delay();
	WDOG_Init(&wdogConfig);
	delay();
	protocolSend("Wdog initialized",strlen("Wdog initialized"));
}

void _watchdogDeInit(){
	WDOG_DisableUpdate();
	WDOG_DeInit();
}

void _rtcInit(){
	RTC_ConfigType rtcConfig;

	rtcConfig.bClockPresaler	= 5;
	rtcConfig.bClockSource		= 2;
	rtcConfig.bFlag				= 0;
	rtcConfig.bInterruptEn		= 1;
	rtcConfig.bRTCOut			= 0;
	rtcConfig.u16ModuloValue	= 1000;
	delay();
	RTC_Init(&rtcConfig);
	delay();
	protocolSend("RTC initialized",strlen("RTC initialized"));
}

void _rtcDeInit(){
	RTC_DeInit();
}

void _uartInit(){
	UART_ConfigType uartConfig;

	/* UART Initialization */
	uartConfig.u32Baudrate = 115200;
	uartConfig.u32SysClkHz = BUS_CLK_HZ;
	delay();
	UART_Init(UART1, &uartConfig);
	delay();
	UART_EnableInterrupt(UART1, UART_RxBuffFullInt);
	enable_irq(UART1_IRQn);
	protocolSend("UART initialized",strlen("UART initialized"));
}

void _crcInit(){
	CRC_ConfigType crcConfig;

	/* CRC Initialization */
	crcConfig.bFinalXOR				= 1;
	crcConfig.bTransposeReadType	= 0;
	crcConfig.bTransposeWriteType	= 0;
	crcConfig.bWidth				= 0;
	crcConfig.u32PolyData			= 0x8005;
	delay();
	CRC_Init(&crcConfig);
	delay();
	protocolSend("CRC engine initialized",strlen("CRC engine initialized"));
}

void _gpioInit(){
	/* GPIO Initialization */
	GPIO_Init(GPIOB, GPIO_PTH1_MASK|GPIO_PTH2_MASK|GPIO_PTE7_MASK, GPIO_PinOutput);
	GPIO_PinSet(GPIO_PTH1);
	GPIO_PinSet(GPIO_PTH2);
	GPIO_PinSet(GPIO_PTE7);
	protocolSend("GPIO initialized",strlen("GPIO initialized"));
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
