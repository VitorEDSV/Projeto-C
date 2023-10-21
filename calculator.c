#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "patient.h"
#include "add_patient.h"

int calculateAge(char* birthDate) {
    // Calcula a idade do paciente a partir da data de nascimento

    time_t now;
    struct tm* currentDate;
    int currentDay, currentMonth, currentYear;
    int birthDay, birthMonth, birthYear;
    int age;

    // Obtém a data atual
    time(&now);
    currentDate = localtime(&now);
    currentDay = currentDate->tm_mday;
    currentMonth = currentDate->tm_mon + 1; // tm_mon é base 0
    currentYear = currentDate->tm_year + 1900; // tm_year é baseado em anos desde 1900

    // Extrai dia, mês e ano da data de nascimento
    sscanf(birthDate, "%d/%d/%d", &birthDay, &birthMonth, &birthYear);

    // Calcula a idade
    age = currentYear - birthYear;

    // Verifica se o aniversário já ocorreu este ano
    if (currentMonth < birthMonth || (currentMonth == birthMonth && currentDay < birthDay)) {
        age--;
    }

    return age;
}

float calculateTMB(struct Pacient* pacient) {
    // Calcula a Taxa Metabólica Basal (TMB) do paciente considerando o fator de atividade

    int age = calculateAge(pacient->birth);
    float weight = pacient->antropometria.peso;
    float height = pacient->antropometria.altura;
    float bmr;
    float activityFactor;

    if (strcmp(pacient->gender, "masculino") == 0) {
        bmr = 66.5 + (13.75 * weight) + (5 * height) - (6.75 * age);
    } else {
        bmr = 655.1 + (9.56 * weight) + (1.85 * height) - (4.68 * age);
    }

    // Solicitação e verificação do nível de atividade
    int validActivityLevel = 0;
    while (!validActivityLevel) {


        if (strcmp(pacient->antropometria.activityLevel, "s") == 0) {
            activityFactor = 1.2;
            validActivityLevel = 1;
        } else if (strcmp(pacient->antropometria.activityLevel, "l") == 0) {
            activityFactor = 1.375;
            validActivityLevel = 1;
        } else if (strcmp(pacient->antropometria.activityLevel, "m") == 0) {
            activityFactor = 1.55;
            validActivityLevel = 1;
        } else if (strcmp(pacient->antropometria.activityLevel, "a") == 0) {
            activityFactor = 1.725;
            validActivityLevel = 1;
        } else if (strcmp(pacient->antropometria.activityLevel, "ma") == 0) {
            activityFactor = 1.9;
            validActivityLevel = 1;
        } else {
            printf("Opção inválida. Digite novamente.\n");
            calculateTMB(pacient);
        }
    }

    // Aplica o fator de atividade ao resultado da TMB
    float bmrWithActivity = bmr * activityFactor;

    return bmrWithActivity;
}

float calculateIMC(float peso, float altura) {
    // Verifica se a altura é válida (diferente de zero)
    if (altura == 0) {
        printf("Altura inválida!\n");
        return -1; // Retorna um valor inválido para indicar erro
    }

    // Calcula o IMC
    float imc = peso / (altura * altura);
    return imc;
}

int showCalculadora(struct Pacient* pacient) {
    system("cls");
    printf("\n");
    printf("##### Calculadora Metabolica #####\n");
    printf("\n");
     printf("\n");

    float bmr = calculateTMB(pacient);
            printf("\n");
            printf("Taxa Metabolica Basal (TMB): %.2f calorias\n", bmr);
    
    float imc = calculateIMC(pacient->antropometria.peso, pacient->antropometria.altura);
            printf("\n");
            printf("IMC (Indice de Massa Corporal): %.2f\n", imc);
    
    printf("\n");
    printf("\n");
    
    printf("Pressione Enter para Voltar...");
    clearInputBuffer();    
    // Chama a função novamente para permanecer na calculadora
    return showPacientPage(pacient);
}

