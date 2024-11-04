#include <stdio.h>
#include <stdlib.h>

#define ERR_INPUT 100
#define MAX_MATRICES 100

typedef struct {
  int rows;
  int cols;
  int **data;
} Matrix;

/**
 * Creates a matrix with given rows and columns.
 * @param rows The number of rows.
 * @param cols The number of columns.
 * @return A pointer to the created Matrix.
 */
Matrix* create_matrix(int rows, int cols);

/**
 * Reads the elements of a matrix from the standard input.
 * @param matrix A pointer to the Matrix to be filled with data.
 * @param row The number of rows in the matrix.
 * @param col The number of columns in the matrix.
 * @return A pointer to the Matrix filled with data, or NULL on error.
 */
Matrix* read_matrix(Matrix* matrix, int row, int col);

/**
 * Prints the elements of a matrix to the standard output.
 * @param matrix A pointer to the Matrix to be printed.
 */
void print_matrix(Matrix *matrix);

/**
 * Adds two matrices.
 * @param m1 A pointer to the first Matrix.
 * @param m2 A pointer to the second Matrix.
 * @return A pointer to the resulting Matrix, or NULL on error.
 */
Matrix* addition(Matrix *m1, Matrix *m2);

/**
 * Subtracts the second matrix from the first matrix.
 * @param m1 A pointer to the first Matrix.
 * @param m2 A pointer to the second Matrix.
 * @return A pointer to the resulting Matrix, or NULL on error.
 */
Matrix* subtraction(Matrix *m1, Matrix *m2);

/**
 * Multiplies two matrices.
 * @param m1 A pointer to the first Matrix.
 * @param m2 A pointer to the second Matrix.
 * @return A pointer to the resulting Matrix, or NULL on error.
 */
Matrix* multiplication(Matrix *m1, Matrix *m2);

/**
 * Frees the memory allocated for a matrix.
 * @param matrix A pointer to the pointer of the Matrix to be freed.
 */
void free_matrix(Matrix **matrix);

int main(int argc, char *argv[])
{
  Matrix **matrices = (Matrix**) malloc(MAX_MATRICES * sizeof(Matrix*));
  char *operations = (char*) malloc ((MAX_MATRICES - 1) * sizeof(char));
  int num_matrices = 0;
  int num_operations = 0;
  int output_return = EXIT_SUCCESS;
  //read all matrices and operations 
  while (1) {
    int rows, cols;
    if (scanf("%d %d", &rows, &cols) != 2) {
      fprintf(stderr, "Error: Chybny vstup!\n");
      output_return =  ERR_INPUT;
      break;
    }

    Matrix *matrix = create_matrix(rows, cols);

    if (matrix == NULL) {
      fprintf(stderr, "Error: Chybny vstup!\n");
      output_return =  ERR_INPUT;
      break;
    }

    matrix = read_matrix(matrix, rows, cols);

    if (matrix == NULL) {
      fprintf(stderr, "Error: Chybny vstup!\n");
      output_return =  ERR_INPUT;
      break;
    }

    char curr_operation;
    if (scanf(" %c", &curr_operation) == EOF) {
      //end of input
      matrices[num_matrices++] = matrix;
      break;
    }

    //if curr is *, then read the next matrix and multiply it
    if (curr_operation == '*') {
      //read the second matrix
      int rows2, cols2;
      mult_again:
      if (scanf("%d %d", &rows2, &cols2) != 2) {
        fprintf(stderr, "Error: Chybny vstup!\n");
        output_return =  ERR_INPUT;
        free_matrix(&matrix);
        break;
      }

      Matrix *matrix2 = create_matrix(rows2, cols2);

      if (matrix2 == NULL) {
        fprintf(stderr, "Error: Chybny vstup!\n");
        output_return =  ERR_INPUT;
        free_matrix(&matrix);
        break;
      }

      matrix2 = read_matrix(matrix2, rows2, cols2);

      if (matrix2 == NULL) {
        fprintf(stderr, "Error: Chybny vstup!\n");
        output_return =  ERR_INPUT;
        free_matrix(&matrix);
        free_matrix(&matrix2);
        break;
      }

      //multiply them

      Matrix* result = multiplication(matrix, matrix2);

      if (result == NULL) {
        fprintf(stderr, "Error: Chybny vstup!\n");
        output_return =  ERR_INPUT;
        free_matrix(&matrix);
        free_matrix(&matrix2);
        break;
      }

      //read new operation but check if its * again
      if (scanf(" %c", &curr_operation) == EOF) {
        //end of input, add result to array
        free_matrix(&matrix);
        free_matrix(&matrix2);
        matrices[num_matrices++] = result;
        break;
      }
      if (curr_operation == '*') {
        //first matrix = result, matrix2 - read new matrix

        free_matrix(&matrix);
        free_matrix(&matrix2);
        matrix = result;
        goto mult_again;

      } else {
        //next is + or - then just add it to array
        free_matrix(&matrix);
        free_matrix(&matrix2);
        matrices[num_matrices++] = result;
        operations[num_operations++] = curr_operation;
      }
    } else {
      //if its + or - just add them into the array
      matrices[num_matrices++] = matrix;
      operations[num_operations++] = curr_operation;
  
    }

  }

  if (output_return == EXIT_SUCCESS) {
    Matrix *result = matrices[0];
    for (int i = 0; i < num_operations; i++) {
      if (operations[i] == '+') {
        Matrix *temp = addition(result, matrices[i + 1]);
        free_matrix(&result);
        result = temp;

        if (result == NULL) {
          fprintf(stderr, "Error: Chybny vstup!\n");
          output_return =  ERR_INPUT;
          free_matrix(&result);
          break;
        }

      } else if (operations[i] == '-') {
        Matrix *temp = subtraction(result, matrices[i + 1]);
        free_matrix(&result);
        result = temp;

        if (result == NULL) {
          fprintf(stderr, "Error: Chybny vstup!\n");
          output_return =  ERR_INPUT;
          free_matrix(&result);
          break;
        }

      }
    }

    if (output_return == EXIT_SUCCESS) {
      printf("%d %d\n", result->rows, result->cols);

      print_matrix(result);
      if (result != NULL) {
        free_matrix(&result);
      }

    }
    
  }

  for (int i = 1; i < num_matrices; i++) {
    if (matrices[i] != NULL) {
      free_matrix(&matrices[i]);
    } 
  }

  free(operations);
  free(matrices);
  return output_return;
}


