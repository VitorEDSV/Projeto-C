#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "login.h"
#include "home.h"
#include "patient.h"
#include "calculator.c"

// Funcao para exibir a pagina de Antropometria
void showCalculatorPage(struct Pacient* pacient);
int showPacientPage(struct Pacient* pacient);
int dadosFisicos(struct Pacient* pacient);
int showCalculadora(struct Pacient* pacient);

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void readInputLine(char* buffer, size_t bufferSize) {

    fgets(buffer, bufferSize, stdin);
    // Remover o caractere de nova linha ('\n') do buffer
    size_t length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
    }
}

void addAlteration(struct Alteration** history, float value) {
    struct Alteration* newAlteration = (struct Alteration*)malloc(sizeof(struct Alteration));
    newAlteration->value = value;
    newAlteration->date = time(NULL);
    newAlteration->next = *history;
    *history = newAlteration;
    
    FILE* file = fopen("alteration_history.txt", "a");
    if (file != NULL) {
        fprintf(file, "Value: %.2f, Date: %s", value, ctime(&newAlteration->date));
        fclose(file);
    } else {
        printf("Erro ao abrir o arquivo de historico.\n");
    }
}

// Funcao para exibir o historico de alteracoes
void showAlterationHistory(struct Alteration* history) {
    int input;
    struct Alteration* current = history;

    while (1) {
        system("cls");
        printf("Historico de alteracoes:\n");

        while (current != NULL) {
            printf(" - Valor: %.2f, Data: %s", current->value, asctime(localtime(&(current->date))));
            current = current->next;
        }

        printf("\nDigite '0' para voltar a pagina anterior\n");
        printf("Digite '1' para exibir o grafico\n");
        printf("Digite sua escolha: ");
        scanf("%d", &input);
        clearInputBuffer();

        if (input == 0) {
            break;
        } else if (input == 1) {
            // Chamada da funcao para exibir o grafico
            printf("\nExibindo o grafico...\n");
            // ... logica para exibir o grafico ...
        } else {
            printf("\nOpcao invalida. Tente novamente.\n");
        }
    }
}

// Funcao genérica para exibir a pagina de alteracao
void showAlterationPage(float* field, struct Alteration** history, const char* fieldName, void (*returnFunction)(struct Pacient*), struct Pacient* pacient) {
    system("cls");
    printf("## Pagina de Alteracao - %s ##\n", fieldName);
    printf("\nValor atual de %s: %.2f\n", fieldName, *field);

    int selectedOption;
    printf("\nDigite o numero correspondente para abrir a funcao:\n");
    printf("1. Alterar %s\n", fieldName);
    printf("2. Historico de Alteracoes\n");
    printf("0. Voltar\n");
    scanf("%d", &selectedOption);
    clearInputBuffer();

    switch (selectedOption) {
        case 0:
            printf("Voltando...\n");
            break;
        case 1: {
            float newValue;
            printf("\nDigite o novo valor de %s: ", fieldName);
            scanf("%f", &newValue);
            clearInputBuffer();
            addAlteration(history, newValue);
            *field = newValue;
            printf("%s alterado com sucesso!\n", fieldName);
            
            // Salvar em arquivo para cada paciente
            FILE* file = fopen(pacient->name, "a");
            if (file != NULL) {
                fprintf(file, " - Valor: %.2f, Data: %s", (*history)->value, asctime(localtime(&((*history)->date))));

                fclose(file);
            } else {
                printf("Erro ao salvar a alteracao em arquivo para o paciente %s.\n", pacient->name);
            }
            
            break;
        }
        case 2:
            showAlterationHistory(*history);
            break;
        default:
            printf("Opcao invalida.\n");
            break;
    }

    printf("\nPressione qualquer tecla para voltar...");
    getchar();
    clearInputBuffer();

    // Volta para a pagina principal
    returnFunction(pacient);
}

