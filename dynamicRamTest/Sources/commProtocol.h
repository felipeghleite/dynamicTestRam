/*
 * commProtocol.h
 *
 *  Created on: 07/08/2016
 *      Author: Felipe
 */

#ifndef SOURCES_COMMPROTOCOL_H_
#define SOURCES_COMMPROTOCOL_H_

#include "uart/uart.h"

typedef struct{
	unsigned char rcvBuffer[20];
	unsigned char rcvIndex;
	unsigned char txBuffer[20];
	unsigned char txIndex;
	unsigned char startFlag;
	unsigned char endFlag;
	unsigned char payloadSize;
	unsigned char payload[24];
	unsigned char payloadChecksum;
	unsigned char decodeSuccess;
	unsigned char junkBuffer[6];
}protocolContextTag;

void protocolRXHandler(protocolContextTag *protocolContext);

short protocolDecode(protocolContextTag *protocolContext);

short protocolSend(char string[], int size);




#endif /* SOURCES_COMMPROTOCOL_H_ */
