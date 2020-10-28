/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-28     OMEN       the first version
 */
#ifndef APPLICATIONS_ARTBH1750_H_
#define APPLICATIONS_ARTBH1750_H_

#include <rtthread.h>
#include <rtdevice.h>

#define BH1750_I2C_BUS_NAME         "i2c1"   /* 传感器连接的I2C总线设备名称 */
#define BH1750_ADDR                  0x23    /* 从机地址 */

typedef enum
{
        POWER_OFF_CMD      =        0x00,        //断电：无激活状态
        POWER_ON_CMD       =        0x01,        //通电：等待测量指令
        RESET_REGISTER      =        0x07,        //重置数字寄存器（在断电状态下不起作用）
        CONT_H_MODE           =        0x10,        //连续H分辨率模式：在11x分辨率下开始测量，测量时间120ms
        CONT_H_MODE2         =        0x11,        //连续H分辨率模式2：在0.51x分辨率下开始测量，测量时间120ms
        CONT_L_MODE            =        0x13,        //连续L分辨率模式：在411分辨率下开始测量，测量时间16ms
        ONCE_H_MODE           =        0x20,        //一次高分辨率模式：在11x分辨率下开始测量，测量时间120ms，测量后自动设置为断电模式
        ONCE_H_MODE2         =        0x21,        //一次高分辨率模式2：在0.51x分辨率下开始测量，测量时间120ms，测量后自动设置为断电模式
        ONCE_L_MODE            =        0x23        //一次低分辨率模式：在411x分辨率下开始测量，测量时间16ms，测量后自动设置为断电模式
} BH1750_MODE;

typedef struct {
    struct rt_i2c_bus_device *i2c_bus_;     /* I2C总线设备句柄 */
    rt_uint16_t data_;
}ArtBh1750;

extern ArtBh1750 art_bh1750;


#endif /* APPLICATIONS_ARTBH1750_H_ */
