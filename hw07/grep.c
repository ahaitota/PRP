#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define MAX_LINE_LENGTH 1024
#define ERR_FILE_OPEN 100
#define NO_PATTERN 1
#define NOT_SUBSTR -1
#define START "\033[01;31m\033[K"
#define END "\033[m\033[K"

int get_size(char *str);
int is_substring(char *word, char *string_file, int* indices);
int get_index(char *str);
int is_substring_regex(char *word, char *string_file, char regex, char letter);

int main(int argc, char *argv[])
{
  int current_option = 0; //0 - nothing, 1 - E, 2 - colors
  int current_file = 0; // 0 - file is given, 1 - read from stdin

  FILE *file;
  char *word = NULL;
  char line[MAX_LINE_LENGTH];
  
  int ret = 0;
  int return_flag = EXIT_SUCCESS;

  int index;
  char regex;
  char letter;
  char new_str[10];

  int flag1 = is_substring("-E", argv[1], NULL);
  int flag2 = is_substring("--color=always", argv[1], NULL);
  if (flag1 >= 0) {
    //-E
    current_option = 1;
    word = argv[2];
    index = get_index(word);
    regex = word[index];
    letter = word[index - 1];
    int len = get_size(word);

    //delete char
    
    for (int i = 0, j = 0; i < len; i++) {
        if (i != index) {
            new_str[j++] = word[i];
        }
    }
    new_str[len - 1] = '\0';
    word = new_str;

    if (argc == 4) {
      // file is given -./program OPTIONS PATTERN FILE
      current_file = 0;
      file = fopen(argv[3], "r");
      if (file == NULL){
        fprintf(stderr, "Was not able to open the file!\n");
        return_flag =  ERR_FILE_OPEN;
      }
    } else {
      // file is not given -./program OPTIONS PATTERN
      current_file = 1;
      file = stdin;
    }

  } else if (flag2 >= 0) {
    //--color=always
    current_option = 2;
    word = argv[2];
    if (argc == 4) {
      // file is given -./program OPTIONS PATTERN FILE
      current_file = 0;
      file = fopen(argv[3], "r");
      if (file == NULL){
        fprintf(stderr, "Was not able to open the file!\n");
        return_flag =  ERR_FILE_OPEN;
      }
    } else {
      // file is not given -./program OPTIONS PATTERN
      current_file = 1;
      file = stdin;
    }

  } else {
    //./program PATTERN [FILE] 
    current_option = 0;
    word = argv[1];
    if (argc == 2) {
      //file is not given - //./program PATTERN
      current_file = 1;
      file = stdin;
    } else {
      // file is given - ./program PATTERN FILE
      current_file = 0;
      file = fopen(argv[2], "r");
      if (file == NULL){
        fprintf(stderr, "Was not able to open the file!\n");
        return_flag =  ERR_FILE_OPEN;
      }
    }
  }


  while (return_flag == EXIT_SUCCESS && (fgets(line, MAX_LINE_LENGTH, file)) != NULL) {
    int index_array[MAX_LINE_LENGTH] = { 0 };
    int result;
    if (current_option == 1) {
      // printf("before regex\n");
      result = is_substring_regex(word, line, regex, letter);
      // printf("after regex\n");
    } else {
      result = is_substring(word, line, index_array);
    }
    if (result != NOT_SUBSTR) {
      int count = 0;
      int i = 0;
      while (line[i] != '\0') {
        if (index_array[count] == i && current_option == 2) {
          printf("%s", START);
          for (int j = 0; j < get_size(word); j++) {
            printf("%c", line[i + j]);
          }
          printf("%s", END);
          i += get_size(word);
          count++;
        } else {
          printf("%c", line[i]);
          i++;
        }
      }
      ret++;
    }
  }

  if (ret == 0) {
    return_flag = NO_PATTERN;
  } 
  if (current_file == 0) {
    fclose(file);
  }
  
  return return_flag;

}


int get_index(char *str) {
  for (int i = 0; i < get_size(str); i++) {
    if (str[i] == '?' || str[i] == '+' || str[i] == '*') {
      return i;
    }
  }
  return 0;
}
 
int get_size(char *str) {
  int i;
  for (i = 0; str[i] != '\0'; i++);
  return i;
}
 
int is_substring(char *word, char *string_file, int* indices) {
  int word_len = get_size(word);
  int str_len = get_size(string_file);
  int return_value = NOT_SUBSTR;
  int count = 0;

  for (int i = 0; i <= str_len - word_len; i++) {
    int j = 0;
    for (j = 0; j < word_len; j++) {
      if (string_file[i + j] != word[j])
      break;
    } 
    if (j == word_len) {
      return_value = i;
      if (indices != NULL) {
        indices[count++] = i;
      }
    }   
  }
  return return_value;
}

int is_substring_regex(char *word, char *string_file, char regex, char letter) {

  int word_len = get_size(word);
  int str_len = get_size(string_file);
  int return_value = NOT_SUBSTR;

  if (regex == '?') {
    // printf("here?\n");
    for (int i = 0; i <= str_len - word_len; i++) {
      int j = 0;
      for (j = 0; j < word_len; j++) {
        if (string_file[i + j] != word[j]) {
            
          if (word[j] == letter && string_file[i + j] == word[j + 1]) {
            j++;
          } 
          else {
            break;
          }
        }
        
      } 
      if (j == word_len) {
        return_value = i;

      }   
    }
  } else if (regex == '+') {
    // printf("here+\n");
    for (int i = 0; i <= str_len - word_len; i++) {
      int j = 0;
      int count = 0;
      for (j = 0; j < word_len; j++) {
        if (string_file[i + count] != word[count]) {
            
          if (string_file[i + count] == letter && string_file[i + count] == string_file[i + count - 1]) {
            while ((string_file[i + count] == letter && string_file[i + count] == string_file[i + count - 1])) {
              count++;
            }
          }
          else {
            break;
          }
        }
        count++;
      } 
      if (j == word_len) {
        return_value = i;

      }   
    }

  } else if (regex == '*') {
    // printf("here*\n");
    for (int i = 0; i <= str_len - word_len; i++) {
      int j = 0;
      int count = 0;
      for (j = 0; j < word_len; j++) {
        if (string_file[i + count] != word[count]) {

          if (word[count] == letter && string_file[i + count] == word[count + 1]) {
            continue;
          } 
            
          if (string_file[i + count] == letter && string_file[i + count] == string_file[i + count - 1]) {
            while ((string_file[i + count] == letter && string_file[i + count] == string_file[i + count - 1])) {
              count++;
            }
          }
          else {
            break;
          }
        }
        count++;
      } 
      if (j == word_len) {
        return_value = i;

      }   
    }

  }
  // printf("here none\n");


  
  return return_value;
}



