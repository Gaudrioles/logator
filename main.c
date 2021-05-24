#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fonction.h"
#include "fichier.h"
#include "message.h"

#define CHANGELOG_FILE      "CHANGELOG.md"
#define RESOURCE_H_FILE     "resource.h"
#define RESOURCE_RC_FILE    "Resource.rc"
#define GITIGNORE_FILE      ".gitignore"


int main(int argc, char *argv[])
{
    if(argv[1] == NULL)
    {
        printf_bad_flags();
        return -1;
    }

    if(strcmp(argv[1], "-creation") == 0)
    {
        if(creation_fichier_changelog() != 0)
        {
            printf_creation_fichier(CHANGELOG_FILE, 0);
            return -1;
        }
        else if(creation_fichier_resource_h() != 0)
        {
            printf_creation_fichier(RESOURCE_H_FILE, 0);
            return -1;
        }
        else
        {
            printf_creation_fichier(CHANGELOG_FILE, 1);
            printf_creation_fichier(RESOURCE_H_FILE, 1);
        }

    }
    else if(strcmp(argv[1], "-new") == 0)
    {
        if(update_fichier_changelog(argv[2], argv[3]) != 0)
        {
            printf_new();
            return -1;
        }
        else if(update_fichier_resource_h(argv[2]) != 0)
        {
            printf_new();
            return -1;
        }
        else
        {
            printf_update_fichier(CHANGELOG_FILE);
            printf_update_fichier(RESOURCE_H_FILE);
        }

        if(innosetup_status() == 1)
        {
            update_innosetup(argv[2]);
        }

    }
    else if(strcmp(argv[1], "-resource") == 0)
    {
        if(argv[2] == NULL || argv[3] == NULL)
        {
            printf_resource();
            return -1;
        }
        if(creation_fichier_resource_rc(argv[2], argv[3]) != 0)
        {
            printf_creation_fichier(RESOURCE_RC_FILE, 0);
        }
        else
        {
            printf_creation_fichier(RESOURCE_RC_FILE, 1);
        }
    }
    else if(strcmp(argv[1], "-gitignore") == 0)
    {
        if(creation_fichier_gitignore() != 0)
        {
            printf_creation_fichier(GITIGNORE_FILE, 0);
        }
        else
        {
            printf_creation_fichier(GITIGNORE_FILE, 1);
        }
    }
    else if(strcmp(argv[1], "-help") == 0)
    {
        printf_aide();
    }
    else if(strcmp(argv[1], "-innosetup") == 0)
    {
        if(argv[2] == NULL)
        {
            printf_innosetup();
            return -1;
        }
        else
        {
            if(activation_innosetup(argv[2]) != 0)
            {
                printf_creation_fichier("Innosetup_file", -1);
                return -1;
            }
            else
            {
                printf_update_fichier("Innosetup_file");
            }
        }
    }
    else
    {
        printf_bad_flags();
        return -1;
    }

    return 0;
}
