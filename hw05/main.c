#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define STRING_SIZE 50
#define WRONG_LENGTH 101
#define WRONG_INPUT 100

/**
 * Reads a string from standard input, dynamically reallocating memory as needed.
 *
 * @return A dynamically allocated string read from input.
 */
char* read(void);

/**
 * Reallocates memory for a string when the current allocation is insufficient.
 *
 * @param size Pointer to the current size of the allocated memory.
 * @param str The current string that needs more memory.
 * @return A pointer to the newly allocated memory, or NULL if the allocation fails.
 */
char* make_realloc(int *size, char *str);

/**
 * Checks if a string contains only alphabetic characters.
 *
 * @param str The string to be checked.
 * @param length The length of the string.
 * @return EXIT_SUCCESS if the string is valid, WRONG_INPUT if the string contains non-alphabetic characters.
 */
int check_string(char* str, int length);

/**
 * Decrypts an input string by shifting characters and comparing to the answer string.
 *
 * @param input The encrypted input string.
 * @param answer The target string to match against.
 * @param input_len The length of the input string.
 * @param answer_len The length of the answer string.
 * @param flag A flag to determine the matching strategy.
 * @return The best-matching decrypted string.
 */
char* decrypt(char* input, char* answer, int input_len, int answer_len, char flag);

/**
 * Computes the Levenshtein distance between two strings.
 *
 * @param str1 The first string.
 * @param str2 The second string.
 * @param len1 The length of the first string.
 * @param len2 The length of the second string.
 * @return The Levenshtein distance between the two strings.
 */
int count_levenshtein(char* str1, char* str2, int len1, int len2);

/**
 * Returns the minimum of two integers.
 *
 * @param a The first integer.
 * @param b The second integer.
 * @return The minimum of the two integers.
 */
int min (int a, int b);


int main(int argc, char *argv[])
{
  //1 - optional
  char flag = 0;
  if (argc == 2) {
    flag = 1;
  }
  char* input = read();
  char* answer = read();

  int input_len = strlen(input);
  int answer_len = strlen(answer);

  if (input_len != answer_len && flag == 0) {
    fprintf(stderr, "Error: Chybna delka vstupu!\n");
    free(input);
    free(answer);
    return WRONG_LENGTH;
  }

  int input_valid = check_string(input, input_len);
  int answer_valid = check_string(answer, answer_len);

  if (input_valid == WRONG_INPUT || answer_valid == WRONG_INPUT) {
    fprintf(stderr, "Error: Chybny vstup!\n");
    free(input);
    free(answer);
    return WRONG_INPUT;
  }
  char* result = decrypt(input, answer, input_len, answer_len, flag);


  printf("%s\n", result);

  // if (result != NULL) {
  //   free(result);
  // }

  free(result);
  free(input);
  free(answer);

  return EXIT_SUCCESS;
}

char* read(void) {
  char* str = (char*) malloc(sizeof(char) * STRING_SIZE + 1);

  if (str) {
    int index = 0;
    char character;
    int size = STRING_SIZE;

    while (scanf("%c", &character) != EOF) {
      if (character == '\n') {
        break;
      }
      str[index++] = character;
      if (index == size) {
        str = make_realloc(&size, str);
        if (str == NULL) {
          break;
        }
      }
    }

    if (str) {
      str[index] = '\0';
    }
  }
  return str;
}

char* make_realloc(int *size, char *str) {
  char* t = (char*) realloc(str, sizeof(char) * (*size) * 2 + 1);
  if (!t) {
    free(str);
    str = NULL;
  } else {
    str = t;
    *size *= 2;
  }
  return str;
}

int check_string(char* str, int length) {
  for (int i = 0; i < length; i++) {
    if (!isalpha(str[i])) {
        return WRONG_INPUT;
    }
  }
  return EXIT_SUCCESS;
}

char* decrypt(char* input, char* answer, int input_len, int answer_len, char flag) {
  char* result = NULL;
  int best_match;
  if (flag == 0) {
    best_match = 0;
  } else {
    best_match = input_len;
  }

  //try to use each shift to see which one is better
  for (int variant = 0; variant < 52; variant++) {
    //make a copy of the original to apply shift on it
    char* copy = (char*) malloc((input_len + 1) * sizeof(char));

    if (copy == NULL) {
        return NULL;
    }

    strcpy(copy, input);

    //take each letter in input and try to decode it using current shift
    for (int i = 0; i < input_len; i++) {
      int next_char_code = copy[i] + variant;
      
      //if letter in lowercase 
      if (copy[i] >= 'a' && copy[i] <= 'z') {
        //if it stayed in lowercase
        if (next_char_code >= 'a' && next_char_code <= 'z') {
          copy[i] = next_char_code;
        } 
        //went further than 'z' - need to be shifted to uppercase
        else if (next_char_code <= 'z' + 26) {
          copy[i] = next_char_code - 'z' + 'A' - 1;
        } 
        //exceeds 'z' by more than 26 positions - need to (- 26) and return back to lower case
        else {
          copy[i] = next_char_code - 'z' - 26 + 'a' - 1;
        }
      }
      //if letter in uppercase
      else {
        //if it stayed in uppercase
        if (next_char_code >= 'A' && next_char_code <= 'Z') {
          copy[i] = next_char_code;
        } 
        //went further than 'Z' - need to be shifted to lowercase
        else if (next_char_code <= 'Z' + 26) {
          copy[i] = next_char_code - 'Z' + 'a' - 1;
        } 
        //exceeds 'Z' by more than 26 positions - need to (- 26) and return back to uppercase
        else {
          copy[i] = next_char_code - 'Z' - 26 + 'A' - 1;
        }
      }
    }
    //check how good is current shift
    int curr_matches = 0;
    if (flag == 0) {
      for (int j = 0; j < answer_len; j++) {
        if (copy[j] == answer[j]) {
          curr_matches += 1;
        }
      }
    } else {
      curr_matches = count_levenshtein(copy, answer, input_len, answer_len);
    }
    

    //choose the best match
    if (flag == 0) {
      if (curr_matches > best_match) {
      best_match = curr_matches;
      if (result != NULL) {
        free(result);
      }
      result = copy;
    } else {
      free(copy);
    }

    } else {
      if (curr_matches < best_match) {
        best_match = curr_matches;
        if (result != NULL) {
          free(result);
        }
        result = copy;
      } else {
        free(copy);
      }
    }
  }
  return result;
}

int min (int a, int b) {
  if (a <= b) {
    return a;
  } else {
    return b;
  }
}

int count_levenshtein(char* str1, char* str2, int len1, int len2) {
  len1 += 1;
  len2 += 1;

  int (*matrix)[len2] = malloc(sizeof(int[len1][len2]));

  //initialize
  for (int i = 0; i < len1; i++) {
    for (int j = 0; j < len2; j++) {
      matrix[i][j] = 0;
    }
  }

  for (int i = 1; i < len1; i++) {
    matrix[i][0] = i;
  }

  for (int j = 1; j < len2; j++) {
    matrix[0][j] = j;
  }

  int substitution_cost = 0;

  for (int j = 1; j < len2; j++) {
    for (int i = 1; i < len1; i++) {

      if (str1[i] == str2[j]) {
        substitution_cost = 0;
      } else {
        substitution_cost = 1;
      }

      matrix[i][j] = min(min(matrix[i - 1][j] + 1, matrix[i][j - 1] + 1), min(matrix[i][j - 1] + 1, matrix[i - 1][j - 1] + substitution_cost));
    }
  }

  int ret = matrix[len1 - 1][len2 - 1];

  free(matrix);

  return ret;

}


