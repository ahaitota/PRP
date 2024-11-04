#include <stdio.h>
#include <stdlib.h>

#define UPPER_BOUND 10000
#define LOWER_BOUND -10000
#define ERR_INTERVAL 100

int main(int argc, char *argv[]) {
  int number = 0;
  int count = 0;
  int positive = 0, negative = 0, even = 0, odd = 0, sum = 0;
  int curr_max = LOWER_BOUND;
  int curr_min = UPPER_BOUND;

  while (scanf("%d", &number) != EOF) {

    if (number > UPPER_BOUND || number < LOWER_BOUND) {
      printf("\nError: Vstup je mimo interval!\n");
      return ERR_INTERVAL;
    }
    if (count == 0) {
      printf("%d", number);
    }
    else {
      printf(", %d", number);
    }

    count++;
    sum += number;
    if (number > curr_max) {
      curr_max = number;
    }
    if (number < curr_min) {
      curr_min = number;
    }
    if (number < 0) {
      negative++;
    }
    if (number > 0) {
      positive++;
    }
    if (number % 2 == 0) {
      even++;
    }
    if (number % 2 != 0) {
      odd++;
    }
  }
  float converter = 100.0;
  printf("\nPocet cisel: %d\n", count);
  printf("Pocet kladnych: %d\n", positive);
  printf("Pocet zapornych: %d\n", negative);
  printf("Procento kladnych: %.2f\n", (positive * converter) / count);
  printf("Procento zapornych: %.2f\n", (negative * converter) / count);
  printf("Pocet sudych: %d\n", even);
  printf("Pocet lichych: %d\n", odd);
  printf("Procento sudych: %.2f\n", (even * converter) / count);
  printf("Procento lichych: %.2f\n", (odd * converter) / count);
  printf("Prumer: %.2f\n", (float) sum / count);
  printf("Maximum: %d\n", curr_max);
  printf("Minimum: %d\n", curr_min);

  return 0;
}
