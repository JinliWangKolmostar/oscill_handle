#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/config.h"


static int ClkIsFallingEdge(unsigned char *buffer)
{
    return ((*(buffer - 1) & CLK_BIT) && !(*buffer & CLK_BIT));
}

// int IsFallingEdge_b(unsigned char *buffer)
// {
//     return ((*buffer & 0x20) && !(*(buffer + 1)& 0x20));
// }

int DataFormatTransfrom()
{
    int origin_file_num = 0;
    FILE *fp_source, *fp_destin;
    __int64 data_wrap = 0;
    size_t data_size;
    const unsigned char data_mask = 0x0F;
    unsigned char *read_buffer = (unsigned char *)malloc(GET_DATA_TIME * SINGLE_READ_MAX_LEN * sizeof(char));
    int i;
    for(i = 0; ; i++)
    {
        char origin_file_name[FILE_NAME_MAX_LEN];
        char valid_file_name[FILE_NAME_MAX_LEN];
        sprintf(origin_file_name, WORK_PATH"origin_data/rigol_data_%d.bin", i);
        sprintf(valid_file_name, WORK_PATH"data_oscillo_a/oscillo_valid_data_%d.bin", i);
        fp_source = fopen(origin_file_name, "rb");
        if(fp_source == NULL)
        {
            origin_file_num = i;
            break;
        }
        fp_destin = fopen(valid_file_name, "wb");

        if((data_size = fread(read_buffer, 1, GET_DATA_TIME * SINGLE_READ_MAX_LEN, fp_source)) != 0)
        {
            int count = 0;
            int wrap_4bit_count = 0;
            __int64 *write_slip = (__int64 *)read_buffer;

            while(((*(read_buffer + count) & CS_BIT) == 0) && (count < data_size))
            {
                count++;
            }

            while((count < data_size) && (*(read_buffer+count) & CS_BIT) != 0)
            {
                if(ClkIsFallingEdge(read_buffer + count) == 1)
                {
                    data_wrap |= (__int64)(*(read_buffer + count) & data_mask) << (4 * wrap_4bit_count);

                    wrap_4bit_count++;
                    if(wrap_4bit_count % 16 == 0)
                    {
                        *write_slip++ = data_wrap;
                        data_wrap = 0;
                    }
                }
                count++;
            }
            if(data_wrap != 0)
            {
                *write_slip = data_wrap;
                data_wrap = 0;
            }

            //fwrite(read_buffer, 1, SPI_DATA_SIZE, fp_destin);
            fwrite(read_buffer, 1, (wrap_4bit_count + 1) / 2, fp_destin);
        }
        fclose(fp_source);
        fclose(fp_destin);
    }

    free(read_buffer);
    printf("---------------data transform finished-------------\n");
    return origin_file_num;
}


// void DataFormatConverse_B(int origin_file_num)
// {
//     FILE *fp_source, *fp_destin;
//     __int64 data_wrap = 0;
//     size_t data_size;
//     unsigned char *read_buffer = (unsigned char *)malloc(GET_DATA_TIME * SINGLE_READ_MAX_LEN * sizeof(char));
//     int oscill_file_num;
//     int i;
//     for(i = 0; i < origin_file_num; i++)
//     {
//         char origin_file_name[FILE_NAME_MAX_LEN];
//         char valid_file_name[FILE_NAME_MAX_LEN];
//         sprintf(origin_file_name, WORK_PATH"origin_data/rigol_data_%d.bin", i);
//         sprintf(valid_file_name, WORK_PATH"data_oscillo_b/oscillo_valid_data_%d.bin", i);
//         fp_source = fopen(origin_file_name, "rb");
//         if(fp_source == NULL)
//         {
//             break;
//         }
//         fp_destin = fopen(valid_file_name, "wb");

//         if((data_size = fread(read_buffer, 1, GET_DATA_TIME * SINGLE_READ_MAX_LEN, fp_source)) != 0)
//         {
//             int count = 0;
//             int wrap_4bit_count = 0;
//             __int64 *write_slip = (__int64 *)read_buffer;

//             while(((*(read_buffer + count) & 0x10) == 0) && (count < data_size))
//             {
//                 count++;
//             }

//             while((count < data_size) && (*(read_buffer+count) & 0x10) != 0)
//             {
//                 if(IsFallingEdge_b(read_buffer + count) == 1)
//                 {
//                     data_wrap |= (__int64)(*(read_buffer + count) & data_mask) << (4 * wrap_4bit_count);

//                     wrap_4bit_count++;
//                     if(wrap_4bit_count % 16 == 0)
//                     {
//                         *write_slip++ = data_wrap;
//                         data_wrap = 0;
//                     }
//                 }
//                 count++;
//             }
//             if(data_wrap != 0)
//             {
//                 *write_slip = data_wrap;
//                 data_wrap = 0;
//             }
//             fwrite(read_buffer, 1, (wrap_4bit_count + 1) / 2, fp_destin);
//             //fwrite(read_buffer, 1, SPI_DATA_SIZE, fp_destin);
//         }
//         fclose(fp_source);
//         fclose(fp_destin);
//     }

//     free(read_buffer);
//     printf("-------------data transform B type finished-----------\n");
// }

