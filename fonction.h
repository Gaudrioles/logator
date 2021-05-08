#ifndef FONCTION_H_INCLUDED
#define FONCTION_H_INCLUDED

int creation_fichier_changelog();
int creation_fichier_resource_h();
int creation_fichier_resource_rc(char *FileDescription, char *ProductName);
int creation_fichier_gitignore();

int update_fichier_changelog(char *version, char *commentaire);
int update_fichier_resource_h(char *version);
int update_innosetup(char *version);

int activation_innosetup(char *TRUE);
int update_name_resource_h(char *name);

void fonction_aide();

char *get_date_annee();

#endif // FONCTION_H_INCLUDED
