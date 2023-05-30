/*
 * This file is part of the EasyLogger Library.
 *
 * Copyright (c) 2015, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2015-04-28
 */

#include <elog.h>

#include "string.h"
#include "usart.h"
#include "stdio.h"
#include "stdbool.h"

static volatile uint8_t elog_dma_busy_flag = false;
static uint8_t elog_dma_buf[ELOG_LINE_BUF_SIZE] = {0};

/**
 * EasyLogger port initialize
 *
 * @return result
 */
ElogErrCode elog_port_init(void) {
    ElogErrCode result = ELOG_NO_ERR;
    return result;
}

/**
 * EasyLogger port deinitialize
 *
 */
void elog_port_deinit(void) { /* add your code here */
}

/**
 * output log port interface
 *
 * @param log output of log
 * @param size log size
 */
void elog_port_output(const char *log, size_t size) {
    while (elog_dma_busy_flag) {
        // actually, it is not a good way to wait here
        // as it is no os project, we just do it in this way
        HAL_Delay(10);
    }
    memcpy(elog_dma_buf, log, size);
    HAL_UART_Transmit_DMA(&huart6, elog_dma_buf, size);
    elog_dma_busy_flag = true;
}

// it should be called after dma transfer complete
void elog_dma_tx_cplt_callback(void) {
    elog_dma_busy_flag = false;
}

// i dont think it is a good way to disable irq as lock
// if you dont use log at irq, you can remove it
// if you use log at irq, get a rtos
// enable and disable irq here will cause missing dma tx complete interrupt
void elog_port_output_lock(void) {
//    __disable_irq();
}

void elog_port_output_unlock(void) {
//    __enable_irq();
}

/**
 * get current time interface
 *
 * @return current time
 */
const char *elog_port_get_time(void) {
    uint32_t tick = HAL_GetTick();
    static char tick_str[12] = {0};
    sprintf(tick_str, "%lu", tick);
    return tick_str;
}

/**
 * get current process name interface
 *
 * @return current process name
 */
const char *elog_port_get_p_info(void) { return NULL; }

/**
 * get current thread name interface
 *
 * @return current thread name
 */
const char *elog_port_get_t_info(void) { return NULL; }
