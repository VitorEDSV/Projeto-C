#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "login.c"

int main() {
    int cho;
    system("cls");
    printf("\n ###### SOLUCOES NUTRICIONAIS ######\n");
    printf("\n");
    printf("'1' Cadastro\n'2' Login\n");
    printf("\n");
    printf("Opcao: "); scanf("%d", &cho);

    if (cho == 1) {
        system("cls");
        registerUser();
    } else if (cho == 2) {
        system("cls");
        login();
    }

    return 0;
}