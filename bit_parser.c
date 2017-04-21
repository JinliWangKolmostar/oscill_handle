#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shift_bit.h"
#include "bit_parser.h"
#include "conver_to_analyze.h"

#define BUF_SIZE 66000

static const char mistake_file_name[64] = "C:\\00_zhenguo_work\\00_test\\mistake_log.txt";

int analyze_and_correct(int file_num)
{
    int i;
    int correct_number = 0;
    int wrong_number = 0;
    int shift_suc = 0;
    int *wrong_file_nums = malloc(sizeof(int) * file_num);
    unsigned char des_buf[BUF_SIZE];
    unsigned char sou_buf[BUF_SIZE];
    t_iq_count iq_count;

    char sou_file_name[128];
    char des_file_name[128];
    GetWrongNumber(wrong_file_nums, file_num);
    int file_num_offset = GetFileNumOffset();

    char log_char[256];
    FILE *wrong_fp = fopen(mistake_file_name, "wt");

    for(i = 0; i <= file_num; i++)
    {
        if(wrong_file_nums[i] == 0)
        {
            sprintf(sou_file_name,
                    "C:\\00_zhenguo_work\\00_test\\data_oscillo_a\\oscillo_valid_data_%d.bin",
                    i);
            sprintf(des_file_name,
                    "C:\\00_zhenguo_work\\00_test\\data_spi\\data_capture_interval_%d.bin",
                    i * 3 + file_num_offset);


            FILE *sou_fp = fopen(sou_file_name, "rb");
            FILE *des_fp = fopen(des_file_name, "rb");
            if(sou_fp == NULL || des_fp == NULL)
            {
                printf("file not find\n");
                break;
            }

            int sou_size = fread(sou_buf, 1, BUF_SIZE, sou_fp);
            int buf_size = fread(des_buf, 1, BUF_SIZE, des_fp);
            if(sou_size < buf_size)
            {
                continue;
            }
            memset(&iq_count, 0, sizeof(t_iq_count));
            CompareBitDiff(sou_buf, des_buf, buf_size, &iq_count);
            fclose(sou_fp);
            fclose(des_fp);

            if(AllChannelsMatched(&iq_count, buf_size) == 1)
            {
                correct_number++;
            }
            else
            {
                wrong_number++;
                memset(log_char, 0, 256);
                sprintf(log_char, "-----------------%s data error-----------------\n\n",
                        sou_file_name + strlen("C:\\00_zhenguo_work\\00_test\\data_oscillo_a\\"));
                fwrite(log_char, 1, strlen(log_char), wrong_fp);
                memset(log_char, 0, 256);
                sprintf(log_char, "-----------------%s matched-----------------\n",
                       des_file_name + strlen("C:\\00_zhenguo_work\\00_test\\data_spi\\"));
                fwrite(log_char, 1, strlen(log_char), wrong_fp);
                memset(log_char, 0, 256);
                sprintf(log_char, "��·IQƥ������: I0:%d   I1:%d    Q0:%d    Q1:%d\n", iq_count.i0_count,
                                                                            iq_count.i1_count,
                                                                            iq_count.q0_count,
                                                                            iq_count.q1_count);
                fwrite(log_char, 1, strlen(log_char), wrong_fp);
                memset(log_char, 0, 256);
                sprintf(log_char, "----------------------------try to correct----------------------------\n");
                fwrite(log_char, 1, strlen(log_char), wrong_fp);
                unsigned char tmp[BUF_SIZE];
                int i;
                for(i = 0; i < BUF_SIZE; i++)
                {
                    tmp[i] = des_buf[i];
                }
                if(iq_count.i0_count < 2 * buf_size)
                {
                    memset(log_char, 0, 256);
                    sprintf(log_char, "I0 ����1λ\n");
                    fwrite(log_char, 1, strlen(log_char), wrong_fp);
                    tBit_shift shift_i0 = {1, {1 ,0, 0, 0}, 1};
                    shift_bit(tmp, buf_size, &shift_i0);
                }
                if(iq_count.i1_count < 2 * buf_size)
                {
                    memset(log_char, 0, 256);
                    sprintf(log_char, "I1 ����1λ\n");
                    fwrite(log_char, 1, strlen(log_char), wrong_fp);
                    tBit_shift shift_i0 = {1, {0 ,1, 0, 0}, 1};
                    shift_bit(tmp, buf_size, &shift_i0);
                }
                if(iq_count.q0_count < 2 * buf_size)
                {
                    memset(log_char, 0, 256);
                    sprintf(log_char, "Q0 ����1λ\n");
                    fwrite(log_char, 1, strlen(log_char), wrong_fp);
                    tBit_shift shift_i0 = {1, {0 ,0, 1, 0}, 1};
                    shift_bit(tmp, buf_size, &shift_i0);
                }
                if(iq_count.q1_count < 2 * buf_size)
                {
                    memset(log_char, 0, 256);
                    sprintf(log_char, "Q1 ����1λ\n");
                    fwrite(log_char, 1, strlen(log_char), wrong_fp);
                    tBit_shift shift_i0 = {1, {0 ,0, 0, 1}, 1};
                    shift_bit(tmp, buf_size, &shift_i0);
                }
                t_iq_count iq_left_count;
                memset(&iq_left_count, 0, sizeof(t_iq_count));
                CompareBitDiff(sou_buf, tmp, buf_size, &iq_left_count);
                memset(log_char, 0, 256);
                sprintf(log_char, "��·IQƥ������: I0:%d   I1:%d    Q0:%d    Q1:%d\n", iq_left_count.i0_count,
                                                                            iq_left_count.i1_count,
                                                                            iq_left_count.q0_count,
                                                                            iq_left_count.q1_count);
                fwrite(log_char, 1, strlen(log_char), wrong_fp);
                if(AllChannelsMatched(&iq_left_count, buf_size) == 1)
                {
                    shift_suc++;
                    char file_name[128];
                    strcpy(file_name, sou_file_name);
                    strcat(file_name, ".left");
                    FILE *cor_fp = fopen(file_name, "wb");
                    fwrite(tmp, 1, buf_size, cor_fp);
                    fclose(cor_fp);
                }
                else
                {
                    for(i = 0; i < BUF_SIZE; i++)
                    {
                        tmp[i] = des_buf[i];
                    }
                    if(iq_count.i0_count < 2 * buf_size)
                    {
                        memset(log_char, 0, 256);
                        sprintf(log_char, "I0 ����1λ\n");
                        fwrite(log_char, 1, strlen(log_char), wrong_fp);
                        tBit_shift shift_i0 = {0, {1 ,0, 0, 0}, 1};
                        shift_bit(tmp, buf_size, &shift_i0);
                    }
                    if(iq_count.i1_count < 2 * buf_size)
                    {
                        memset(log_char, 0, 256);
                        sprintf(log_char, "I1 ����1λ\n");
                        fwrite(log_char, 1, strlen(log_char), wrong_fp);
                        tBit_shift shift_i1 = {0, {0 ,1, 0, 0}, 1};
                        shift_bit(tmp, buf_size, &shift_i1);
                    }
                    if(iq_count.q0_count < 2 * buf_size)
                    {
                        memset(log_char, 0, 256);
                        sprintf(log_char, "Q0 ����1λ\n");
                        fwrite(log_char, 1, strlen(log_char), wrong_fp);
                        tBit_shift shift_q0 = {0, {0 ,0, 1, 0}, 1};
                        shift_bit(tmp, buf_size, &shift_q0);
                    }
                    if(iq_count.q1_count < 2 * buf_size)
                    {
                        memset(log_char, 0, 256);
                        sprintf(log_char, "Q1 ����1λ\n");
                        fwrite(log_char, 1, strlen(log_char), wrong_fp);
                        tBit_shift shift_q1 = {0, {0 ,0, 0, 1}, 1};
                        shift_bit(tmp, buf_size, &shift_q1);
                    }
                    t_iq_count iq_right_count;
                    memset(&iq_right_count, 0, sizeof(t_iq_count));
                    CompareBitDiff(sou_buf, tmp, buf_size, &iq_right_count);
                    memset(log_char, 0, 256);
                    sprintf(log_char, "��·IQƥ������: I0:%d   I1:%d    Q0:%d    Q1:%d\n", iq_right_count.i0_count,
                                                                                iq_right_count.i1_count,
                                                                                iq_right_count.q0_count,
                                                                                iq_right_count.q1_count);
                    fwrite(log_char, 1, strlen(log_char), wrong_fp);
                    if(AllChannelsMatched(&iq_right_count, buf_size) == 1)
                    {
                        shift_suc++;
                        char file_name[128];
                        strcpy(file_name, sou_file_name);
                        strcat(file_name, ".right");
                        FILE *cor_fp = fopen(file_name, "wb");
                        fwrite(tmp, 1, buf_size, cor_fp);
                        fclose(cor_fp);
                    }
                }

                memset(log_char, 0, 256);
                sprintf(log_char, "-----------------------------complete--------------------------------\n");
                fwrite(log_char, 1, strlen(log_char), wrong_fp);
                memset(log_char, 0, 256);
                sprintf(log_char, "\n\n");
                fwrite(log_char, 1, strlen(log_char), wrong_fp);
            }
        }
    }
    fclose(wrong_fp);
    free(wrong_file_nums);
    printf("--------------------correct complete---------------------\n");
    printf("result:total file number:%d, match: %d, mismatch: %d, ", file_num, file_num - wrong_number, wrong_number);
    printf("shift success: %d\n", shift_suc);

    return 0;
}

