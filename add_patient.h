#ifndef ADD_PATIENT_H
#define ADD_PATIENT_H

void addPacient(const char* userName);
void printPacient(const char* username);
void removePacient(const char* username);
void clearInputBuffer();
int showPacientPage(struct Pacient* pacient);
void showCadastroPaciente(const struct Pacient* pacient);

#endif
