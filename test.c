//
// Created by Spagetik on 23.10.2022.
//12 -s
#include <stdio.h>


// check if str has char c
int hasChar(char *str, char c, int limit) {
    for (int i = 0; i < limit; i++) {
        if (str[i] == c) {
            return 1;
        }
    }
    return 0;
}


// read lines from stdin
int main() {
    char line[100+2+1]; // 100 potomu sho 100 maksimum, 2 это конец строки и возрат, 1 это ячейка для конца строки если строка боьше чем 100
    while (fgets(line, 102, stdin) != NULL) { // 102 потому что 100 + 2 + 1 минус 1 потому что fgets в конец сунет \0 если больше 100
        if (hasChar(line, '\0', 102)) { // нужно проверить первые 102 символа
            printf("НАХУЙ ИДИ");
        }
    }
    return 0;
    }