void showCadastroPaciente(const struct Pacient* pacient) {
    system("cls");
    printf("\n");
    printf("##### Informacoes de Cadastro do Paciente #####\n");
    printf("\n");
    printf("Nome: %s\n", pacient->name);
    printf("Email: %s\n", pacient->email);
    printf("Numero de telefone: %s\n", pacient->number);
    printf("CPF: %s\n", pacient->cpf);
    printf("Data de nascimento: %s\n", pacient->birth);

    int selectedOption;
    printf("\n");
    printf("\nDigite 0 para voltar a pagina anterior: ");
    scanf("%d", &selectedOption);
    clearInputBuffer();

    switch (selectedOption) {
        case 0:
            // Voltar a pagina anterior
            system("cls");
            showPacientPage(pacient);
            return;
        default:
            printf("Opcao invalida.\n");
            showCadastroPaciente(pacient);
            break;
    }
}

// Funcao para exibir a pagina de Antropometria
void showCalculatorPage(struct Pacient* pacient) {
    int choice;

    system("cls");
    printf("######## CALCULADORA ########\n");
    printf("\n");
    printf("1. Peso\n");
    printf("2. Altura\n");
    printf("3. Circunferência da Cintura\n");
    printf("4. Circunferência do Quadril\n");
    printf("\n5. Voltar\n");
    printf("Digite sua escolha: ");
    scanf("%d", &choice);
    clearInputBuffer();

    switch (choice) {
        case 1:
            showAlterationPage(&(pacient->antropometria.peso), &(pacient->pesoHistory), "Peso", showCalculatorPage, pacient);
            break;
        case 2:
            showAlterationPage(&(pacient->antropometria.altura), &(pacient->alturaHistory), "Altura", showCalculatorPage, pacient);
            break;
        case 3:
            showAlterationPage(&(pacient->antropometria.circunferenciaCintura), &(pacient->circunferenciaCinturaHistory), "Circunferência da Cintura", showCalculatorPage, pacient);
            break;
        case 4:
            showAlterationPage(&(pacient->antropometria.circunferenciaQuadril), &(pacient->circunferenciaQuadrilHistory), "Circunferência do Quadril", showCalculatorPage, pacient);
            break;
        case 5:
            showPacientPage(pacient);
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
            showCalculatorPage(pacient);
            break;
    }
}

