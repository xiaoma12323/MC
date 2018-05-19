#include "stm32f10x.h"
#include "string.h"
#include "stdio.h"

#define GsmRevSize   200

extern u8 GsmFlag;
extern u16 GsmRevCnt;
extern char GsmRev[GsmRevSize];

void USART1_Init(void);
void USART1_SendChar(u8 ch);
void USART1_SendStr(char* str);
void USART2_Init(void);
void USART2_SendChar(u8 ch);
void USART2_SendStr(char* str);
void GsmRevClean(void);
