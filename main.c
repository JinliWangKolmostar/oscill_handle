#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shift_bit.h"
#include "bit_parser.h"
#include "data_collect.h"
#include "data_conver.h"
#include "find_substring.h"

#define COLLECT_NUM 120


int main()
{
    int oscillo_file_num;
   // DataCollect(COLLECT_NUM);

#if 1

    oscillo_file_num =  DataFormatConverse_A();
    DataFormatConverse_B();

    match_data();

    analyze_and_correct(oscillo_file_num);

#endif // 0
    //discardSpiHead();
    return 0;
}

