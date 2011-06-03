/*
 * =====================================================================================
 *
 *       Filename:  key.h
 *
 *    Description:  与键盘输入有关的功能的封装
 *
 *        Version:  1.0
 *        Created:  2011/5/10 12:51:58
 *
 * =====================================================================================
 */

#ifndef  key_INC
#define  key_INC

#define KEY_LEFT 4
#define KEY_RIGHT 3
#define KEY_UP 1
#define KEY_DOWN 2
#define KEY_ENTER 99
#define KEY_ESC 23
#define KEY_F1 11
#define KEY_F2 12
#define KEY_F3 13
#define KEY_F4 14

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  keyGetInput
 *  Description:  获得键盘输入的扫描码
 * =====================================================================================
 */
UINT16 keyGetInput();


#endif   /* ----- #ifndef key_INC  ----- */

