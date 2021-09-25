#ifndef FICHIER_H_INCLUDED
#define FICHIER_H_INCLUDED

int nombre_de_ligne(char *fichier_nom);
int innosetup_status();
int verif_fichier_existe(char* fichier_nom);

char *application_get_name();
char *remove_guillemet(char* source);

double get_version();

#endif // FICHIER_H_INCLUDED
