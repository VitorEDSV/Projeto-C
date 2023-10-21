#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Antropometria {
    float peso;
    float altura;
    float circunferenciaCintura;
    float circunferenciaQuadril;
    char activityLevel[20];
};

struct Alteration {
    float value;
    time_t date;
    struct Alteration* next;
};

struct Pacient {
    char name[100];
    char email[100];
    char number[50];
    char cpf[20];
    char birth[20];
    char gender[20];
    char dieta[1000];
    struct Antropometria antropometria;
    struct Alteration* pesoHistory;
    struct Alteration* alturaHistory;
    struct Alteration* circunferenciaCinturaHistory;
    struct Alteration* circunferenciaQuadrilHistory;
    
};