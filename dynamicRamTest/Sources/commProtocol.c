/*
 * commProtocol.c
 *
 *  Created on: 07/08/2016
 *      Author: Felipe
 */

#include "uart/uart.h"
#include "commProtocol.h"



void protocolRXHandler(protocolContextTag *protocolContext){

	unsigned char ch;

	ch = UART_GetChar(UART1);
	protocolContext->rcvBuffer[protocolContext->rcvIndex++] = (0xFF & ch);
//	UART_PutChar(UART1, ch);

	if(protocolContext->rcvBuffer[protocolContext->rcvIndex-1] == 0xC5){
		protocolContext->endFlag = 1;
		protocolContext->startFlag = 0;
	}

	if(protocolContext->rcvBuffer[protocolContext->rcvIndex-1] == 0x5C){
		protocolContext->startFlag = 1;
		protocolContext->endFlag = 0;
	}
	if(protocolContext->endFlag){
//
//		protocolContext->txIndex = protocolContext->rcvIndex;
//
//		while(protocolContext->txIndex > 0){
//			UART_PutChar(UART1, protocolContext->rcvBuffer[protocolContext->txIndex--]);
//		}

		if(protocolDecode(protocolContext)){
			protocolContext->decodeSuccess = 0; // failure
		}
		else
			protocolContext->decodeSuccess = 1; // success

		protocolContext->rcvIndex = 0;
	}
}

short protocolDecode(protocolContextTag *protocolContext){
	unsigned int index, startIndex;
	unsigned int size;
	unsigned int checksum = 0;
	memset(protocolContext->payload,0,20);
	for(index = 0; index < protocolContext->rcvIndex; index++){
		if(protocolContext->rcvBuffer[index] == 0x5C){
			startIndex = index;
		}
	}
	protocolContext->payloadSize = protocolContext->rcvBuffer[startIndex+1];
	checksum ^= protocolContext->payloadSize;
	for(index = startIndex+2; index < protocolContext->payloadSize; index++){
		protocolContext->payload[index-2] = protocolContext->rcvBuffer[index];
		checksum ^= protocolContext->rcvBuffer[index];
	}

	protocolContext->payloadChecksum = protocolContext->rcvBuffer[index];
	if(checksum == 0x0A){
		checksum += 1;
	}

	if(checksum == protocolContext->payloadChecksum){
		return 0;
	}
	else
		return 1;

}

short protocolSend(char string[], int size){
	unsigned char txString[size+4];
	int index;
	int checksum = 0;

	txString[0] = 0x5C;
	txString[1] = (char) ((size & 0xFF)+1);
	checksum ^= ((size & 0xFF)+1);
	for(index = 0; index < size; index++){
	 txString[index+2] = string[index];
	 checksum ^= string[index];
	}
	if(checksum == 0x0A){
		txString[index+2] = checksum+1;
	}
	else{
	txString[index+2] = checksum;
	}
	txString[index+3] = 0xC5;

	size += 4;

	for(index = 0; index < size; index++){
		UART_PutChar(UART1, txString[index]);
	}
	UART_PutChar(UART1, 0x0A);

}

