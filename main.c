#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shift_bit.h"
#include "bit_parser.h"
#include "data_collect.h"
#include "data_conver.h"
#include "find_substring.h"

#define COLLECT_NUM 337
#define PROJECT_PATH C:\\test_data

int main()
{
#if 0
    //DataCollect(COLLECT_NUM);

    DataFormatConverse_A();
    DataFormatConverse_B();
    DataFormatConverse_C();
    match_data();

#endif // 0
    analyze_and_correct(COLLECT_NUM);

    return 0;
}