int showPacientPage(struct Pacient* pacient) {
    system("cls");
    printf("\n");
    printf("######## Pagina do Paciente ########\n");
    printf("\n");
    printf("Paciente:  %s\n", pacient->name);
    printf("\n");

    // Tentar abrir o arquivo em modo de leitura
    FILE *arquivo = fopen("dados.txt", "r");
    if (arquivo != NULL) {
        // Ler os dados do arquivo
        fscanf(arquivo, "Peso: %f\n", &(pacient->antropometria.peso));
        fscanf(arquivo, "Altura: %f\n", &(pacient->antropometria.altura));

        // Fechar o arquivo
        fclose(arquivo);
    }

    printf("Peso Atual: %.2f\n", pacient->antropometria.peso);
    printf("Altura: %.2f\n", pacient->antropometria.altura);
    printf("\n");
    printf("\n");

    printf("1. Dados Fisicos\n");
    printf("2. Calculadora\n");
    printf("3. Dieta\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("4. Informacoes de Cadastro\n");
    printf("\n");
    printf("0. Voltar\n");

    int selectedOption;
    printf("\n");
    printf("\nDigite o numero correspondente para abrir a funcao: ");
    scanf("%d", &selectedOption);
    clearInputBuffer();

    switch (selectedOption) {
        case 0:
            printf("Voltando...\n");
            showPacientList(pacient);
            return 0;
        case 1:
            dadosFisicos(pacient);
            break;
        case 2:
            showCalculadora(pacient);
            break;
        case 3:
            exibirDieta(pacient);
            break;
        case 4:
            showCadastroPaciente(pacient);
            break;
        default:
            printf("Opcao invalida.\n");
            showPacientPage(pacient);
            break;
    }
}

void showPacientList(const char* username) {
    char filename[100];
    snprintf(filename, sizeof(filename), "%s.txt", username);

    FILE* userFile = fopen(filename, "r");
    if (userFile == NULL) {
        printf("Erro ao abrir o arquivo do usuario.\n");
        return;
    }

    struct Pacient pacient;

    system("cls");
    printf("\n");
    printf("##### Lista de Pacientes #####\n");
    printf("\n");

    int number = 1;
    while (fread(&pacient, sizeof(struct Pacient), 1, userFile)) {
        printf("%d. %s\n", number, pacient.name);
        number++;
    }

    fclose(userFile);

    int selectedNumber;
    printf("\nEscolha o paciente ou (0 para voltar): ");
    scanf("%d", &selectedNumber);
    clearInputBuffer();

    if (selectedNumber == 0) {
        printf("Voltando...\n");
        system("cls");
        return;
    } else if (selectedNumber >= 1 && selectedNumber < number) {
        userFile = fopen(filename, "r");
        if (userFile == NULL) {
            printf("Erro ao abrir o arquivo do usuario.\n");
            return;
        }

        int count = 1;
        while (fread(&pacient, sizeof(struct Pacient), 1, userFile)) {
            if (count == selectedNumber) {
                showPacientPage(&pacient);
                break;
            }
            count++;
        }

        fclose(userFile);
    } else {
        printf("Opcao invalida.\n");
    }
}

void removePacient(const char* username) {
    char filename[100];
    snprintf(filename, sizeof(filename), "%s.txt", username);

    FILE* userFile = fopen(filename, "r");
    if (userFile == NULL) {
        printf("Erro ao abrir o arquivo do usuario.\n");
        return;
    }

    struct Pacient pacient;
    char cpf[20];
    int found = 0;

    printf("Informe o CPF do paciente que deseja remover: ");
    scanf("%s", &cpf);

    FILE* tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Erro ao criar arquivo temporario.\n");
        fclose(userFile);
        return;
    }

    while (fread(&pacient, sizeof(struct Pacient), 1, userFile)) {
        if (strcmp(pacient.cpf, cpf) != 0) {
            fwrite(&pacient, sizeof(struct Pacient), 1, tempFile);
        } else {
            found = 1;
        }
    }

    fclose(userFile);
    fclose(tempFile);

    if (remove(filename) != 0) {
        printf("Erro ao remover o paciente.\n");
        return;
    }

    if (rename("temp.txt", filename) != 0) {
        printf("Erro ao renomear arquivo temporario.\n");
        return;
    }

    if (found) {
        printf("\n");
        printf("Paciente removido com sucesso!\n");
        printf("\n");
    } else {
        printf("\n");
        printf("Paciente nao encontrado.\n");
        printf("\n");
    }
}

void addPacient(const char* userName) {
    struct Pacient pacient;

    system("cls");

    printf("-----Preencha os dados do paciente-----\n");
    printf("\n");

    printf("");
    readInputLine(pacient.name, sizeof(pacient.name));

    printf("Nome Completo: ");
    readInputLine(pacient.name, sizeof(pacient.name));

    printf("Genero (m/f): ");
    readInputLine(pacient.gender, sizeof(pacient.gender));

    printf("CPF: ");
    readInputLine(pacient.cpf, sizeof(pacient.cpf));

    printf("Email: ");
    readInputLine(pacient.email, sizeof(pacient.email));

    printf("Celular: ");
    readInputLine(pacient.number, sizeof(pacient.number));

    printf("Data de nascimento (dd/mm/aa): ");
    readInputLine(pacient.birth, sizeof(pacient.birth));

    char filename[100];
    snprintf(filename, sizeof(filename), "%s.txt", userName);

    FILE* userFile = fopen(filename, "a");
    if (userFile == NULL) {
        printf("Erro ao abrir o arquivo do usuario.\n");
        return;
    }

    fwrite(&pacient, sizeof(struct Pacient), 1, userFile);
    fclose(userFile);

    printf("\n");
    printf("Paciente cadastrado com sucesso!\n");
    printf("\n");
}