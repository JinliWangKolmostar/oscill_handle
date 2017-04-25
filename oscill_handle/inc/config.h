//
//  config.h
//  aa
//
//  Created by wangjingli on 2017/4/23.
//  Copyright © 2017年 wangjingli. All rights reserved.
//

#ifndef CONFIG_H
#define CONFIG_H

#define STR(a) #a

#define DEBUG_PROJECT
#ifndef DEBUG_PROJECT
#define WORK_PATH STR(.\\)
#else
//#define WORK_PATH STR(\\Users\\wangjingli\\Documents\\data\\00_test\\)
#define WORK_PATH STR(C:\\test_data\\)
#endif

#define COLLECT_MODE 3
#if (COLLECT_MODE == 1)
#define GET_DATA_TIME 2
#define READ_INTERVAL_TIME 3
#define SPI_FILE_SIZE 65536
#define WAVE_START_OFFSET 400001
#elif (COLLECT_MODE == 2)
#define GET_DATA_TIME 18
#define READ_INTERVAL_TIME 6
#define SPI_FILE_SIZE 131120
#define WAVE_START_OFFSET 1
#elif (COLLECT_MODE == 3)
#define GET_DATA_TIME 1
#define READ_INTERVAL_TIME 2
#define WAVE_START_OFFSET 1
#define SPI_FILE_SIZE 131120
#endif

#define SINGLE_READ_MAX_LEN  250000
#define LOG_CONTENT_MAX_LEN  256
#define FILE_NAME_MAX_LEN  256

#define CS_BIT  (1 << 1)
#define CLK_BIT (1 << 2)


#endif /* CONFIG_H */
