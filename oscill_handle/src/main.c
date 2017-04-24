#include <stdio.h>
#include "../inc/bit_parser.h"
#include "../inc/data_collect.h"
#include "../inc/data_format_transform.h"
#include "../inc/match_with_spi.h"

int main(void) {
  int collect_file_num = 0;
  int origin_file_num = 0;
  char collect_data_switch;
  printf("collect data?(y/n)\n");
  while (scanf("%c", &collect_data_switch) != 1) {
    while (getchar() != '\n')
      ;
    printf("enter error! please enter again\n");
  }
  while (getchar() != '\n')
    ;
  if (collect_data_switch == 'y') {
    printf("enter number of collection:\n");
    while (scanf("%d", &collect_file_num) != 1) {
      while (getchar() != '\n')
        ;
      printf("enter error! please enter again\n");
    }
  }

  DataCollect(collect_file_num);

  origin_file_num = DataFormatTransfrom();

  int match_num = MatchData(origin_file_num);
  if (match_num == origin_file_num) {
    printf("match all\n");
  } else {
    AnalyzeAndCorrect(origin_file_num);
  }

  return 0;
}
