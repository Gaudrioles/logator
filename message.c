#include <stdlib.h>
#include <stdio.h>

#include "resource.h"

void printf_aide(void)
{
    fprintf(stdout, "\n\n\t=============================================================================================\n");
    fprintf(stdout, "\t*                                                                                           *\n");
    fprintf(stdout, "\t*         -> Liste des commandes                                                            *\n");
    fprintf(stdout, "\t*                logator -help                                                              *\n");
    fprintf(stdout, "\t*                                                                                           *\n");
    fprintf(stdout, "\t*         -> Creation fichier CHANGELOG.md et resource.h :                                  *\n");
    fprintf(stdout, "\t*                logator -creation                                                          *\n");
    fprintf(stdout, "\t*                                                                                           *\n");
    fprintf(stdout, "\t*         -> Ajout de nouvelle version :                                                    *\n");
    fprintf(stdout, "\t*                logator -new \"commentaire\"                                                 *\n");
    fprintf(stdout, "\t*                                                                                           *\n");
    fprintf(stdout, "\t*         -> Creation fichier Resource.rc                                                   *\n");
    fprintf(stdout, "\t*                logator -resource \"FileDescription\"  \"ProductName\"                         *\n");
    fprintf(stdout, "\t*                                                                                           *\n");
    fprintf(stdout, "\t*         -> Creation fichier .gitignore :                                                  *\n");
    fprintf(stdout, "\t*                logator -gitignore                                                         *\n");
    fprintf(stdout, "\t*                                                                                           *\n");
    fprintf(stdout, "\t*         -> Activation innosetup :                                                         *\n");
    fprintf(stdout, "\t*                logator -innosetup \"TRUE\"                                                  *\n");
    fprintf(stdout, "\t*                                                                                           *\n");
    fprintf(stdout, "\t*         -> Lecture du fichier CHANGELOG :                                                 *\n");
    fprintf(stdout, "\t*                logator -view                                                              *\n");
    fprintf(stdout, "\t*                                                                                           *\n");
    fprintf(stdout, "\t*         -> Suppression entree CHANGELOG :                                                 *\n");
    fprintf(stdout, "\t*                logator -remove                                                            *\n");
    fprintf(stdout, "\t*                                                                                           *\n");
    fprintf(stdout, "\t=============================================================================================\n");

    return;
}



void printf_new(void)
{
    fprintf(stdout, "-- !! -- Erreur -- !! --\n");
    fprintf(stdout, "logator -new \"commentaire\"\n");

    return;
}

void printf_update_fichier(char *fichier)
{
    fprintf(stdout, "Mise a jour du fichier %s\n", fichier);

    return;
}

void printf_innosetup(void)
{
    fprintf(stdout, "-- !! -- Erreur -- !! --\n");
    fprintf(stdout, "logator -innosetup \"TRUE\"\n");

    return;
}

void printf_bad_flags(void)
{
     fprintf(stdout, "\nLogator %s (x64) : Copyright (c) 2021 Bubi GAUDRIOLES : 14-02-2022\n\n", APP_VERSION);
     fprintf(stdout, "Usage: logator -help [Liste des commandes]\n");

    return;
}

void printf_creation_fichier(char *fichier, int oui)
{
    switch (oui)
    {
    case 0:
        fprintf(stdout, "Creation impossible %s\n", fichier);
        break;
    case 1:
        fprintf(stdout, "Creation du fichier %s\n", fichier);
        break;
    case -1:
        fprintf(stdout, "Modification du fichier %s impossible\n", fichier);
        break;

    default:
        break;
    }

    return;
}

void printf_resource(void)
{
    fprintf(stdout, "-- !! -- Erreur -- !! --\n");
    fprintf(stdout, "logator -resource \"FileDescription\"  \"ProductName\"\n");

    return;
}
