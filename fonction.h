#ifndef FONCTION_H_INCLUDED
#define FONCTION_H_INCLUDED

int creation_fichier_changelog();
int creation_fichier_ressource_h();
int creation_fichier_ressource_rc(char* FileDescription, char* Executable_name, char* ProductName);
int creation_fichier_gitignore();

int update_fichier_changelog(char* version, char* commentaire);
int update_fichier_ressource_h(char* version);

void fonction_aide();

char* get_date_annee();

#endif // FONCTION_H_INCLUDED
