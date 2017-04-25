#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <visa.h>
#include "../inc/config.h"

#define RIGOL_COMMAND_LEN 128
#define MATCHS_DESCRIPTOR_SIZE 256
#define RIGOL_HEAD_MAX_LEN 11

void DataCollect(int collect_num) {
#if 1
  ViSession defaultRM, vi;

  char strStarPos[RIGOL_COMMAND_LEN];
  char strStopPos[RIGOL_COMMAND_LEN];
  char *strStop = ":STOP\n";
  char *strRun = ":RUN\n";
  char *strSource = "WAV:SOURce D0\n";
  char *strMode = ":WAV:MODE RAW\n";
  char *strGetData = ":WAV:DATA?\n";

  unsigned long command_WriteLen;
  unsigned char *buffer = (unsigned char *)malloc(
      (SINGLE_READ_MAX_LEN + RIGOL_HEAD_MAX_LEN) * sizeof(char));
  unsigned long readLen;

  ViRsrc matches = (ViRsrc)malloc(MATCHS_DESCRIPTOR_SIZE);
  ViUInt32 nmatches;
  ViFindList list;
  viOpenDefaultRM(&defaultRM);
  viFindRsrc(defaultRM, "USB?*", &list, &nmatches, matches);
  viOpen(defaultRM, matches, VI_NULL, VI_NULL, &vi);

  // set source
  viWrite(vi, (unsigned char *)strSource, strlen(strSource),
          &command_WriteLen);
  // set
  viWrite(vi, (unsigned char *)strMode, strlen(strMode), &command_WriteLen);

  int file_num = 0;
  char des_file_name[FILE_NAME_MAX_LEN];
  time_t start_time, end_time;

  while (file_num < collect_num) {
    printf("collecting %d file ...\n", file_num + 1);
    //为了第一次采数能在一秒初开始计时，而不是在一秒中后段才开始
    //因为time返回值以秒为单位，误差最大会在一秒
    if (file_num == 0)
    {
        start_time = time(NULL);
        while (1) {
          end_time = time(NULL);
          if (end_time - start_time >= 1) break;
        }
    }
    start_time = time(NULL);
    // stop
    viWrite(vi, (unsigned char *)strStop, strlen(strStop), &command_WriteLen);

    sprintf(des_file_name, WORK_PATH "origin_data\\rigol_data_%d.bin",
            file_num++);
    FILE *fp = fopen(des_file_name, "wb");
    int i;
    for (i = 0; i < GET_DATA_TIME; i++) {
      sprintf(strStarPos, ":WAV:STAR %u\n",
              i * SINGLE_READ_MAX_LEN + WAVE_START_OFFSET);
      sprintf(strStopPos, ":WAV:STOP %u\n",
              (i + 1) * SINGLE_READ_MAX_LEN + WAVE_START_OFFSET);
      // set star position
      viWrite(vi, (unsigned char *)strStarPos, strlen(strStarPos),
              &command_WriteLen);
      // set end position
      viWrite(vi, (unsigned char *)strStopPos, strlen(strStopPos),
              &command_WriteLen);
      // send get data commend
      viWrite(vi, (unsigned char *)strGetData, strlen(strGetData),
              &command_WriteLen);
      // read data from ram of oscilloscope
      viRead(vi, buffer, SINGLE_READ_MAX_LEN + RIGOL_HEAD_MAX_LEN, &readLen);

      fwrite(buffer + RIGOL_HEAD_MAX_LEN, 1, readLen - RIGOL_HEAD_MAX_LEN, fp);
    }
    viWrite(vi, (unsigned char *)strRun, strlen(strRun), &command_WriteLen);

    fclose(fp);

    while (1) {
      end_time = time(NULL);
      if (end_time - start_time >= READ_INTERVAL_TIME) break;
    }
  }

  viClose(vi);
  viClose(defaultRM);
  free(buffer);
  free(matches);
  printf("-----------------collect complete---------------\n");
#endif
}

