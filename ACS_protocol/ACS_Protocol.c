#include "ACS_Protocol.h"

DeviceStruct *DV_Head = NULL;
UserStruct* USR_Head = NULL;
LockStruct* LOCK_Head = NULL;
uint8_t UserIdConuter = 1;

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

		DV_Head->AttachDevicesCounter = 0;
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

		DV_Temp2->AttachDevicesCounter = 0;
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

	DeviceStruct* DV_Temp1 = DV_Head;
	DeviceStruct* DV_Temp2 = DV_Head;
	while((DV_Temp1->DeviceID != RequesterID) && (DV_Temp1 != NULL))
	{
		if(DV_Temp1 == NULL)
		{
			return -1;
		}

		DV_Temp1 = (DeviceStruct*)DV_Temp1->next;
	}

	if(!ACS_IsRequster(DV_Temp1->DeviceType))
		return -2;

	while((DV_Temp2->DeviceID != ExecutorID) && (DV_Temp2 != NULL))
	{
		if(DV_Temp2 == NULL)
		{
			return -1;
		}

		DV_Temp2 = (DeviceStruct*)DV_Temp2->next;
	}

	if(!ACS_IsExecutor(DV_Temp2->DeviceType))
		return -2;


	if(DV_Temp1->AttachDevicesCounter < 5)
	{
		*(DV_Temp1->AttachDevices + DV_Temp1->AttachDevicesCounter) = ExecutorID;
		DV_Temp1->AttachDevicesCounter++;
		return 1;
	}
	else
	{
		return -2;
	}

}

uint8_t ACS_IsRequster(uint8_t DevType)
{
	if( (DevType & 0xF0)==0xB0 || (DevType & 0xF0)==0xC0 )
		return 1;
	else
		return 0;
}
uint8_t ACS_IsExecutor(uint8_t DevType)
{
	if( (DevType & 0xF0)==0xD0 || (DevType & 0xF0)==0xE0 )
		return 1;
	else
		return 0;
}

int8_t ACS_AddUser(char* cName, char* cFormane, uint8_t* cCardID)
{
	UserStruct* DV_Temp1;
	UserStruct* DV_Temp2;
	uint8_t i;

	if (USR_Head == NULL)
	{
		USR_Head = (UserStruct*)malloc(sizeof(UserStruct));

		if(USR_Head == NULL)
			return -1;

		USR_Head->ID = UserIdConuter++;
		strcpy(USR_Head->Name,cName);
		strcpy(USR_Head->Formane, cFormane);
		strcpy(USR_Head->CardID, cCardID);
		USR_Head->next = NULL;
	}
	else
	{
		DV_Temp1 = USR_Head;

		while (DV_Temp1->next != NULL)
			DV_Temp1 = (UserStruct*)DV_Temp1->next;

		DV_Temp2 = (UserStruct*)malloc(sizeof(UserStruct));

		if(DV_Temp2 == NULL)
			return -1;

		DV_Temp2->ID = UserIdConuter++;
		strcpy(DV_Temp2->Name,cName);
		strcpy(DV_Temp2->Formane, cFormane);
		strcpy(DV_Temp2->CardID, cCardID);

		DV_Temp2->next = NULL;

		DV_Temp1->next = DV_Temp2;
	}

	return 1;

}

int8_t ACS_AddLock(uint8_t DevID)
{
	LockStruct* DV_Temp1;
	LockStruct* DV_Temp2;
	uint8_t i;

	if (LOCK_Head == NULL)
	{
		LOCK_Head = (LockStruct*)malloc(sizeof(LockStruct));

		if(LOCK_Head == NULL)
			return -1;

		LOCK_Head->DeviceID = DevID;
		LOCK_Head->AttachUsersCounter = 0;
		LOCK_Head->next = NULL;

		for(i=0; i<10; i++)
			LOCK_Head->AttachUsers[i] = NULL;
	}
	else
	{
		DV_Temp1 = LOCK_Head;

		while (DV_Temp1->next != NULL)
			DV_Temp1 = (LockStruct*)DV_Temp1->next;

		DV_Temp2 = (LockStruct*)malloc(sizeof(LockStruct));

		if(DV_Temp2 == NULL)
			return -1;

		DV_Temp2->DeviceID = DevID;
		DV_Temp2->next = NULL;
		DV_Temp2->AttachUsersCounter = 0;
		for(i=0; i<10; i++)
			DV_Temp2->AttachUsers[i] = NULL;

		DV_Temp1->next = DV_Temp2;
	}

	return 1;
}

uint8_t ACS_IdCmp(uint8_t* CardID1, uint8_t* CardID2)
{
	uint8_t i;

	for(i=0; i<4; i++)
	{
		if( *(CardID1+i) != *(CardID2+i) )
			return -1;
	}
	return 0;
}

uint8_t ACS_GetUserID(uint8_t CardID[4])
{
	UserStruct* DV_Temp1 = USR_Head;

	while(DV_Temp1 != NULL && ACS_IdCmp(DV_Temp1->CardID,CardID) != 0)
		DV_Temp1 = (UserStruct*)DV_Temp1->next;

	if(DV_Temp1 != NULL)
		return DV_Temp1->ID;
	else
		return 0;

}

uint8_t ACS_UsrToLock(uint8_t UsrID, uint8_t LockId)
{
	LockStruct* LOCK_Temp1 = LOCK_Head;
	UserStruct* USR_Temp1 = USR_Head;

	while(LOCK_Temp1 != NULL && LOCK_Temp1->DeviceID != LockId)
		LOCK_Temp1 = LOCK_Temp1->next;

	if(LOCK_Temp1 == NULL)
		return -1;

	while(USR_Temp1 != NULL && USR_Temp1->ID != UsrID)
		USR_Temp1 = USR_Temp1->next;

	if(USR_Temp1 == NULL)
		return -1;

	LOCK_Temp1->AttachUsers[LOCK_Temp1->AttachUsersCounter++] = USR_Temp1;
}

uint8_t ACS_AllowToOpen(uint8_t UsrID, uint8_t LockId)
{
	LockStruct* LOCK_Temp1 = LOCK_Head;
	UserStruct* USR_Temp1 = USR_Head;
	uint8_t i;

	while(LOCK_Temp1 != NULL && LOCK_Temp1->DeviceID != LockId)
		LOCK_Temp1 = LOCK_Temp1->next;

	if(LOCK_Temp1 == NULL)
		return -1;

	for(i=0; i<LOCK_Temp1->AttachUsersCounter; i++)
	{
		if(LOCK_Temp1->AttachUsers[i]->ID == UsrID)
			return 1;
	}

	return -1;
}

