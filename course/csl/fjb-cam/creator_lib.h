#ifndef _CREATOR_LIB_H
#define _CREATOR_LIB_H

#include <unistd.h>
#include <stdarg.h>

#define ARRAY_LENGTH(x)     	( sizeof(x) / sizeof(x[0]) )
#define OK			1
#define	UM			0
#define TRUE			1
#define FALSE			0

int Delay(int ms);

/* 7 Segment LED functions */
int	_7SEG_put_led(int fd, unsigned which, unsigned long data);
int	_7SEG_Black(int fd);

/* LED Lamp functions */
int 	HUHU_put_led(int fd, unsigned char value);

/* DIP Switch functions */
int 	DIPSW_get_sw (int fd, unsigned char *mask);

/* KEYPAD functions */
int 	KEYPAD_clear_buffer(int fd);
int	KEYPAD_get_key (int fd, unsigned short *key);
int 	KEYPAD_wait_key (int fd, unsigned short *key);
void 	WaitChar(int fd);
unsigned char GetIntNumRange (int fd, unsigned char MinNum, unsigned char MaxNum);

/* LCD functions */
int 	LCD_ClearScreen(int fd);
int 	LCD_put_string(int fd, char *string);
int	LCD_printf (int fd, char *fmt,...);
void	LCD_ErrorMessage(int fd, char *msg);
int     LCD_EnableCursor(int fd, int IsCursorOn);

#endif // _CREATOR_LIB_H
