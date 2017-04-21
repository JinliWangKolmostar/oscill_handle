#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LOG_BUF_SIZE 256
static const char log_file_name[64] = "C:\\00_zhenguo_work\\00_test\\log.txt";

void GetWrongNumber(int file_num[], int file_nums)
{
    memset(file_num, 0, sizeof(int) * file_nums);
    int head_len = strlen("C:\\00_zhenguo_work\\00_test\\data_oscillo_c\\oscillo_valid_data_");
    char *stop;
    unsigned char log_buf[LOG_BUF_SIZE];
    FILE *fp_log = fopen(log_file_name, "r");
    if(fp_log == NULL)
    {
        printf("log file not exist\n");
    }
    while(fgets((char *)log_buf, LOG_BUF_SIZE, fp_log) != NULL)
    {
        int tmp = strtol((char *)(log_buf + head_len), &stop, 10);
        file_num[tmp]++;
    }
}

void LogPerLineParser(FILE *fp_log, int *oscillo_num, int *spi_num)
{
    unsigned char log_buf[LOG_BUF_SIZE];
    char *stop;
    char *pos;
    int osc_head_len = strlen("C:\\00_zhenguo_work\\00_test\\data_oscillo_a\\oscillo_valid_data_");
    int spi_head_len = strlen("<-------> C:\\00_zhenguo_work\\00_test\\data_spi\\data_capture_interval_");

    while(fgets((char *)log_buf, LOG_BUF_SIZE, fp_log) != NULL)
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
    }
    while(1)
    {
        LogPerLineParser(fp_log, &oscillo_num, &spi_num);
        if(spi_num - (3 * oscillo_num) == spi_pre - (3 * oscillo_pre))
        {
            break;
        }
        oscillo_pre = oscillo_num;
        spi_pre = spi_num;
    }

    fclose(fp_log);
    return spi_num - (3 * oscillo_num);
}