int CompareBitDiff(unsigned char *sou_buf,unsigned  char *des_buf, int buf_size, pt_iq_count iq_count)
{
    unsigned int same_bit_count = 0;
    int is_low_4bit;
    int i, j;
    for(j = 0; j < buf_size; j++)
    {
        unsigned char sou_tmp = *sou_buf++;
        unsigned char des_tmp = *des_buf++;
        for(i = 0, is_low_4bit = 1; i < 2; i++)
        {
            if((sou_tmp & 0x01) == (des_tmp & 0x01))
            {
                iq_count->i0_count++;
                same_bit_count++;
            }
            if((sou_tmp & 0x02) == (des_tmp & 0x02))
            {
                iq_count->i1_count++;
                same_bit_count++;
            }
            if((sou_tmp & 0x04) == (des_tmp & 0x04))
            {
                iq_count->q0_count++;
                same_bit_count++;
            }
            if((sou_tmp & 0x08) == (des_tmp & 0x08))
            {
                iq_count->q1_count++;
                same_bit_count++;
            }
            if(is_low_4bit == 1)
            {
                sou_tmp = sou_tmp >> 4;
                des_tmp = des_tmp >> 4;
                is_low_4bit = 0;
            }
        }
    }
    return same_bit_count;
}

int AllChannelsMatched(pt_iq_count iq_count, int spi_data_size)
{
    if( (iq_count->i0_count < 2 * spi_data_size - 2) ||
        (iq_count->i1_count < 2 * spi_data_size - 2) ||
        (iq_count->q0_count < 2 * spi_data_size - 2) ||
        (iq_count->q1_count < 2 * spi_data_size - 2))
    {
        return 0;
    } else {
        return 1;
    }
}
