#include "ACS_Protocol.h"

DeviceStruct *DV_Head = NULL;


void ACS_Init()
{

}

uint8_t ACS_AddDevice(uint8_t ID, uint8_t Type)
{
	DeviceStruct* DV_Temp1;
		DeviceStruct* DV_Temp2;

		if (DV_Head == NULL)
		{
			DV_Head = (DeviceStruct*)malloc(sizeof(DeviceStruct));
			DV_Head->DeviceID = ID;
			DV_Head->DeviceType = Type;
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
			DV_Temp2->DeviceID = ID;
			DV_Temp2->DeviceType = Type;
			DV_Temp2->AttachDevices = NULL;
			DV_Temp2->next = NULL;
			DV_Temp2->prev = NULL;

			DV_Temp1->next = DV_Temp2;

		}
}
