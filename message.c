#include <stdlib.h>
#include <stdio.h>

void printf_aide()
{
    printf("\n\n\t*********************************************************************************************\n");
    printf("\t*                                                                                           *\n");
    printf("\t*         -> Liste des commandes                                                            *\n");
    printf("\t*                logator -help                                                              *\n");
    printf("\t*                                                                                           *\n");
    printf("\t*         -> Creation fichier CHANGELOG.md et resource.h :                                  *\n");
    printf("\t*                logator -creation                                                          *\n");
    printf("\t*                                                                                           *\n");
    printf("\t*         -> Ajout de nouvelle version :                                                    *\n");
    printf("\t*                logator -new \"version\" \"commentaire\"                                       *\n");
    printf("\t*                                                                                           *\n");
    printf("\t*         -> Creation fichier Resource.rc                                                   *\n");
    printf("\t*                logator -resource \"FileDescription\"  \"ProductName\"                         *\n");
    printf("\t*                                                                                           *\n");
    printf("\t*         -> Creation fichier .gitignore :                                                  *\n");
    printf("\t*                logator -gitignore                                                         *\n");
    printf("\t*                                                                                           *\n");
    printf("\t*         -> Activation innosetup :                                                         *\n");
    printf("\t*                logator -innosetup \"TRUE\"                                                  *\n");
    printf("\t*                                                                                           *\n");
    printf("\t*********************************************************************************************\n");

    return;
}

void printf_new()
{
    printf("-new mauvais commentaire\n");
    printf("logator -new \"version\" \"commentaire\"\n");

    return;
}

void printf_update_fichier(char *fichier)
{
    printf("Mise a jour du fichier %s\n", fichier);
    
    return;
}

void printf_innosetup()
{
    printf("-innosetup mauvais commentaire\n");
    printf("logator -innosetup \"TRUE\"\n");

    return;
}

void printf_bad_flags()
{
    printf("Mauvais argument\n Fonction help -> logator.exe -help\n");

    return;
}

void printf_creation_fichier(char *fichier, int oui)
{
    switch (oui)
    {
    case 1:
        printf("Creation du fichier %s\n", fichier);
        break;
    case 0:
        printf("Creation impossible %s\n", fichier);
        break;
    case -1:
        printf("Modification du fichier %s impossible\n", fichier);
        break;
    
    default:
        break;
    }

    return;
}

void printf_resource()
{
    printf("-resource mauvais commentaire");
    printf("logator -resource \"FileDescription\"  \"ProductName\"\n");

    return;
}

void printf_innosetup()
{
    printf("-innosetup mauvais commentaire");
    printf("logator -innosetup \"TRUE\"\n");

    return;
}