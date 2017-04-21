#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SPI_DATA_SIZE 65536
#define GET_DATA_TIME 18

static const unsigned char data_mask = 0x0F;
static const unsigned int SINGLE_READ_MAX_LEN = 250000;

int IsFallingEdge_a(unsigned char *buffer)
{
    return ((*(buffer - 1) & 0x20) && !(*buffer & 0x20));
}

int IsFallingEdge_b(unsigned char *buffer)
{
    return ((*buffer & 0x20) && !(*(buffer + 1)& 0x20));
}

void DataFormatConverse_A()
{
    __int64 data_wrap = 0;
	size_t data_size;
	unsigned char *read_buffer = (unsigned char *)malloc(GET_DATA_TIME * SINGLE_READ_MAX_LEN * sizeof(char));
	int i;

	for(i = 0; ; i++)
	{
		char origin_file_name[128];
		char valid_file_name[128];
		sprintf(origin_file_name, "C:\\00_zhenguo_work\\00_test\\origin_data\\rigol_data_%d.bin", i);
		sprintf(valid_file_name, "C:\\00_zhenguo_work\\00_test\\data_oscillo_a\\oscillo_valid_data_%d.bin", i);
		FILE *fp_source = fopen(origin_file_name, "rb");
		if(fp_source == NULL)
        {
            break;
        }
		FILE *fp_destin = fopen(valid_file_name, "wb");

		if((data_size = fread(read_buffer, 1, GET_DATA_TIME * SINGLE_READ_MAX_LEN, fp_source)) != 0)
		{
			int count = 0;
			int wrap_4bit_count = 0;
			__int64 *write_slip = (__int64 *)read_buffer;

			while(((*(read_buffer + count) & 0x10) == 0) && (count < data_size))
			{
				count++;
			}

			while((count < data_size) && (*(read_buffer+count) & 0x10) != 0)
			{
				if(IsFallingEdge_a(read_buffer + count) == 1)
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
	printf("-----------------data handle finished---------------\n");
}


void DataFormatConverse_B()
{
    __int64 data_wrap = 0;
	size_t data_size;
	unsigned char *read_buffer = (unsigned char *)malloc(GET_DATA_TIME * SINGLE_READ_MAX_LEN * sizeof(char));

	int i;
	for(i = 0; ; i++)
	{
		char origin_file_name[128];
		char valid_file_name[128];
		sprintf(origin_file_name, "C:\\00_zhenguo_work\\00_test\\origin_data\\rigol_data_%d.bin", i);
		sprintf(valid_file_name, "C:\\00_zhenguo_work\\00_test\\data_oscillo_b\\oscillo_valid_data_%d.bin", i);
		FILE *fp_source = fopen(origin_file_name, "rb");
		if(fp_source == NULL)
        {
            break;
        }
		FILE *fp_destin = fopen(valid_file_name, "wb");

		if((data_size = fread(read_buffer, 1, GET_DATA_TIME * SINGLE_READ_MAX_LEN, fp_source)) != 0)
		{
			int count = 0;
			int wrap_4bit_count = 0;
			__int64 *write_slip = (__int64 *)read_buffer;

			while(((*(read_buffer + count) & 0x10) == 0) && (count < data_size))
			{
				count++;
			}

			while((count < data_size) && (*(read_buffer+count) & 0x10) != 0)
			{
				if(IsFallingEdge_b(read_buffer + count) == 1)
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
			fwrite(read_buffer, 1, (wrap_4bit_count + 1) / 2, fp_destin);
			//fwrite(read_buffer, 1, SPI_DATA_SIZE, fp_destin);
		}
		fclose(fp_source);
		fclose(fp_destin);
	}

	free(read_buffer);
	printf("-----------------data handle finished---------------\n");
}

void DataFormatConverse_C()
{
    __int64 data_wrap = 0;
	size_t data_size;
	unsigned char *read_buffer = (unsigned char *)malloc(GET_DATA_TIME * SINGLE_READ_MAX_LEN * sizeof(char));

	int i;
	for(i = 0; ; i++)
	{
		char origin_file_name[128];
		char valid_file_name[128];
		sprintf(origin_file_name, "C:\\00_zhenguo_work\\00_test\\origin_data\\rigol_data_%d.bin", i);
		sprintf(valid_file_name, "C:\\00_zhenguo_work\\00_test\\data_oscillo_c\\oscillo_valid_data_%d.bin", i);
		FILE *fp_source = fopen(origin_file_name, "rb");
		if(fp_source == NULL)
        {
            break;
        }
		FILE *fp_destin = fopen(valid_file_name, "wb");

		if((data_size = fread(read_buffer, 1, GET_DATA_TIME * SINGLE_READ_MAX_LEN, fp_source)) != 0)
		{
			int count = 0;
			int wrap_4bit_count = 0;
			__int64 *write_slip = (__int64 *)read_buffer;

			while(((*(read_buffer + count) & 0x10) == 0) && (count < data_size))
			{
				count++;
			}

            data_wrap |= (__int64)(*(read_buffer + count) & data_mask) << (4 * wrap_4bit_count);
            count++;
            wrap_4bit_count++;

			while((count < data_size) && (*(read_buffer+count) & 0x10) != 0)
			{
				if(IsFallingEdge_b(read_buffer + count) == 1)
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
			fwrite(read_buffer, 1, (wrap_4bit_count + 1) / 2, fp_destin);
			//fwrite(read_buffer, 1, SPI_DATA_SIZE, fp_destin);
		}
		fclose(fp_source);
		fclose(fp_destin);
	}

	free(read_buffer);
	printf("-----------------data handle finished---------------\n");
}
