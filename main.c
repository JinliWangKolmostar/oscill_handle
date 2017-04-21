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
    DataCollect(COLLECT_NUM);

#if 0

    DataFormatConverse_A();
    DataFormatConverse_B();
    DataFormatConverse_C();
    match_data();

    analyze_and_correct(COLLECT_NUM);

#endif // 0
    return 0;
}

