#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shift_bit.h"
#include "bit_parser.h"
#include "data_collect.h"
#include "data_conver.h"
#include "find_substring.h"

#define COLLECT_NUM 337

int main()
{
    //collect data from oscilloscope
    DataCollect(COLLECT_NUM);

    //transform data format as the same as spi
    DataFormatConverse_A();
    DataFormatConverse_B();
    DataFormatConverse_C();

    //match data is the same
    match_data();

    //analyze the diffirence file and try to correct them
    analyze_and_correct(COLLECT_NUM);

    return 0;
}

