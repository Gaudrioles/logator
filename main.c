#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fonction.h"

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
        else if(creation_fichier_ressource_h() != 0)
        {
            printf("Creation impossible\n");
            return -1;
        }
        else
        {
            printf("Creation du fichier CHANGELOG.md\n");
            printf("Creation du fichier ressource.h\n");
        }

    }
    else if(strcmp(argv[1], "-new") == 0)
    {
        if(update_fichier_changelog(argv[2], argv[3]) != 0)
        {
            printf("Ajout impossible\n");
            return -1;
        }
        else if(update_fichier_ressource_h(argv[2]) != 0)
        {
            printf("Ajout impossible\n");
            return -1;
        }
        else
        {
            printf("Mise a jour du fichier CHANGELOG.md\n");
            printf("Mise a jour du fichier ressource.h\n");
        }


    }
    else if(strcmp(argv[1], "-resource") == 0)
    {
        if(creation_fichier_ressource_rc(argv[2], argv[3], argv[4]) != 0)
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
    else
    {
        printf("Mauvais argument -> logator.exe -help\n");
        return -1;
    }

    return 0;
}
