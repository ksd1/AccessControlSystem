#ifndef VIEW_H_
#define VIEW_H_

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_can.h"
#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"
#include "usb_dcd_int.h"


uint8_t Init_View(void);
void Init_USB();
void VCP_put_string(char* string);
void Display_CAN_Rx_Frame(CanRxMsg);



#endif
