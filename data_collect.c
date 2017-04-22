#include <stdio.h>
#include <stdlib.h>
//#include <visa.h>
#include <string.h>
#include <time.h>

#define CLK_4M_32M

static const unsigned int MEMORY_DEPTH = 120000;
static const unsigned int SINGLE_READ_MAX_LEN = 250000;
static const unsigned int RIGOL_HEAD_MAX_LEN = 11;

#if 0
void DataCollect(int collect_num)
{
    ViSession defaultRM, vi;

	char strStarPos[256];
	char strStopPos[256];
	char* strStop = ":STOP\n";
	char* strRun = ":RUN\n";
	char* strSource = "WAV:SOURce D0\n";
	char* strMode = ":WAV:MODE RAW\n";
	char* strGetData = ":WAV:DATA?\n";

	unsigned long strStop_WriteLen;
	unsigned long strSource_WriteLen;
	unsigned long strMode_WriteLen;
	unsigned long strStarPos_WriteLen;
	unsigned long strStopPos_WriteLen;
	unsigned long strGetData_WriteLen;

	unsigned char *buffer = (unsigned char *)malloc((SINGLE_READ_MAX_LEN + RIGOL_HEAD_MAX_LEN) * sizeof(char));
	unsigned long readLen;

	ViRsrc matches = (ViRsrc)malloc(256);
	ViUInt32 nmatches;
	ViFindList list;
	viOpenDefaultRM (&defaultRM);
	viFindRsrc(defaultRM, "USB?*", &list,&nmatches, matches);
	viOpen (defaultRM,matches,VI_NULL,VI_NULL,&vi);

	//set source
	viWrite(vi, (unsigned char *)strSource, strlen(strSource), &strSource_WriteLen);
	//set
	viWrite(vi, (unsigned char *)strMode, strlen(strMode), &strMode_WriteLen);

#ifndef CLK_4M_32M
	int file_num = 0;
    char des_file_name[256];
	while(file_num < collect_num)
	{
	    time_t start_time = time(NULL);
		//stop
		viWrite(vi, (unsigned char *)strStop, strlen(strStop), &strStop_WriteLen);

        sprintf(des_file_name, "/Users/wangjingli/Documents/data/00_test/origin_data/rigol_data_%d.bin", file_num++);
		FILE *fp = fopen(des_file_name, "wb");
		int i;
		for(i = 0; i < 2; i++) {
			sprintf(strStarPos, ":WAV:STAR %u\n", i * SINGLE_READ_MAX_LEN + 400001);
			sprintf(strStopPos, ":WAV:STOP %u\n", (i + 1) * SINGLE_READ_MAX_LEN + 400001);
			//set star position
			viWrite(vi, (unsigned char *)strStarPos, strlen(strStarPos), &strStarPos_WriteLen);
			//set end position
			viWrite(vi, (unsigned char *)strStopPos, strlen(strStopPos), &strStopPos_WriteLen);
			//send get data commend
			viWrite(vi, (unsigned char *)strGetData, strlen(strGetData), &strGetData_WriteLen);
			//read data from ram of oscilloscope
			viRead(vi, buffer, SINGLE_READ_MAX_LEN + RIGOL_HEAD_MAX_LEN, &readLen);

			fwrite(buffer + RIGOL_HEAD_MAX_LEN, 1, readLen - RIGOL_HEAD_MAX_LEN, fp);
		}
		viWrite(vi, (unsigned char *)strRun, strlen(strRun), &strStop_WriteLen);

		fclose(fp);

        while(1)
        {
            time_t end_time = time(NULL);
            if(end_time - start_time >= 3)
                break;
        }
	}
#else
	int file_num = 0;
    char des_file_name[256];
	while(file_num < collect_num)
	{
	    time_t start_time = time(NULL);
		//stop
		viWrite(vi, (unsigned char *)strStop, strlen(strStop), &strStop_WriteLen);

        sprintf(des_file_name, "/Users/wangjingli/Documents/data/00_test/origin_data/rigol_data_%d.bin", file_num++);
		FILE *fp = fopen(des_file_name, "wb");
		int i;
		for(i = 0; i < 18; i++) {
			sprintf(strStarPos, ":WAV:STAR %u\n", i * SINGLE_READ_MAX_LEN + 1);
			sprintf(strStopPos, ":WAV:STOP %u\n", (i + 1) * SINGLE_READ_MAX_LEN + 1);
			//set star position
			viWrite(vi, (unsigned char *)strStarPos, strlen(strStarPos), &strStarPos_WriteLen);
			//set end position
			viWrite(vi, (unsigned char *)strStopPos, strlen(strStopPos), &strStopPos_WriteLen);
			//send get data commend
			viWrite(vi, (unsigned char *)strGetData, strlen(strGetData), &strGetData_WriteLen);
			//read data from ram of oscilloscope
			viRead(vi, buffer, SINGLE_READ_MAX_LEN + RIGOL_HEAD_MAX_LEN, &readLen);

			fwrite(buffer + RIGOL_HEAD_MAX_LEN, 1, readLen - RIGOL_HEAD_MAX_LEN, fp);
		}
		viWrite(vi, (unsigned char *)strRun, strlen(strRun), &strStop_WriteLen);

		fclose(fp);

        while(1)
        {
            time_t end_time = time(NULL);
            if(end_time - start_time >= 6)
                break;
        }
	}
#endif

	viClose (vi);
	viClose (defaultRM);
	free(buffer);
	free(matches);
	printf("-----------------collect complete---------------\n");
}
#endif
