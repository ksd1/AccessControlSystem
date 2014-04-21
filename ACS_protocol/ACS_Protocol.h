/*
 * ACS_Protocol.h
 *
 * Created: 2014-04-18 19:58:35
 *  Author: Krzysztof Antosz
 */ 

#ifndef ACS_PROTOCOL_H_
#define ACS_PROTOCOL_H_


#include "stm32f4xx.h"
#include "stm32f4xx_can.h"
#include <stdlib.h>

typedef struct
{
	uint8_t DeviceID;
	uint8_t DeviceType;
	uint8_t* AttachDevices;
	void* next;
	void* prev;

}DeviceStruct;




#define TIMEOUT 10000
//Device ID
#define MASTER_ID 0x01
#define DEVICE_ID 0x02

//ACS Request

#define CONNECT_DEVICE 0xF1
#define CONNECTION_ACK 0xF3

//Types of Devices
//Requester = 0xBX - 0xCX
#define SWITCH 0xB1
#define AUTHENTICATOR 0xB2

//Executor 0xDX - 0xEX
#define LIGHT 0xD1
#define LOCK 0xD2


void ACS_Init();
int8_t ACS_AddDevice(uint8_t ID, uint8_t Type);
int8_t ACS_ConnectDevice(CanRxMsg RxMessage);
int8_t ACS_MergeDevices(uint8_t RequesterID, uint8_t ExecutorID, uint8_t Action);
//uint8_t ACS_NoConnectionACK_ErrorHandler();;

#endif /* ACS_PROTOCOL_H_ */
