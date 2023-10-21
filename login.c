#include <stdio.h>
#include <string.h>
#include "home.c"
#include "home_login.h"

struct user {
    char firstName[100];
    char lastName[20];
    char userName[20];
    char password[20];
};

struct loggedInUser {
    char firstName[100];
    char lastName[20];
};

int checkExistingUser(const char* username) {
    FILE* log = fopen("user12.txt", "r");
    if (log == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    struct user l;

    while (fread(&l, sizeof(l), 1, log)) {
        if (strcmp(username, l.userName) == 0) {
            fclose(log);
            return 1; // Usuário já existe
        }
    }

    fclose(log);
    return 0; // Usuário não encontrado
}

void login() {
    char userName[20], password[20];
    int loginSucesso = 0;

    printf("##Pagina de Login##\n");
    printf("\n");

    do {
        printf("Usuario: ");
        scanf("%s", userName);
        printf("Senha: ");
        scanf("%s", password);

        if (checkExistingUser(userName)) {
            struct user loggedInUserData;
            FILE* log = fopen("user12.txt", "r");
            while (fread(&loggedInUserData, sizeof(loggedInUserData), 1, log)) {
                if (strcmp(userName, loggedInUserData.userName) == 0 && strcmp(password, loggedInUserData.password) == 0) {
                    struct loggedInUser loggedInUser;
                    strcpy(loggedInUser.firstName, loggedInUserData.firstName);
                    strcpy(loggedInUser.lastName, loggedInUserData.lastName);
                    system("cls");
                    printf("-----Login bem sucedido-----\n");
                    printf("\n");
                    printf("Usuario logado: %s %s\n", loggedInUser.firstName, loggedInUser.lastName);
                    printf("\n");
                    loginSucesso = 1;
                    homePage(userName);

                    break;
                }
            }
            fclose(log);
        }

        if (!loginSucesso) {
            printf("\n");
            printf("Usuario ou senha incorretos. Tente novamente.\n");
            printf("\n");
            system("cls");
            printf("Usuario ou senha incorretos, tente novamente...\n");
            main();

        }
    } while (!loginSucesso);
}

void registerUser() {
    struct user l;

    printf("Primeiro Nome: ");
    scanf("%s", l.firstName);

    printf("Sobrenome: ");
    scanf("%s", l.lastName);

    int usernameExists = 1;
    while (usernameExists) {
        printf("Nome de Usuario: ");
        scanf("%s", l.userName);

        if (checkExistingUser(l.userName)) {
            printf("\n");
            printf("O nome de usuario ja esta em uso. Por favor, escolha outro nome. \n");
            printf("\n");
        } else {
            usernameExists = 0;
        }
    }

    printf("Senha: ");
    scanf("%s", l.password);

    FILE* log = fopen("user12.txt", "a");
    if (log == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    fwrite(&l, sizeof(l), 1, log);
    fclose(log);
    
    printf("\n");
    printf("#CADASTRADO COM SUCESSO!#\n");
    printf("\n");
    printf("Pressione Enter para continuar...");
    while (getchar() != '\n') {
        // Lê os caracteres restantes até encontrar uma nova linha
    }
    getchar();

    system("cls");

    login();
}