#include <stdio.h>

#define SPI_MAX_SIZE 131120
#define HEAD_OFFSET 48
#define FILE_NAME_MAX_LEN 256

void discardSpiHead()
{
  char buffer[SPI_MAX_SIZE];
  int i;
  for(i = 0; ;i++)
  {
    char file_name[FILE_NAME_MAX_LEN];
    sprintf(file_name, "/Users/wangjingli/Documents/data/00_test/data_spi/data_capture_interval_handle_%d.bin", i);
    FILE *fp_src = fopen(file_name, "rb");
    if(fp_src == NULL)
    {
      break;
    }
    size_t file_size = fread(buffer, 1, SPI_MAX_SIZE, fp_src);
    fclose(fp_src);
    sprintf(file_name, "/Users/wangjingli/Documents/data/00_test/data_spi/data_capture_interval_handle_%d.bin", i);
    FILE *fp_des = fopen(file_name, "w");
    fwrite(buffer + HEAD_OFFSET, 1, file_size - HEAD_OFFSET, fp_des);
    fclose(fp_des);
  }
}

