#ifndef BIT_PARSER_H_INCLUDED
#define BIT_PARSER_H_INCLUDED


typedef struct __iq_match_count
{
    int  i0_count;
    int  i1_count;
    int  q0_count;
    int  q1_count;
}t_iq_count, *pt_iq_count;

int analyze_and_correct(int file_num);
int CompareBitDiff(unsigned char *sou_buf, unsigned  char *des_buf, int buf_size, pt_iq_count iq_count);
int AllChannelsMatched(pt_iq_count iq_count, int spi_data_size);
#endif // BIT_PARSER_H_INCLUDED