void free_matrix(Matrix **matrix) {
  if (matrix == NULL || *matrix == NULL) return;

  if ((*matrix)->data != NULL) { //here?
    for (int i = 0; i < (*matrix)->rows; i++) {
      if ((*matrix)->data[i] != NULL) {
        free((*matrix)->data[i]); // Free each row
        (*matrix)->data[i] = NULL; // Nullify the row pointer to avoid re-use
      }
    }
    free((*matrix)->data); // Free the data pointer
    (*matrix)->data = NULL;
  }

  free(*matrix); // Free the Matrix struct itself
  *matrix = NULL;
}

void print_matrix(Matrix *matrix) {
  for (int i = 0; i < matrix->rows; ++i) {
    for (int j = 0; j < matrix->cols; ++j) {
      if (j < matrix->cols - 1){
        printf("%d ", matrix->data[i][j]);
      }
      else{
        printf("%d", matrix->data[i][j]);
      }
    }
    printf("\n");
  }
}

Matrix* create_matrix(int rows, int cols) {
    Matrix *matrix = (Matrix*)malloc(sizeof(Matrix));
    if (matrix == NULL) {
      return NULL;
    }
    matrix->rows = rows;
    matrix->cols = cols;
    matrix->data = (int**)malloc(rows * sizeof(int*));

    if (matrix->data == NULL) {
      free_matrix(&matrix);
      return NULL;
    }

    for (int i = 0; i < rows; i++) {
      matrix->data[i] = (int*)malloc(cols * sizeof(int));
      if (matrix->data[i] == NULL) {
        free_matrix(&matrix);
        return NULL;
      }
    }
    return matrix;
}

Matrix* read_matrix(Matrix* matrix, int row, int col) {
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < col; ++j) {
      if (!scanf("%d", &matrix->data[i][j])){
        free_matrix(&matrix);
        return NULL;
      }
    }
  }
  return matrix;
}

Matrix* addition(Matrix *m1, Matrix *m2) {
    if (m1->rows != m2->rows || m1->cols != m2->cols) {
        return NULL;
    }

    Matrix *result = create_matrix(m1->rows, m1->cols);
    for (int i = 0; i < m1->rows; i++) {
      for (int j = 0; j < m1->cols; j++) {
        result->data[i][j] = m1->data[i][j] + m2->data[i][j];
      }
    }
    return result;
}
 
Matrix* subtraction(Matrix *m1, Matrix *m2) {
  if (m1->rows != m2->rows || m1->cols != m2->cols) {
    return NULL;
  }

  Matrix *result = create_matrix(m1->rows, m1->cols);
  for (int i = 0; i < m1->rows; i++) {
    for (int j = 0; j < m1->cols; j++) {
      result->data[i][j] = m1->data[i][j] - m2->data[i][j];
    }
  }
  return result;
}
 
Matrix* multiplication(Matrix *m1, Matrix *m2) {
  if (m1->cols != m2->rows) {
    return NULL;
  }

  Matrix *result = create_matrix(m1->rows, m2->cols);
  for (int i = 0; i < m1->rows; i++) {
    for (int j = 0; j < m2->cols; j++) {
      result->data[i][j] = 0;
      for (int k = 0; k < m1->cols; k++) {
        result->data[i][j] += m1->data[i][k] * m2->data[k][j];
      }
    }
  }
  return result;
}

