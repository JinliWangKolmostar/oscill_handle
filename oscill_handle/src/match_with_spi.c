#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/config.h"

static const int SPI_HEAD_LEN = 48;

static int MemoryCompare(const void *src, const void *des, int cmp_len)
{
    int i, j;
    unsigned int *src_32 = (unsigned int *)src;
    unsigned int *des_32 = (unsigned int *)des;

    for(j = 0; j < cmp_len / 4; j++)
    {
        if(*(src_32 + j) != *(des_32 + j))
        {
            return -1;
        }
    }
    int remainder = cmp_len % 4;
    unsigned char *src_8 = (unsigned char *)src;
    unsigned char *des_8 = (unsigned char *)des;
    for(i = 0; i < remainder; i++)
    {
        if((*src_8 + (j-1)*4 + i) != *(des_8 + (j-1)*4 + i))
        {
            return -1;
        }
    }
    return 1;
}

static int SearchFirstMatch(unsigned char *oscill_data_buffer)
{
    int i;
    unsigned char file_contet[SPI_FILE_SIZE];
    for(i = 0; ; i++)
    {
        char des_file_name[FILE_NAME_MAX_LEN];
        sprintf(des_file_name,
                WORK_PATH"data_spi\\data_capture_interval_%d.bin",
                i);
        FILE *fp_des = fopen(des_file_name, "rb");
        if(fp_des == NULL)
        {
            break;
        }
        int get_size = fread(file_contet, 1, SPI_FILE_SIZE, fp_des);
        if(get_size == 0)
        {
            continue;
        }
        fclose(fp_des);

        int cmp_result = MemoryCompare(oscill_data_buffer, file_contet + SPI_HEAD_LEN, get_size - SPI_HEAD_LEN);
        if(cmp_result == 1)
        {
            return i;
        }
    }

    return -1;
}

static int FindSubstring(char *source_file_name, FILE *fp_log, int *cur_spi_file_count)
{
    int first_offset;
    int ret;
    unsigned char file_contet[SPI_FILE_SIZE];
    unsigned char sou_buffer[SPI_FILE_SIZE];
    FILE *fp_sou = fopen(source_file_name, "rb");
    if(fp_sou == NULL)
    {
        return -1;
    }
    fread(sou_buffer, 1, SPI_FILE_SIZE, fp_sou);
    fclose(fp_sou);

    if(*cur_spi_file_count < 0)
    {
        first_offset = SearchFirstMatch(sou_buffer);
        if(first_offset > 0)
        {
            *cur_spi_file_count = first_offset;
            ret = 1;
        }
        else
            return -1;
    }
    else
    {
        int spi_file_num;
        /* there is a bug : if file not be matched, skip fixed interval maybe cause mismatch of all */
        /* but this way have a faser speed to match */
        for(spi_file_num = 0; spi_file_num <= READ_INTERVAL_TIME; spi_file_num++)
        {
            char des_file_name[FILE_NAME_MAX_LEN];
            sprintf(des_file_name,
                    WORK_PATH"data_spi\\data_capture_interval_%d.bin",
                    *cur_spi_file_count + spi_file_num);
            FILE *fp_des = fopen(des_file_name, "rb");
            if(fp_des == NULL)
            {
                return 0;
            }
            int get_size = fread(file_contet, 1, SPI_FILE_SIZE, fp_des);
            fclose(fp_des);

            int cmp_result = MemoryCompare(sou_buffer, file_contet + SPI_HEAD_LEN, get_size - SPI_HEAD_LEN);
            if(cmp_result == 1)
            {
                *cur_spi_file_count += spi_file_num;
                ret = 1;
                break;
            }
            else
            {
                *cur_spi_file_count += READ_INTERVAL_TIME;
                ret = 0;
            }
        }
    }

    return ret;
}

int match_data(int oscill_file_num)
{
    int i;
    int cur_spi_file_count = -1;
    int match_file_count_a = 0;
    char sou_file_name[FILE_NAME_MAX_LEN];
    char log_file_name[FILE_NAME_MAX_LEN] = WORK_PATH"log.txt";
    FILE *fp_log = fopen(log_file_name, "w");
    for(i = 0; i < oscill_file_num; i++)
    {
        sprintf(sou_file_name, WORK_PATH"data_oscillo_a\\oscillo_valid_data_%d.bin", i);
        int ret = FindSubstring(sou_file_name, fp_log, &cur_spi_file_count);
        if(ret == -1)
        {
            break;
        }
        else if(ret == 1)
        {
            char log_string[LOG_CONTENT_MAX_LEN];
            sprintf(log_string, "oscillo_valid_data_%d.bin <-------> data_capture_interval_%d.bin\n",
                                i,
                                cur_spi_file_count);
            fwrite(log_string, 1, strlen(log_string), fp_log);
            match_file_count_a++;
        }
        else
        {
            char log_string[LOG_CONTENT_MAX_LEN];
            sprintf(log_string, "mismatch : oscillo_valid_data_%d.bin\n", i);
            fwrite(log_string, 1, strlen(log_string), fp_log);
        }
    }
    fclose(fp_log);
    printf("-----------------match finished---------------\n");
    return match_file_count_a;
}
