#include "../inc/config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static const char log_file_name[FILE_NAME_MAX_LEN] = WORK_PATH"log.txt";

int GetWrongNumber(int file_num[], int file_nums)
{
    memset(file_num, 0, sizeof(int) * file_nums);
    int head_len = strlen("oscillo_valid_data_");
    char *stop;
    unsigned char log_buf[LOG_CONTENT_MAX_LEN];
    FILE *fp_log = fopen(log_file_name, "r");
    if(fp_log == NULL)
    {
        printf("log file not exist\n");
        return -1;
    }
    while(fgets((char *)log_buf, LOG_CONTENT_MAX_LEN, fp_log) != NULL)
    {
        int tmp = strtol((char *)(log_buf + head_len), &stop, 10);
        file_num[tmp]++;
    }
    return 0;
}

void LogPerLineParser(FILE *fp_log, int *oscillo_num, int *spi_num)
{
    unsigned char log_buf[LOG_CONTENT_MAX_LEN];
    char *stop;
    char *pos = NULL;
    int osc_head_len = strlen("oscillo_valid_data_");
    int spi_head_len = strlen("<-------> data_capture_interval_handle_");

    while(fgets((char *)log_buf, LOG_CONTENT_MAX_LEN, fp_log) != NULL)
    {
        pos = strstr((char *)log_buf, "<------->");
        if(pos != NULL)
            break;
    }
    if(pos != NULL)
    {
        *oscillo_num = strtol((char *)(log_buf + osc_head_len), &stop, 10);
        *spi_num = strtol(pos + spi_head_len, &stop, 10);
    }
}

int GetFileNumOffset()
{
    int oscillo_num = 0;
    int spi_num = 0;
    int oscillo_pre = 0;
    int spi_pre = 0;
    FILE *fp_log = fopen(log_file_name, "r");
    if(fp_log == NULL)
    {
        printf("log file not exist\n");
        return -1;
    }
    while(1)
    {
        LogPerLineParser(fp_log, &oscillo_num, &spi_num);
        if(spi_num - (READ_INTERVAL_TIME * oscillo_num) ==
           spi_pre - (READ_INTERVAL_TIME * oscillo_pre))
        {
            break;
        }
        oscillo_pre = oscillo_num;
        spi_pre = spi_num;
    }

    fclose(fp_log);
    return spi_num - (READ_INTERVAL_TIME * oscillo_num);
}
