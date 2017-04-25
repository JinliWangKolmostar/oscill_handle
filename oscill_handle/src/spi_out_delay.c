//
//  main.c
//  delay_statis
//
//  Created by wangjingli on 2017/4/16.
//  Copyright © 2017年 wangjingli. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include "../oscill_handle/oscill_handle/inc/config.h"
#include "../oscill_handle/oscill_handle/inc/data_format_transform.h"

#define CLK_32M_CYCLE_NS 32
#define CLK_4M_CYCLE_NS 250

int DelayCalcu(unsigned char *buffer, size_t buffer_size) {
  int count = 0;
  int pre_count = 0;

  while (count < buffer_size) {
    if (IsRaisingEdge(buffer + count, CS_BIT) == 1) {
      pre_count = count;
      while (IsFallingEdge(buffer + pre_count, CLK_BIT) == 0) {
        pre_count--;
      }
      if (pre_count <= count) {
        break;
      }
    }
    count++;
  }

  return (count - pre_count);
}

int Is32M(unsigned char *buf) {
  int count = 0;
  int first_raising;

  while (IsRaisingEdge(buf + count, CLK_BIT) != 1) {
    count++;
  }
  first_raising = count++;

  while (IsRaisingEdge(buf + count, CLK_BIT) != 1) {
    count++;
  }

  return (count - first_raising) <= CLK_32M_CYCLE_NS;
}

int WriteStatLog(int *statis_32M, int *statis_4M) {
  int j;
  char log_string[LOG_CONTENT_MAX_LEN];
  FILE *fp_log = fopen(WORK_PATH "origin_data\\log.txt", "wt");
  if (fp_log == NULL) {
    printf("error\n");
    return 0;
  }
  for (j = 0; j < CLK_32M_CYCLE_NS; j++) {
    sprintf(log_string, "delay %d ns : %d\n", j, statis_32M[j]);
    fwrite(log_string, 1, strlen(log_string), fp_log);
  }
  sprintf(log_string, "total : %d\n", statis_32M[CLK_32M_CYCLE_NS]);
  fwrite(log_string, 1, strlen(log_string), fp_log);

  for (j = 0; j < CLK_4M_CYCLE_NS; j++) {
    sprintf(log_string, "delay %d ns : %d\n", j, statis_4M[j]);
    fwrite(log_string, 1, strlen(log_string), fp_log);
  }
  sprintf(log_string, "total : %d\n", statis_4M[CLK_4M_CYCLE_NS]);
  fwrite(log_string, 1, strlen(log_string), fp_log);
  fclose(fp_log);

  return 1;
}

int DelayStatistic(int file_num) {
  int i;
  int statis_32M[CLK_32M_CYCLE_NS + 1] = {0};
  int statis_4M[CLK_4M_CYCLE_NS + 1] = {0};

  for (i = 0; i < file_num; i++) {
    unsigned char buffer[SINGLE_READ_MAX_LEN];
    char file_name[FILE_NAME_MAX_LEN];
    sprintf(file_name, WORK_PATH "origin_data\\rigol_data_%d.bin", i);
    FILE *fp = fopen(file_name, "rb");
    if (fp == NULL) {
      continue;
    }
    size_t buffer_size = fread(buffer, 1, SINGLE_READ_MAX_LEN, fp);
    fclose(fp);
    if (buffer_size < 10) {
      continue;
    }

    int delay_time = DelayCalcu(buffer, buffer_size);
    if (Is32M(buffer) == 1) {
      if (delay_time < CLK_32M_CYCLE_NS) {
        statis_32M[CLK_32M_CYCLE_NS]++;
        statis_32M[delay_time]++;
      }
    } else {
      if (delay_time < CLK_4M_CYCLE_NS) {
        statis_4M[CLK_4M_CYCLE_NS]++;
        statis_4M[delay_time]++;
      }
    }
  }

  WriteStatLog(statis_32M, statis_4M);
  return 0;
}

s
