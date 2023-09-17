#include <stdio.h>
#include <stdlib.h>

// Função para multiplicar duas matrizes
int **multiplicarMatrizes(int **matrizA, int **matrizB, int linhasA, int colunasA, int colunasB) {
    int **resultado = (int **)malloc(linhasA * sizeof(int *));
    for (int i = 0; i < linhasA; i++) {
        resultado[i] = (int *)malloc(colunasB * sizeof(int));
    }

    for (int i = 0; i < linhasA; i++) {
        for (int j = 0; j < colunasB; j++) {
            resultado[i][j] = 0;
            for (int k = 0; k < colunasA; k++) {
                resultado[i][j] += matrizA[i][k] * matrizB[k][j];
            }
        }
    }

    return resultado;
}

// Função para liberar a memória alocada para uma matriz
void liberarMatriz(int **matriz, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

int main() {
    int linhasA = 3;
    int colunasA = 2;
    int colunasB = 3;

    // Alocar e inicializar as matrizes (exemplo)
    int **matrizA = (int **)malloc(linhasA * sizeof(int *));
    for (int i = 0; i < linhasA; i++) {
        matrizA[i] = (int *)malloc(colunasA * sizeof(int));
        for (int j = 0; j < colunasA; j++) {
            matrizA[i][j] = i + j;
        }
    }

    int **matrizB = (int **)malloc(colunasA * sizeof(int *));
    for (int i = 0; i < colunasA; i++) {
        matrizB[i] = (int *)malloc(colunasB * sizeof(int));
        for (int j = 0; j < colunasB; j++) {
            matrizB[i][j] = i * j;
        }
    }

    // Chamar a função para multiplicar as matrizes
    int **resultado = multiplicarMatrizes(matrizA, matrizB, linhasA, colunasA, colunasB);

    // Imprimir o resultado
    for (int i = 0; i < linhasA; i++) {
        for (int j = 0; j < colunasB; j++) {
            printf("%d ", resultado[i][j]);
        }
        printf("\n");
    }

    // Liberar a memória alocada para as matrizes e o resultado
    liberarMatriz(matrizA, linhasA);
    liberarMatriz(matrizB, colunasA);
    liberarMatriz(resultado, linhasA);

    return 0;
}
