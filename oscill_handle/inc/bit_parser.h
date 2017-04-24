#ifndef BIT_PARSER_H
#define BIT_PARSER_H


typedef struct __iq_match_count
{
    int  i0_count;
    int  i1_count;
    int  q0_count;
    int  q1_count;
}t_iq_count, *pt_iq_count;

int analyze_and_correct(int file_num);

#endif // BIT_PARSER_H
