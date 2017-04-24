#include <stdio.h>
#include "shift_bit.h"

void shift_bit(unsigned char *spi_buf, int buf_size, ptBit_shift shift_bit)
{
    int i, j;
    unsigned char cur_byte;
    if(shift_bit->direction == 1)
    {
        unsigned char next_byte;
        for(i = 0; i < buf_size - 1; i++)
        {
            cur_byte = *(spi_buf + i);
            next_byte = *(spi_buf + i + 1);
            for(j = 0; j < 4; j++)
            {
                if(shift_bit->bits[j] == 1)
                {
                    cur_byte = (cur_byte & ~(1 << j)) | (((cur_byte & (1 << (j + 4)))) >> 4);
                    cur_byte = (cur_byte & ~(1 << (j + 4))) | ((next_byte & (1 << j)) << 4);
                }
            }
            *(spi_buf + i) = cur_byte;
        }

        cur_byte = *(spi_buf + i);
        for(j = 0; j < 4; j++)
        {
            if(shift_bit->bits[j] == 1)
            {
                cur_byte = (cur_byte & ~(1 << j)) | (((cur_byte & (1 << (j + 4)))) >> 4);
            }
        }
        *(spi_buf + i) = cur_byte;
    }
    if(shift_bit->direction == 0)
    {
        unsigned char pre_byte;
        for(i = buf_size - 1; i > 0; i--)
        {
            cur_byte = *(spi_buf + i);
            pre_byte = *(spi_buf + i -1);
            for(j = 0; j < 4; j++)
            {
                if(shift_bit->bits[j] == 1)
                {
                    cur_byte = (cur_byte & ~(1 << (j + 4))) | (((cur_byte & (1 << j))) << 4);
                    cur_byte = (cur_byte & ~(1 << j)) | ((pre_byte & (1 << (j + 4))) >> 4);
                }
            }
            *(spi_buf + i) = cur_byte;
        }

        cur_byte = *(spi_buf + i);
        for(j = 0; j < 4; j++)
        {
            if(shift_bit->bits[j] == 1)
            {
                cur_byte = (cur_byte & ~(1 << (j + 4))) | (((cur_byte & (1 << j))) << 4);
            }
        }
        *(spi_buf + i) = cur_byte;
    }
}
