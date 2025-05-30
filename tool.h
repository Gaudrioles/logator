#ifndef TOOL_H_INCLUDED
#define TOOL_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>
#include "main.h"

bool add_new_changelog(const char *path, const char *appVersion, const char *commentaire);
bool update_innosetup(ResourceData *data);
bool activation_innosetup(const char *path, ResourceData *data);
bool remove_last_changelog_entry(const char *path);
bool Printf__changelog(const char *path);
bool changelog_file_to_char_cut(const char *path);

char *get_date_annee(void);
char *get_last_changelog(const char *path);

int nombreDeLigne(FILE *fichier);

double GoToDecimal(int x);

void getNewVersion(char *appVersion, int versionType, int entier, int decimal, VersionModif modif);
void extractNumbers(const char *src, int *entier, int *decimal);

#endif /* !TOOL_H_INCLUDED */