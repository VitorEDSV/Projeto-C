#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "login.h"
#include "add_patient.c"

int homePage(const char* userName) {
    int opcao;

    do {
        printf("\n");     
        printf("##### SOLUCOES NUTRICIONAIS #####\n");
        // printf("Usuário logado: %s %s\n", loggedInUser->firstName, loggedInUser->lastName);
        printf("\n");

        printf("1. Adicionar Paciente\n");
        printf("2. Remover Pacientes\n");
        printf("3. Lista de Pacientes\n");
        printf("0. Sair\n");
        
        printf("\n");

        printf("Opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                addPacient(userName);
                break;
            case 2:
                removePacient(userName);
                break;
            case 3:
                showPacientList(userName);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }

        printf("\n");
    } while(opcao != 0);

    return 0;
}