#include "ACS_Protocol.h"

DeviceStruct *DV_Head = NULL;


void ACS_Init()
{

}

int8_t ACS_AddDevice(uint8_t ID, uint8_t Type)
{
	DeviceStruct* DV_Temp1;
	DeviceStruct* DV_Temp2;
	uint8_t i;

	if (DV_Head == NULL)
	{
		DV_Head = (DeviceStruct*)malloc(sizeof(DeviceStruct));

		if(DV_Head == NULL)
			return -1;

		DV_Head->DeviceID = ID;
		DV_Head->DeviceType = Type;

		if( (Type & 0xF0)==0xB0 || (Type & 0xF0)==0xC0 ) //Requester = 0xBX - 0xCX
		{
			DV_Head->AttachDevices = (uint8_t*)malloc(5*sizeof(uint8_t));
			for(i=0; i<5; i++)
				DV_Head->AttachDevices[i] = 0;

		}
		else
			DV_Head->AttachDevices = NULL;

		DV_Head->next = NULL;
		DV_Head->prev = NULL;
	}
	else
	{
		DV_Temp1 = DV_Head;

		while (DV_Temp1->next != NULL)
			DV_Temp1 = (DeviceStruct*)DV_Temp1->next;

		DV_Temp2 = (DeviceStruct*)malloc(sizeof(DeviceStruct));

		if(DV_Temp2 == NULL)
			return -1;

		DV_Temp2->DeviceID = ID;
		DV_Temp2->DeviceType = Type;

		if( (Type & 0xF0)==0xB0 || (Type & 0xF0)==0xC0 ) //Requester = 0xBX - 0xCX
		{
			DV_Temp2->AttachDevices = (uint8_t*)malloc(5*sizeof(uint8_t));
			for(i=0; i<5; i++)
				DV_Temp2->AttachDevices[i] = 0;
		}
		else
			DV_Temp2->AttachDevices = NULL;

		DV_Temp2->next = NULL;
		DV_Temp2->prev = NULL;

		DV_Temp1->next = DV_Temp2;
	}

	return 1;
}

DeviceStruct* ACS_GetDeviceListPointer()
{
	return DV_Head;
}

int8_t ACS_ConnectDevice(CanRxMsg RxMessage)
{
	CanTxMsg TxMessage;

	if(ACS_AddDevice(RxMessage.Data[1], RxMessage.Data[2]) < 0)
		return -1;

	TxMessage.StdId = RxMessage.Data[1];
	TxMessage.Data[0] = CONNECTION_ACK;

	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.DLC = 1;

	CAN_Transmit(CAN1,&TxMessage);

	return 1;
}

int8_t ACS_MergeDevices(uint8_t RequesterID, uint8_t ExecutorID, uint8_t Action)
{




	return 0;

}
