/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-28     OMEN       the first version
 */

#include "ArtBh1750.h"

ArtBh1750 art_bh1750;

/**
 * @brief        ArtBh1750�����캯������ʼ������
 * @param        ArtBh1750����
 * @retval       NULL
*/
static void ArtBh1750Init(ArtBh1750 *c_this) {
    art_bh1750.i2c_bus_ = RT_NULL;
    art_bh1750.data_ = 0;
}

/**
 * @brief        ��BH1750����һ��ָ��
 * @param        cmd ���� BH1750����ģʽָ���BH1750_MODE��ö�ٶ��壩
 * @retval        �ɹ�����RT_EOK
*/
rt_err_t ArtBh1750SendCmd(BH1750_MODE cmd) {
    struct rt_i2c_msg msgs;

    msgs.addr = BH1750_ADDR;
    msgs.flags = RT_I2C_WR;
    msgs.len = 1;
    msgs.buf = (rt_uint8_t*)&cmd;

    /* ����I2C�豸�ӿڴ������� */
    if (rt_i2c_transfer(art_bh1750.i2c_bus_, &msgs, 1) == 1) {
        return RT_EOK;
    } else {
        return -RT_ERROR;
    }
}

/**
 * @brief        ��BH1750����һ�ι�ǿ����
 * @param        dat ���� �洢����ǿ�ȵĵ�ַ�������ֽ����飩
 * @retval        �ɹ� ���� ����RT_EOK
*/
rt_err_t ArtBh1750ReadDat(rt_uint8_t* dat) {
    struct rt_i2c_msg msgs;

    msgs.addr = BH1750_ADDR;
    msgs.flags = RT_I2C_RD;
    msgs.len = 2;
    msgs.buf = dat;

    /* ����I2C�豸�ӿڴ������� */
    if (rt_i2c_transfer(art_bh1750.i2c_bus_, &msgs, 2) == 2) {
        return RT_EOK;
    } else {
        return -RT_ERROR;
    }
}

/**
 * @brief        ��BH1750�������ֽ�����ת��Ϊ����ǿ��ֵ��0-65535��
 * @param        dat  ���� �洢����ǿ�ȵĵ�ַ�������ֽ����飩
 * @retval        �ɹ� ���� ���ع���ǿ��ֵ
*/
rt_uint16_t ArtBh1750DatToLux(rt_uint8_t* dat) {
    rt_uint16_t lux = 0;
    lux = dat[0];
    lux <<= 8;
    lux += dat[1];
    lux = (int)(lux / 1.2);

    return lux;
}

void art_bh1750_read_entry(void *parameter) {
    rt_uint8_t dat[2] = {0};                //dat[0]�Ǹ��ֽڣ�dat[1]�ǵ��ֽ�

    /* ����I2C�����豸����ȡI2C�����豸��� */
    art_bh1750.i2c_bus_ = (struct rt_i2c_bus_device*)rt_device_find(BH1750_I2C_BUS_NAME);

    if(art_bh1750.i2c_bus_ == RT_NULL) {
        rt_kprintf("can't find %s device!\n", BH1750_I2C_BUS_NAME);
    }
    while(1) {
        /* ������������ģʽ */
        ArtBh1750SendCmd(ONCE_H_MODE);

        /* �ȴ�����ת����� */
        rt_thread_mdelay(150);

        /* ��ȡ���� */
        ArtBh1750ReadDat(dat);
        art_bh1750.data_ = ArtBh1750DatToLux(dat);
        /* ת�����ݲ���ӡ */
        rt_kprintf("current: %5d lux\n", art_bh1750.data_);

        rt_thread_mdelay(1000);
    }
}
int ArtBh1750ReadDataThread(void) {
    rt_thread_t tid;    //�߳̾��

    ArtBh1750Init(&art_bh1750);

    tid = rt_thread_create("bh1750_thread",
                            art_bh1750_read_entry,
                            RT_NULL,
                            512,
                            9,
                            10);
   if(tid != RT_NULL)
   {
        //�̴߳����ɹ��������߳�
        rt_thread_startup(tid);
   }

   return 0;

}
INIT_APP_EXPORT(ArtBh1750ReadDataThread);
/* ������ msh �����б��� */
MSH_CMD_EXPORT(ArtBh1750ReadDataThread, i2c bh1750 example);



