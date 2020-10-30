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

#define BH1750_I2C_BUS_NAME         "i2c1"   /* ���������ӵ�I2C�����豸���� */
#define BH1750_ADDR                  0x23    /* �ӻ���ַ */

typedef enum
{
        POWER_OFF_CMD      =        0x00,        //�ϵ磺�޼���״̬
        POWER_ON_CMD       =        0x01,        //ͨ�磺�ȴ�����ָ��
        RESET_REGISTER      =        0x07,        //�������ּĴ������ڶϵ�״̬�²������ã�
        CONT_H_MODE           =        0x10,        //����H�ֱ���ģʽ����11x�ֱ����¿�ʼ����������ʱ��120ms
        CONT_H_MODE2         =        0x11,        //����H�ֱ���ģʽ2����0.51x�ֱ����¿�ʼ����������ʱ��120ms
        CONT_L_MODE            =        0x13,        //����L�ֱ���ģʽ����411�ֱ����¿�ʼ����������ʱ��16ms
        ONCE_H_MODE           =        0x20,        //һ�θ߷ֱ���ģʽ����11x�ֱ����¿�ʼ����������ʱ��120ms���������Զ�����Ϊ�ϵ�ģʽ
        ONCE_H_MODE2         =        0x21,        //һ�θ߷ֱ���ģʽ2����0.51x�ֱ����¿�ʼ����������ʱ��120ms���������Զ�����Ϊ�ϵ�ģʽ
        ONCE_L_MODE            =        0x23        //һ�εͷֱ���ģʽ����411x�ֱ����¿�ʼ����������ʱ��16ms���������Զ�����Ϊ�ϵ�ģʽ
} BH1750_MODE;

typedef struct {
    struct rt_i2c_bus_device *i2c_bus_;     /* I2C�����豸��� */
    rt_uint16_t data_;
}ArtBh1750;

extern ArtBh1750 art_bh1750;


#endif /* APPLICATIONS_ARTBH1750_H_ */
