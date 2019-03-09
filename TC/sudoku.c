#include <stdio.h>

int checkValidation(int * V, int x, valueToCheck) {
    for(int i = x; i < x + 9; i ++) {
        if (V[i] == valueToCheck) {
            return 0;
        }
    }
    return 1;
}

int naoHaViolacao(int x, int i, int * V) {
    int aux = 0;
    // Verificando a validade na linha
    do {
        if (!checkValidation(V, aux, x)) {
            return 0;
        }
       aux += 9;
    } while (aux <= 72);

    aux = i;
    // Verificando a validade na coluna
    while(aux >= 0) {
        aux -= 9;
    }
    aux += 9;
    do {
        if (V[aux] == x && i != aux) {
            return 0;
        }
        aux += 9;
    } while (aux < 72);

    // Verificando a validade no quadrante

}

int resolveSudoku(int * V, int i) {
    if (i >= 81) return 1;
    int x = 1;
    if (!V[i]) {
        do {
            if (naoHaViolacao(x, i, V)) {
                V[i] = x;
            }
            if (resolveSudoku(V, i+1)) return 1;
            x++;
        } while(x < 10);
        return 0;
    }
    resolveSudoku(V, i+1);
}

int main() {
    int data[81] = {
            5, 3, 0, 0, 7, 0, 0, 0, 0, // 0
            6, 0, 0, 1, 9, 5, 0, 0, 0, // 9
            0, 9, 8, 0, 0, 0, 0, 6, 0, // 18
            8, 0, 0, 0, 6, 0, 0, 0, 3, // 27
            4, 0, 0, 8, 0, 3, 0, 0, 1, // 36
            7, 0, 0, 0, 2, 0, 0, 0, 6, // 45
            0, 6, 0, 0, 0, 0, 2, 8, 0, // 54
            0, 0, 0, 4, 1, 9, 0, 0, 5, // 63
            0, 0, 0, 0, 8, 0, 0, 7, 9  // 72
    };

    resolveSudoku(data, 0);
    imprime(data);

    return 0;
}