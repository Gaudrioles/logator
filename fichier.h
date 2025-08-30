#ifndef FICHIER_H_INCLUDED
#define FICHIER_H_INCLUDED
#include <stdbool.h>
#include <stdio.h>
#include "main.h"

bool DemandeAccordFichier(const char *FichierNom, int valeur);
bool loadResourceToStruct(const char *path, ST_logator *st);
bool loadResourceToVariable(const char *path, char *AppName, double *AppVersion, bool *AppInno);
bool write_fichier_resource_h(const char *path, ResourceData *data);
bool VerifExiste(const char *path);
bool creation_fichier_changelog(const char *path);
bool creation_fichier_resource_h(const char *path, ResourceData *data);
bool creation_fichier_resource_rc(const char *path, const char *Description, ResourceData *data);
bool creation_fichier_gitignore(const char *path);
bool parseResourceFile(const char *filePath, ResourceData *data);

int getNombreDeLigne(FILE *fichier);

char *FichierToChar(const char *FichierNom);
char *GetLastValue(void);

#endif // FICHIER_H_INCLUDED
