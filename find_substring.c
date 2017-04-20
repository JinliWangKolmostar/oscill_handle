#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DES_FILE_SIZE 65 * 1024
#define COMPARE_SIZE 256

static unsigned char file_contet[DES_FILE_SIZE];
static const char log_file_name[64] = "C:\\test_data\\log.txt";

int FindSubstring(char *source_file_name)
{
    int j, i;
    int match_success = 0;
    unsigned char buffer[COMPARE_SIZE];
    FILE *fp_sou = fopen(source_file_name, "rb");
    if(fp_sou == NULL)
    {
        return -1;
    }
    FILE *fp_log = fopen(log_file_name, "a+");

    for(i = 0; ; i++)
    {
        char des_file_name[64];
        sprintf(des_file_name, "C:\\test_data\\data_spi\\data_capture_interval_%d.bin", i);
        FILE *fp_des = fopen(des_file_name, "rb");
        if(fp_des == NULL)
        {
            //char log_string[128];
            //sprintf(log_string, "%s\n", source_file_name);
           // fwrite(log_string, 1, strlen(log_string), fp_log);
           // printf("%s not be matched\n", source_file_name);
            break;
        }
        int get_size = fread(file_contet, 1, DES_FILE_SIZE, fp_des);
        if(get_size == 0)
        {
            //char log_string[128];
            //sprintf(log_string, "%s\n", source_file_name);
            //fwrite(log_string, 1, strlen(log_string), fp_log);
           // printf("get_size error\n");
            break;
        }
        fclose(fp_des);

        fseek(fp_sou, 0, SEEK_SET);
        for(j = 0; j < 4; j++)
        {
            fseek(fp_sou, 512 * j, SEEK_CUR);
            fread(buffer, 1, COMPARE_SIZE, fp_sou);

            char *substring_pos = strstr((char *)file_contet, (char *)buffer);
            if(substring_pos != NULL)
            {
                char log_string[128];
                sprintf(log_string, "%s <-------> %s\n", source_file_name,
                                                         des_file_name);
                fwrite(log_string, 1, strlen(log_string), fp_log);
                match_success = 1;
                break;
            }
        }
        if(match_success == 1)
            break;
    }

    fclose(fp_log);
    fclose(fp_sou);
    return 0;
}

void match_data()
{
    int match_file_num_a = 0;
	while(1)
	{
		char sou_file_name[64];
		sprintf(sou_file_name, "C:\\test_data\\data_oscillo_a\\oscillo_valid_data_%d.bin", match_file_num_a++);
	 	if(FindSubstring(sou_file_name) == -1)
        {
            break;
        }
	}
    int match_file_num_b = 0;
	while(1)
	{
		char sou_file_name[64];
		sprintf(sou_file_name, "C:\\test_data\\data_oscillo_b\\oscillo_valid_data_%d.bin", match_file_num_b++);
	 	if(FindSubstring(sou_file_name) == -1)
        {
            break;
        }
	}
	int match_file_num_c = 0;
	while(1)
	{
		char sou_file_name[64];
		sprintf(sou_file_name, "C:\\test_data\\data_oscillo_c\\oscillo_valid_data_%d.bin", match_file_num_c++);
	 	if(FindSubstring(sou_file_name) == -1)
        {
            break;
        }
	}
	printf("-----------------match finished---------------\n");
}
