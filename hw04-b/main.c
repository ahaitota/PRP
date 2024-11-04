#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>

#define SIZE 1000000
#define ERR_NUMBER 100

int make_eratosthenes(bool eratosthenes[SIZE]);
void print_primes(char* word, int *primes);
bool check(char* word, int divider);

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

  while (true) {
    char word[101];
    scanf("%s", word);

    int hasLetter = 0;

    for (int i = 0; word[i] != '\0'; i++) {
        if (!isdigit(word[i])) {  
            hasLetter = 1;
            break;
        }
    }

    if (hasLetter > 0) {
      fprintf(stderr, "Error: Chybny vstup!\n");
      return_value = ERR_NUMBER;
      break;
    }

    else if (word[0] == '-') {
      fprintf(stderr, "Error: Chybny vstup!\n");
      return_value = ERR_NUMBER;
      break;
    }

    else if (word[0] == '0'){
      break;
    }
    


    else if (strlen(word) == 1 && word[0] == '1') {
      printf("Prvociselny rozklad cisla 1 je:\n1\n");
    }

    else {
        print_primes(word, primes);
    }

  }

  return return_value;
}

void print_primes(char* word, int *primes) {
  printf("Prvociselny rozklad cisla %s je:\n", word);

  int i = 0;
  
  while (strlen(word) != 1 || word[0] != '1') {
    int count = 0;

    // check if it's divisible
    int ret = check(word, primes[i]);

    if (ret) {
      printf("%d", primes[i]);
      count++;

      while(check(word, primes[i])){
        //here
        count++;
        // break;
      }

      if (count > 1) {
        printf("^%d", count);
      }
        
      if (strlen(word) != 1 || word[0] != '1') {
        printf(" x ");
      }
    }
    // break;
    i++;
  }
  
  printf("\n");
}

bool check(char* word, int divider) {
  if (strlen(word) == 1 && word[0] == '1') {
    return false;
  }

  int word_len = strlen(word);
  char new_word[101];  

  int i = 0, j = 0;
  int number = 0;
  int remainder = 0;

  while (number < divider) {
    int curr_num = word[i++] - '0';
    number = (number * 10) + curr_num;
    word_len--;
  }
  
  while (word_len >= 0) {
    int quotient = number / divider;
    remainder = number % divider;
    
    //update
    new_word[j++] = quotient + '0';
    number = (remainder * 10) + (word[i++] - '0');
    
    word_len--;

  }

  new_word[j] = '\0';

  if (remainder == 0) {
    // rewrite word
    int k = 0;
    for (k = 0; new_word[k] != '\0'; k++) {
      word[k] = new_word[k];
    }
    word[k] = '\0';

    return true;
  } 

  return false;
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