int dadosFisicos(struct Pacient* pacient) {
    system("cls");

    // Tentar abrir o arquivo em modo de leitura
    FILE *arquivo = fopen("dados.txt", "r");
    if (arquivo != NULL) {
        // Ler os dados do arquivo
        fscanf(arquivo, "Peso: %f\n", &(pacient->antropometria.peso));
        fscanf(arquivo, "Altura: %f\n", &(pacient->antropometria.altura));
        fscanf(arquivo, "Nivel de Atividade: %s\n", pacient->antropometria.activityLevel);

        // Fechar o arquivo
        fclose(arquivo);
    }

    printf("###### DADOS FISICOS ######\n");

    // Exibir os valores lidos do arquivo
    printf("Peso (kg): %.2f\n", pacient->antropometria.peso);
    printf("Altura (0.00m): %.2f\n", pacient->antropometria.altura);
    printf("Nivel de Atividade: %s\n", pacient->antropometria.activityLevel);

    printf("Novo Peso (kg): ");
    scanf("%f", &(pacient->antropometria.peso));
    printf("Nova Altura (0.00m): ");
    scanf("%f", &(pacient->antropometria.altura));
    printf("Novo Nivel de Atividade: (s - Sedentario)  (l - Leve)  (m - Moderado)  (a - Ativo)  (ma - Muito Ativo):  ");
    scanf("%s", pacient->antropometria.activityLevel);
    printf("Pressione Enter para continuar...");
    getchar();
    system("cls");
    printf("Dados Alterados Com Sucesso...");

    // Abrir o arquivo em modo de gravação
    arquivo = fopen("dados.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return -1; // Indica erro
    }

    // Gravar os dados no arquivo
    fprintf(arquivo, "Peso: %.2f\n", pacient->antropometria.peso);
    fprintf(arquivo, "Altura: %.2f\n", pacient->antropometria.altura);
    fprintf(arquivo, "Nivel de Atividade: %s\n", pacient->antropometria.activityLevel);

    // Fechar o arquivo
    fclose(arquivo);

    return showPacientPage(pacient);
}

void adicionarDieta(struct Pacient* pacient) {
    system("cls");

    printf("###### ADICIONAR DIETA ######\n");
    printf("\n");
    printf("Paciente: %s\n", pacient->name);
    printf("\n");

    // Matriz para armazenar a dieta por dia da semana e período do dia
    char dieta[7][3][100];

    // Nomes dos períodos do dia
    char periodos[3][10] = {"Manha", "Tarde", "Noite"};

    // Loop para preencher a tabela interativa
    int dia, periodo;
    for (dia = 0; dia < 7; dia++) {
        printf("Dia %d:\n", dia + 1);
        for (periodo = 0; periodo < 3; periodo++) {
            printf(" - %s: ", periodos[periodo]);
            fgets(dieta[dia][periodo], sizeof(dieta[dia][periodo]), stdin);
            dieta[dia][periodo][strcspn(dieta[dia][periodo], "\n")] = '\0';
        }
        printf("\n");
    }

    // Atribuir a tabela de dieta ao paciente
    memcpy(pacient->dieta, dieta, sizeof(pacient->dieta));

    printf("Dieta adicionada com sucesso!\n");

    printf("\nPressione Enter para continuar...");
    getchar();
    system("cls");
}

void exibirDieta(struct Pacient* pacient) {
    system("cls");

    printf("######## EXIBIR DIETA ########\n");
    printf("\n");
    printf("Paciente: %s\n", pacient->name);
    printf("\n");

    // Matriz para armazenar a dieta por dia da semana e período do dia
    char dieta[7][3][100];
    memcpy(dieta, pacient->dieta, sizeof(dieta));

    // Nomes dos períodos do dia
    char periodos[3][10] = {"Manha", "Tarde", "Noite"};

    // Exibição da tabela de dieta
    printf("+-----------+-------------+-------------+-------------+\n");
    printf("|   Dia     |   Manha     |   Tarde     |   Noite     |\n");
    printf("+-----------+-------------+-------------+-------------+\n");

    int dia, periodo;
    for (dia = 0; dia < 7; dia++) {
        printf("|  Dia %d    ", dia + 1);
        for (periodo = 0; periodo < 3; periodo++) {
            printf("| %-12s ", dieta[dia][periodo]);
        }
        printf("|\n");
        printf("+-----------+-------------+-------------+-------------+\n");
    }

    int selectedOption;
    printf("\n");
    printf("Digite o numero correspondente para abrir a funcao (0 para voltar): ");
    scanf("%d", &selectedOption);
    clearInputBuffer();

    switch (selectedOption) {
        case 0:
            printf("Voltando...\n");
            showPacientPage(pacient);
            return;
        case 1:
            adicionarDieta(pacient);
            break;
        default:
            printf("Opção inválida.\n");
            break;
    }

    exibirDieta(pacient); // Chamada recursiva para exibir novamente a dieta após adicionar
}