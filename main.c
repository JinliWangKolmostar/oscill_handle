#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shift_bit.h"
#include "bit_parser.h"
#include "data_collect.h"
#include "data_conver.h"
#include "find_substring.h"

const int origin_file_num = 100;

int main()
{
    int oscillo_file_num;
   // DataCollect(COLLECT_NUM);

    // int origin_file_num = getFileNumInFolder();
    DataFormatConverse_A(origin_file_num);
    DataFormatConverse_B(origin_file_num);

    int match_num = match_data(origin_file_num);
    if(match_num == origin_file_num)
    {
        printf("match all\n");
    }
    else
    {
        analyze_and_correct(origin_file_num);
    }

    return 0;
}

