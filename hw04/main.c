#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define SIZE 1000000
#define ERR_NUMBER 100

int make_eratosthenes(bool eratosthenes[SIZE]);
void print_primes(long num, int *primes);

int main(int argc, char *argv[])
{
  bool eratosthenes[SIZE];
  int new_size = 0;
  new_size = make_eratosthenes(eratosthenes);
  
  int current_index = 0;
  int primes[new_size];

  //make sieve of eratosthenes
  for (int i = 0; i < SIZE; i++) {
    if (eratosthenes[i] == true) {
      primes[current_index] = i;
      current_index += 1;
    }
  }



  int return_value = EXIT_SUCCESS;

  long int num = 0;

  while (true) {
    int read_result = scanf("%ld", &num);

    if (read_result != 1) {
      fprintf(stderr, "Error: Chybny vstup!\n");
      return_value = ERR_NUMBER;
      break;
    }

    else if (num < 0) {
      fprintf(stderr, "Error: Chybny vstup!\n");
      return_value = ERR_NUMBER;
      break;
    }

    else if (num == 0){
      break;
    }
    
    else if (num == 1) {
      printf("Prvociselny rozklad cisla 1 je:\n1\n");
    }

    else {
      print_primes(num, primes);
    }

  }

  return return_value;
}

void print_primes(long num, int *primes) {
  printf("Prvociselny rozklad cisla %ld je:\n", num);
  int i = 0;
  
  while (num != 1) {
    int count = 0;

    if (num % primes[i] == 0) {
      printf("%d", primes[i]);

      while(num % primes[i] == 0){
        num = num / primes[i];
        count++;
      }

      if (count > 1)
        printf("^%d", count);
      if (num > 1)
        printf(" x ");

    }
    i++;
  }
  printf("\n");
}

int make_eratosthenes(bool eratosthenes[SIZE]) {
  // from 0 to 999 999, need +1 to get the correct number
  
  int new_size = SIZE;

  //set all to true except 1
  for (int i = 0; i < SIZE; i++) {
    if(i == 0 || i == 1) {
      eratosthenes[i] = false;
      new_size -= 1;
    } else {
      eratosthenes[i] = true;
    }
  }

  //filter out numbers
  for (int i = 0; i < sqrt(SIZE); i++) {
    //if this number is prime
    if (eratosthenes[i] == true) {
      //delete all multiples of i
      for (int j = i*i; j < SIZE; j += i) {
        if (eratosthenes[j] == true) {
          new_size -= 1;
          eratosthenes[j] = false;
        }
      }
    }
  }

  return new_size;
}
