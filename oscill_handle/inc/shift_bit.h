#ifndef SHIFT_BIT_H
#define SHIFT_BIT_H

typedef struct bit_shift {
  int direction;
  int bits[4];
  int times;
} tBit_shift, *ptBit_shift;

void shift_bit(unsigned char *spi_buf, int buf_size, ptBit_shift shift_bit);
void test_shift_bit();

#endif  // SHIFT_BIT_H
