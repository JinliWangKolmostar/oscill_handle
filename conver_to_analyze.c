#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LOG_BUF_SIZE 256
static const char log_file_name[64] = "C:\\test_data\\log.txt";

void GetWrongNumber(int file_num[], int file_nums)
{
    memset(file_num, 0, sizeof(int) * file_nums);
    int head_len = strlen("C:\\test_data\\data_oscillo_c\\oscillo_valid_data_");
    char *stop;
    unsigned char log_buf[LOG_BUF_SIZE];
    FILE *fp_log = fopen(log_file_name, "r");
    while(fgets((char *)log_buf, LOG_BUF_SIZE, fp_log) != NULL)
    {
        int tmp = strtol((char *)(log_buf + head_len), &stop, 10);
        file_num[tmp]++;
    }
}

int GetFileNumOffset()
{
    int oscillo_num = 0;
    int spi_num = 0;
    unsigned char log_buf[LOG_BUF_SIZE];
    char *stop;
    char *pos;
    int osc_head_len = strlen("C:\\test_data\\data_oscillo_a\\oscillo_valid_data_");
    int spi_head_len = strlen("<-------> C:\\test_data\\data_spi\\data_capture_interval_");
    FILE *fp_log = fopen(log_file_name, "r");
    //FILE *fp_log1 = fopen("C:\\test_data\\log1.txt", "w");
    while(fgets((char *)log_buf, LOG_BUF_SIZE, fp_log) != NULL)
    {
        pos = strstr((char *)log_buf, "<------->");
        if(pos != NULL)
            break;
    }
    if(pos != NULL)
    {
        oscillo_num = strtol((char *)(log_buf + osc_head_len), &stop, 10);
        spi_num = strtol(pos + spi_head_len, &stop, 10);
    }

    fclose(fp_log);
    //fclose(fp_log1);
    return spi_num - (3 * oscillo_num);
}
