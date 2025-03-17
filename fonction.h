#ifndef FONCTION_H_INCLUDED
#define FONCTION_H_INCLUDED
#include <stdbool.h>
#include "main.h"

bool add_new_changelog(const char* path, double AppVersion, const char* commentaire);
bool remove_last_changelog_entry(const char* path);
bool activation_innosetup(const char* path, ST_logator* st);
bool update_innosetup(ST_logator* st);
bool add_new_changelog(const char* path, double AppVersion,const char* commentaire);
bool Printf__changelog(const char* path);
bool changelog_file_to_char_cut(const char* path);

char* get_date_annee(void);
char* get_last_changelog(const char* path);

int GetWholePart(double x);
int GetDecimalPart(double x);
int nombreDeLigne(FILE *fichier);

double GoToDecimal(int x);
double ReadLastValue(const char *filename);
double newVersion(int versionType, double OldVersion);
double removeVersion(int versionType, double OldVersion);

#endif // FONCTION_H_INCLUDED