#ifndef FONCTION_H_INCLUDED
#define FONCTION_H_INCLUDED

bool creation_fonction(const char *appName);
bool newVersion_fonction(int versionType, const char *commentaire);
bool resource_fonction(const char *fileDescription);
bool gitignore_fonction(void);
bool innosetup_fonction(bool innosetup);
bool remove_fonction(void);

#endif // FONCTION_H_INCLUDED