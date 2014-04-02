#include "interrupts.h"


__ALIGN_BEGIN extern USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;
volatile extern uint32_t ticker, downTicker;

void NMI_Handler(void)       {}
void HardFault_Handler(void) {}
void MemManage_Handler(void) {}
void BusFault_Handler(void)  {}
void UsageFault_Handler(void){}
void SVC_Handler(void)       {}
void DebugMon_Handler(void)  {}
void PendSV_Handler(void)    {}

void OTG_FS_IRQHandler(void)
{
  USBD_OTG_ISR_Handler (&USB_OTG_dev);
}

void OTG_FS_WKUP_IRQHandler(void)
{
  if(USB_OTG_dev.cfg.low_power)
  {
    *(uint32_t *)(0xE000ED10) &= 0xFFFFFFF9 ;
    SystemInit();
    USB_OTG_UngateClock(&USB_OTG_dev);
  }
  EXTI_ClearITPendingBit(EXTI_Line18);
}

void SysTick_Handler(void)
{
	ticker++;
	if (downTicker > 0)
	{
		downTicker--;
	}
}
