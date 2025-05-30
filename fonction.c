#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"
#include "tool.h"
#include "fichier.h"
#include "message.h"

bool creation_fonction(const char *appName)
{
    /* Création du fichier changelog.md */
    if(!creation_fichier_changelog(CHANGELOG_FILE))
    {
        printf_creation_fichier(CHANGELOG_FILE, 0);
        return false;
    }
    
    /* MSG */
    printf_creation_fichier(CHANGELOG_FILE, 1);
    
    /* Data */
    ResourceData data;
    snprintf(data.appName, sizeof(data.appName), "%s", appName);
    snprintf(data.appVersion, sizeof(data.appVersion), "%s", "0.1");
    snprintf(data.innoSetup, sizeof(data.innoSetup), "%s", "FALSE");

    /* Creation ressource.h */		
	if(!creation_fichier_resource_h(RESOURCE_H_FILE, &data))
    {
        printf_creation_fichier(RESOURCE_H_FILE, 0);
        return false;
    }
    
    /* MSG */
    printf_creation_fichier(RESOURCE_H_FILE, 1);

    return true;
}

bool newVersion_fonction(int versionType, const char *commentaire)
{
    /* Data */
    ResourceData data;
    if(!parseResourceFile(RESOURCE_H_FILE, &data))
    {
        printf("Lecture impossible %s newVersion_fonction\n", RESOURCE_H_FILE);
        return false;
    }

    /* char to int */
    int entier = 0;
    int decimal = 0;
    extractNumbers(data.appVersion, &entier, &decimal);

    /* int to char */
    getNewVersion(data.appVersion, versionType, entier, decimal, versionAdd);
    
    if(!add_new_changelog(CHANGELOG_FILE, data.appVersion, commentaire))
    {
        printf_new();
        return false;
    }
    
    /* MSG */
    printf_update_fichier(CHANGELOG_FILE);
    
    /* Write Ressource.h */
    if(!write_fichier_resource_h(RESOURCE_H_FILE, &data))
    {
        printf_new();
        return false;
    }

    /* MSG */
    printf_update_fichier(RESOURCE_H_FILE);
    
    if(strcmp(data.innoSetup, "TRUE") == 0)
    {
        update_innosetup(&data);
    }

    return true;
}

bool resource_fonction(const char *fileDescription)
{
    /* Data */
    ResourceData data;
    if(!parseResourceFile(RESOURCE_H_FILE, &data))
    {
        printf("Lecture impossible %s resource_fonction\n", RESOURCE_H_FILE);
        return false;
    }

    /* Création Resource.rc */
    if(!creation_fichier_resource_rc(RESOURCE_RC_FILE, fileDescription, &data))
    {
        printf_creation_fichier(RESOURCE_RC_FILE, 0);
        return false;
    }
    
    /* MSG */
    printf_creation_fichier(RESOURCE_RC_FILE, 1);

    return true;
}

bool gitignore_fonction(void)
{
    /* Creation .gitignore */
    if(!creation_fichier_gitignore(GITIGNORE_FILE))
    {
        printf_creation_fichier(GITIGNORE_FILE, 0);
        return false;
    }
    
    /* MSG */
    printf_creation_fichier(GITIGNORE_FILE, 1);
    
    return true;
}

bool innosetup_fonction(bool innosetup)
{
    /* Data */
    ResourceData data;
    if(!parseResourceFile(RESOURCE_H_FILE, &data))
    {
        printf("Lecture impossible %s innosetup_fonction\n", RESOURCE_H_FILE);
        return false;
    }

    if(!innosetup)
    {
        return true;
    }

    /* Modif Resource.h */
    snprintf(data.innoSetup, sizeof(data.innoSetup), "%s", "TRUE");
    if(!activation_innosetup(RESOURCE_H_FILE, &data))
    {
        printf_creation_fichier("Innosetup_file", -1);
        return false;
    }

    /* MSG */    
    printf_update_fichier("Innosetup_file");
    
    return true;
}

bool remove_fonction(void)
{
    /* Remove Last changelog entry  Ask yes / no */
    if(!remove_last_changelog_entry(CHANGELOG_FILE))
    {
        return true;
    }

    /* Data */
    ResourceData data;
    if(!parseResourceFile(RESOURCE_H_FILE, &data))
    {
        printf("Lecture impossible %s innosetup_fonction\n", RESOURCE_H_FILE);
        return false;
    }
    
    char *appversion = GetLastValue();
    if(!appversion)
    {
        return false;
    }

    snprintf(data.appVersion, sizeof(data.appVersion), "%s", appversion);
    free(appversion);
    
    if(!write_fichier_resource_h(RESOURCE_H_FILE, &data))
    {
        return false;
    }

    return true;
}