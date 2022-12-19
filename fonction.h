#ifndef FONCTION_H_INCLUDED
#define FONCTION_H_INCLUDED

int creation_fichier_changelog(void);
int creation_fichier_resource_h(void);
int creation_fichier_resource_rc(char *FileDescription, char *ProductName);
int creation_fichier_gitignore(void);
int creation_fichier_vscode(void);
int lecture_fichier_changelog(void);

int update_fichier_changelog(double version, char *commentaire);
int update_fichier_resource_h(double version);
int update_innosetup(double version);
int update_name_resource_h(char *name);

int activation_innosetup(char* valeur);

int fonction_remove(void);

char *get_date_annee(void);

#endif // FONCTION_H_INCLUDED