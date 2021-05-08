#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fonction.h"
#include "fichier.h"

int main(int argc, char *argv[])
{
    if(argv[1] == NULL)
    {
        printf("Mauvais argument\n");
        return -1;
    }

    if(strcmp(argv[1], "-creation") == 0)
    {
        if(creation_fichier_changelog() != 0)
        {
            printf("Creation impossible\n");
            return -1;
        }
        else if(creation_fichier_resource_h() != 0)
        {
            printf("Creation impossible\n");
            return -1;
        }
        else
        {
            printf("Creation du fichier CHANGELOG.md\n");
            printf("Creation du fichier resource.h\n");
        }

    }
    else if(strcmp(argv[1], "-new") == 0)
    {
        if(update_fichier_changelog(argv[2], argv[3]) != 0)
        {
            printf("Ajout impossible\n");
            return -1;
        }
        else if(update_fichier_resource_h(argv[2]) != 0)
        {
            printf("Ajout impossible\n");
            return -1;
        }
        else
        {
            printf("Mise a jour du fichier CHANGELOG.md\n");
            printf("Mise a jour du fichier resource.h\n");
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
            printf("Mauvais argument\n Fonction help -> logator.exe -help\n");
            return -1;
        }
        if(creation_fichier_resource_rc(argv[2], argv[3]) != 0)
        {
            printf("Creation du fichier Resource.rc impossible\n");
        }
        else
        {
            printf("Creation du fichier Resource.rc\n");
        }
    }
    else if(strcmp(argv[1], "-gitignore") == 0)
    {
        if(creation_fichier_gitignore() != 0)
        {
            printf("Creation du fichier .gitignore impossible\n");
        }
        else
        {
            printf("Creation du fichier .gitignore\n");
        }
    }
    else if(strcmp(argv[1], "-help") == 0)
    {
        fonction_aide();
    }
    else if(strcmp(argv[1], "-innosetup") == 0)
    {
        if(argv[2] == NULL)
        {
            printf("Mauvais argument\n Fonction help -> logator.exe -help\n");
            return -1;
        }
        else
        {
            if(activation_innosetup(argv[2]) != 0)
            {
                printf("activation innosetup impossible\n");
                return -1;
            }
            else
            {
                printf("update innosetup\n");
            }
        }
    }
    else
    {
        printf("Mauvais argument\n Fonction help -> logator.exe -help\n");
        return -1;
    }

    return 0;
}
