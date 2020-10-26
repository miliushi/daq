/*
 * le88def.h
 *
 *  Created on: Apr 26, 2017
 *      Author: prya
 */

#ifndef LE88DEF_H_
#define LE88DEF_H_

#define LE88_BASE_ADDR_0					0xff00
#define LE88_BASE_ADDR_1					0xff10
#define LE88_BASE_ADDR_2					0xff20
#define LE88_BASE_ADDR_3					0xff30
#define LE88_BASE_ADDR_4					0xff40
#define LE88_BASE_ADDR_5					0xff50
#define LE88_BASE_ADDR_6					0xff60
#define LE88_BASE_ADDR_7					0xff70
#define LE88_BASE_ADDR_8					0xff80
#define LE88_BASE_ADDR_9					0xff90
#define LE88_BASE_ADDR_10					0xffa0
#define LE88_BASE_ADDR_11					0xffb0
#define LE88_BASE_ADDR_12					0xffc0
#define LE88_BASE_ADDR_13					0xffd0
#define LE88_BASE_ADDR_14					0xffe0
#define LE88_BASE_ADDR_15					0xfff0

#define LE88_REGISTER_COMMAND				0x00
#define LE88_REGISTER_STATUS				0x02
#define LE88_REGISTER_SYNC					0x04
// reserved									0x06
#define	LE88_REGISTER_DATA					0x08
#define LE88_REGISTER_LAM_LOW				0x0a
#define LE88_REGISTER_LAM_HIGH				0x0c
// reserved									0x0e

#define LE88_STATUS_Q_BIT                   0x0001
#define LE88_STATUS_X_BIT                   0x0002
#define LE88_STATUS_IN1_BIT                 0x0020
#define LE88_STATUS_IN2_BIT                 0x0040
#define LE88_STATUS_SY3_BIT                 0x0080
#define LE88_STATUS_SY4_BIT                 0x0100

#endif /* LE88DEF_H_ */
