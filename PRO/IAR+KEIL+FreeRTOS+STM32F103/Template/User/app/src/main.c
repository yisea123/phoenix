/*
*********************************************************************************************************
*
*	模块名称 : 主程序入口
*	文件名称 : main.c
*	版    本 : V1.0
*	说    明 : 跑马灯例子。使用了systick中断实现精确定时，控制LED指示灯闪烁频率。
*	修改记录 :
*		版本号  日期       作者    说明
*		V1.0    2015-08-30 armfly  首发
*
*	Copyright (C), 2015-2016, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"				/* 底层硬件驱动 */

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"V4-001_不一样的跑马灯（软件定时器、状态机）"
#define EXAMPLE_DATE	"2015-08-30"
#define DEMO_VER		"1.0"

uint8_t g_MainStatus = 0;	/* 状态机 */

static void status_0(void);
static void status_1(void);
static void status_2(void);

/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: c程序入口
*	形    参：无
*	返 回 值: 错误代码(无需处理)
*********************************************************************************************************
*/
const char *string = "ABCD!\r\n";

int main(void)
{
	/*
		ST固件库中的启动文件已经执行了 SystemInit() 函数，该函数在 system_stm32f4xx.c 文件，主要功能是
	配置CPU系统的时钟，内部Flash访问时序，配置FSMC用于外部SRAM
	*/
	bsp_Init();		/* 硬件初始化 */
	
	g_MainStatus = 0;	/* 初始状态为状态0 */
	
	comSendBuf(COM1,(uint8_t *)string,strlen(string));
	
	/* 状态机大循环 */
	while (1)
	{
		switch (g_MainStatus)
		{
			case 0:			/* 上电执行一次。LED1闪烁3次，每次间隔1秒。3次后状态机返回。*/
				status_0();	
				g_MainStatus = 1;	/* 转移到状态1 */
			  printf("ABCD!");
				break;
			
			case 1:			/* LED1 - LED4 依次流水显示。每次点亮1个LED。状态持续5秒后返回。 */
				status_1();		
				g_MainStatus = 2;	/* 转移到状态2 */
				break;
			
			case 2:
				status_2();	/* LED1 - LED4 依次流水显示。每次点亮3个LED, 熄灭1个。状态持续5秒后返回。*/
				g_MainStatus = 1;	/* 转移到状态1 */
				break;
		}	
	}
}

/*
*********************************************************************************************************
*	函 数 名: status_0
*	功能说明: 状态0  上电执行一次。LED1闪烁3次，每次间隔1秒。3次后状态机返回。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void status_0(void)
{
	/* 关闭LED */
	bsp_LedOff(1);
	bsp_LedOff(2);
	bsp_LedOff(3);
	bsp_LedOff(4);
	
	/* 点亮 LED1 */
	bsp_LedOn(1);
	
	bsp_StartTimer(0, 3000);		/* 定时器0是3000ms 单次定时器 */		
	bsp_StartAutoTimer(1, 500);		/* 定时器1是500ms 自动重装定时器, 控制LED1按1Hz频率翻转闪烁 */
	while (1)
	{			
		bsp_Idle();		/* CPU空闲时执行的函数，在 bsp.c */
		
		/* 这个地方可以插入其他任务 */		
		
		/* bsp_CheckTimer()检查定时器1时间是否到。函数形参表示软件定时器的ID, 值域0 - 3 */
		if (bsp_CheckTimer(1))		
		{
			bsp_LedToggle(1);		/* 间隔500ms 翻转一次 LED1 */
		}
		
		/* 检查定时器0时间是否到 */
		if (bsp_CheckTimer(0))
		{
			/* 3秒定时到后退出本状态 */
			break;
		}
	}
	
	/* 任务结束时，应该关闭定时器，因为他们会占用后台的资源 */
	//bsp_StopTimer(0);	 单次定时器如果超时到过一次后，可以不必关闭
	bsp_StopTimer(1);
}

/*
*********************************************************************************************************
*	函 数 名: status_1
*	功能说明: 状态1。 LED1 - LED4 依次流水显示。每次点亮1个LED。状态持续5秒后返回。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void status_1(void)
{
	uint8_t led_no = 1;		/* LED指示灯序号 1-4 */
	
	bsp_StartTimer(0, 5000);		/* 定时器0是5000ms 单次定时器 */
	bsp_StartAutoTimer(1, 200);		/* 定时器1是500ms 自动重装定时器, 控制LED1按1Hz频率翻转闪烁 */
	bsp_LedOn(1);
	led_no = 1;
	while (1)
	{			
		bsp_Idle();		/* CPU空闲时执行的函数，在 bsp.c */
		
		/* 这个地方可以插入其他任务 */		
		
		/* 检查定时器0时间是否到 */
		if (bsp_CheckTimer(0))
		{
			break;
		}

		if (bsp_CheckTimer(1))		/* 检查自动定时器2，间隔200ms翻转一次LED1 */
		{
			/* 先关闭所有的LED，然后在打开其中一个 */
			bsp_LedOff(1);
			bsp_LedOff(2);
			bsp_LedOff(3);
			bsp_LedOff(4);
			
			if (++led_no == 5)
			{
				led_no = 1;
			}

			bsp_LedOn(led_no);	/* 点亮其中一个LED */	
		}		
	}
	
	/* 任务结束时，应该关闭定时器，因为他们会占用后台的资源 */
	//bsp_StopTimer(0);	 单次定时器如果超时到过一次后，可以不必关闭
	bsp_StopTimer(1);
}

/*
*********************************************************************************************************
*	函 数 名: status_2
*	功能说明: 状态2.  LED1 - LED4 依次流水显示。每次点亮3个LED, 熄灭1个。状态持续5秒后返回。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void status_2(void)
{
	uint8_t led_no = 1;		/* LED指示灯序号 1-4 */
	
	bsp_StartTimer(0, 5000);		/* 定时器0是5000ms 单次定时器 */
	bsp_StartAutoTimer(1, 200);		/* 定时器1是500ms 自动重装定时器, 控制LED1按1Hz频率翻转闪烁 */
	bsp_LedOn(1);
	led_no = 1;
	while (1)
	{			
		bsp_Idle();		/* CPU空闲时执行的函数，在 bsp.c */
		
		/* 这个地方可以插入其他任务 */		
		
		/* 检查定时器0时间是否到 */
		if (bsp_CheckTimer(0))
		{
			break;
		}

		if (bsp_CheckTimer(1))		/* 检查自动定时器2，间隔200ms翻转一次LED1 */
		{
			/* 先打开所有的LED，然后在关闭其中一个 */
			bsp_LedOn(1);
			bsp_LedOn(2);
			bsp_LedOn(3);
			bsp_LedOn(4);
			
			if (++led_no == 5)
			{
				led_no = 1;
			}

			bsp_LedOff(led_no);	/* 点亮其中一个LED */			
		}		
	}
	
	/* 任务结束时，应该关闭定时器，因为他们会占用后台的资源 */
	//bsp_StopTimer(0);	 单次定时器如果超时过一次后，可以不必执行stop函数
	bsp_StopTimer(1);
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
