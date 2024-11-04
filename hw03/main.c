#include <stdio.h>
#include <stdlib.h>

#define UPPER_BOUND 69
#define LOWER_BOUND 3
#define ERR_NUMBER 100
#define ERR_INTERVAL 101
#define ERR_WIDTH 102
#define ERR_FENCE 103

int main(int argc, char *argv[])
{
    int width = 0, height = 0;
    int fence = 0; // 0 - false, 1 - true 

    if (scanf("%d %d", &width, &height) != 2) {
        fprintf(stderr, "Error: Chybny vstup!\n");
        return ERR_NUMBER;
    }

    if (width < LOWER_BOUND || height < LOWER_BOUND || width > UPPER_BOUND || height > UPPER_BOUND) {
        fprintf(stderr, "Error: Vstup mimo interval!\n");
        return ERR_INTERVAL;
    }

    if (width % 2 == 0) {
        fprintf(stderr, "Error: Sirka neni liche cislo!\n");
        return ERR_WIDTH;
    }

    if (width == height) {
        if (scanf("%d", &fence) != 1) {
            fprintf(stderr, "Error: Chybny vstup!\n");
            return ERR_NUMBER;
        }
        if (fence <= 0 || fence >= height) {
            fprintf(stderr, "Error: Neplatna velikost plotu!\n");
            return ERR_FENCE;
        }
    }

    int half = (width / 2.0) + 0.5;

    // roof
    for (int k = 1; k <= half - 1; k++) {
        for (int l = k; l < half; l++) {
            printf(" ");
        }
        for (int l = 1; l <= (k * 2 - 1); l++) {
            if (l == 1 || l == (2 * k - 1)) {
                printf("X");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }  

    // square
    for (int i = 1; i <= height; i++) {
    for (int j = 1; j <= width; j++) {
        if (i == 1 || i == height) {
            printf("X");
        }
        if (i > 1 && j == 1 && i != height) {
            printf("X");
        }
        if (i > 1 && j == width && i != height) {
            printf("X");
        }
        if (width != height) {
            if (i > 1 && i < height && j > 1 && j < width) {
                printf(" ");
            }
        }
        if (width == height) {
            if (i > 1 && i < height && j > 1 && j < width) {
                if ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)) {
                    printf("o");
                } else {
                    printf("*");
                }
            }
        }
        if (j == width && i > height - fence) {
            for (int y = width + 1; y <= width + fence / 2; y++) {
                if (fence % 2 == 0) {
                    if (i == height - fence + 1 || i == height) {
                        printf("-|");
                    } else {
                        printf(" |");
                    }
                }
                if (fence % 2 == 1) {
                    if (y == width + 1) {
                        printf("|");
                    }
                    if (i == height - fence + 1 || i == height) {
                        printf("-|");
                    } else {
                        printf(" |");
                    }
                }
            }
        }
    }
    printf("\n");
}

    return EXIT_SUCCESS;
}
