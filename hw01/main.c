#include <stdio.h>
#include <stdlib.h>

#define UPPER_BOUND 10000
#define LOWER_BOUND -10000
#define ERR_INTERVAL 100
#define ERR_NAN 101

int main(int argc, char *argv[])
{
  int ret = EXIT_SUCCESS;

  int first_num = 0;
  int second_num = 0;
  scanf("%d %d", &first_num, &second_num);

  if (UPPER_BOUND < first_num || first_num < LOWER_BOUND ||
  UPPER_BOUND < second_num || second_num < LOWER_BOUND) {
    fprintf(stderr, "Error: Vstup je mimo interval!\n");
    return ERR_INTERVAL;
  }

  printf("Desitkova soustava: %d %d\n", first_num, second_num);
  printf("Sestnactkova soustava: %x %x\n", first_num, second_num);

  printf("Soucet: %d + %d = %d\n", first_num, second_num, first_num + second_num);
  printf("Rozdil: %d - %d = %d\n", first_num, second_num, first_num - second_num);
  printf("Soucin: %d * %d = %d\n", first_num, second_num, first_num * second_num);
  if (second_num == 0) {
    printf("Podil: %d / %d = NaN\n", first_num, second_num);
    fprintf(stderr, "Error: Nedefinovany vysledek!\n");
    ret = ERR_NAN;
  } else {
    printf("Podil: %d / %d = %d\n", first_num, second_num, first_num / second_num);
  }
  printf("Prumer: %.1f\n", (float) (first_num + second_num) / 2);


  return ret;
}
