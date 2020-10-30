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
 * @brief        ArtBh1750对象构造函数，初始化参数
 * @param        ArtBh1750对象
 * @retval       NULL
*/
static void ArtBh1750Init(ArtBh1750 *c_this) {
    art_bh1750.i2c_bus_ = RT_NULL;
    art_bh1750.data_ = 0;
}

/**
 * @brief        向BH1750发送一条指令
 * @param        cmd —— BH1750工作模式指令（在BH1750_MODE中枚举定义）
 * @retval        成功返回RT_EOK
*/
rt_err_t ArtBh1750SendCmd(BH1750_MODE cmd) {
    struct rt_i2c_msg msgs;

    msgs.addr = BH1750_ADDR;
    msgs.flags = RT_I2C_WR;
    msgs.len = 1;
    msgs.buf = (rt_uint8_t*)&cmd;

    /* 调用I2C设备接口传输数据 */
    if (rt_i2c_transfer(art_bh1750.i2c_bus_, &msgs, 1) == 1) {
        return RT_EOK;
    } else {
        return -RT_ERROR;
    }
}

/**
 * @brief        从BH1750接收一次光强数据
 * @param        dat —— 存储光照强度的地址（两个字节数组）
 * @retval        成功 —— 返回RT_EOK
*/
rt_err_t ArtBh1750ReadDat(rt_uint8_t* dat) {
    struct rt_i2c_msg msgs;

    msgs.addr = BH1750_ADDR;
    msgs.flags = RT_I2C_RD;
    msgs.len = 2;
    msgs.buf = dat;

    /* 调用I2C设备接口传输数据 */
    if (rt_i2c_transfer(art_bh1750.i2c_bus_, &msgs, 2) == 2) {
        return RT_EOK;
    } else {
        return -RT_ERROR;
    }
}

/**
 * @brief        将BH1750的两个字节数据转换为光照强度值（0-65535）
 * @param        dat  —— 存储光照强度的地址（两个字节数组）
 * @retval        成功 —— 返回光照强度值
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
    rt_uint8_t dat[2] = {0};                //dat[0]是高字节，dat[1]是低字节

    /* 查找I2C总线设备，获取I2C总线设备句柄 */
    art_bh1750.i2c_bus_ = (struct rt_i2c_bus_device*)rt_device_find(BH1750_I2C_BUS_NAME);

    if(art_bh1750.i2c_bus_ == RT_NULL) {
        rt_kprintf("can't find %s device!\n", BH1750_I2C_BUS_NAME);
    }
    while(1) {
        /* 发送命令设置模式 */
        ArtBh1750SendCmd(ONCE_H_MODE);

        /* 等待数据转换完成 */
        rt_thread_mdelay(150);

        /* 读取数据 */
        ArtBh1750ReadDat(dat);
        art_bh1750.data_ = ArtBh1750DatToLux(dat);
        /* 转换数据并打印 */
        rt_kprintf("current: %5d lux\n", art_bh1750.data_);

        rt_thread_mdelay(1000);
    }
}
int ArtBh1750ReadDataThread(void) {
    rt_thread_t tid;    //线程句柄

    ArtBh1750Init(&art_bh1750);

    tid = rt_thread_create("bh1750_thread",
                            art_bh1750_read_entry,
                            RT_NULL,
                            512,
                            9,
                            10);
   if(tid != RT_NULL)
   {
        //线程创建成功，启动线程
        rt_thread_startup(tid);
   }

   return 0;

}
INIT_APP_EXPORT(ArtBh1750ReadDataThread);
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(ArtBh1750ReadDataThread, i2c bh1750 example);



