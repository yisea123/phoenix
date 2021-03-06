#include "includes.h"

/*
*********************************************************************************************************
*	函 数 名:hal_beep_init(void)
*	功能说明:初始化蜂鸣器控制引脚 
*	形    参:无  
*	返 回 值:无 
*********************************************************************************************************
*/
void hal_beep_init(void)
{
	beep_init();
}

/*
*********************************************************************************************************
*	函 数 名:hal_beep(FunctionalState NewState)
*	功能说明:蜂鸣器蜂鸣或停止蜂鸣
*	形    参:NewState:ENABLE,蜂鸣；DISABLE,停止蜂鸣  
*	返 回 值:无 
*********************************************************************************************************
*/

void hal_beep(FunctionalState NewState)
{
	beep(NewState);
}



