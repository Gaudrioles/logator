#include <stdlib.h>
#include <stdio.h>

#include "resource.h"

#ifdef __unix__

void printf_aide()
{
   fprintf(stdout, "\n\n ==========================================================================\n");
   fprintf(stdout, " *                                                                        *\n");
   fprintf(stdout, " *         -> Liste des commandes                                         *\n");
   fprintf(stdout, " *                logator -help                                           *\n");
   fprintf(stdout, " *                                                                        *\n");
   fprintf(stdout, " *         -> Creation fichier CHANGELOG.md et resource.h :               *\n");
   fprintf(stdout, " *                logator -creation                                       *\n");
   fprintf(stdout, " *                                                                        *\n");
   fprintf(stdout, " *         -> Ajout de nouvelle version :                                 *\n");
   fprintf(stdout, " *                logator -new \"commentaire\"                              *\n");
   fprintf(stdout, " *                                                                        *\n");
   fprintf(stdout, " *         -> Creation fichier Resource.rc                                *\n");
   fprintf(stdout, " *                logator -resource \"FileDescription\"  \"ProductName\"      *\n");
   fprintf(stdout, " *                                                                        *\n");
   fprintf(stdout, " *         -> Creation fichier .gitignore :                               *\n");
   fprintf(stdout, " *                logator -gitignore                                      *\n");
   fprintf(stdout, " *                                                                        *\n");
   fprintf(stdout, " *         -> Activation innosetup :                                      *\n");
   fprintf(stdout, " *                logator -innosetup \"TRUE\"                               *\n");
   fprintf(stdout, " *                                                                        *\n");
   fprintf(stdout, " *         -> Suppression entree CHANGELOG :                              *\n");
   fprintf(stdout, " *                logator -remove                                         *\n");
   fprintf(stdout, " *                                                                        *\n");
   fprintf(stdout, " ==========================================================================\n");

    return;
}

#elif defined(_WIN32) || defined(WIN32)

void printf_aide()
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
   fprintf(stdout, "\t*         -> Suppression entree CHANGELOG :                                                 *\n");
   fprintf(stdout, "\t*                logator -remove                                                            *\n");
   fprintf(stdout, "\t*                                                                                           *\n");
   fprintf(stdout, "\t=============================================================================================\n");

    return;
}

#endif



void printf_new()
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

void printf_innosetup()
{
   fprintf(stdout, "-- !! -- Erreur -- !! --\n");
   fprintf(stdout, "logator -innosetup \"TRUE\"\n");

    return;
}

void printf_bad_flags()
{
    fprintf(stdout, "\nLogator %s (x64) : Copyright (c) 2021 Bubi GAUDRIOLES : 14-02-2022\n\n", APP_VERSION);
    fprintf(stdout, "Usage: logator -help [Liste des commandes]\n");

    return;
}

void printf_creation_fichier(char *fichier, int oui)
{
    switch (oui)
    {
    case 1:
       fprintf(stdout, "Creation du fichier %s\n", fichier);
        break;
    case 0:
       fprintf(stdout, "Creation impossible %s\n", fichier);
        break;
    case -1:
       fprintf(stdout, "Modification du fichier %s impossible\n", fichier);
        break;

    default:
        break;
    }

    return;
}

void printf_resource()
{
   fprintf(stdout, "-- !! -- Erreur -- !! --\n");
   fprintf(stdout, "logator -resource \"FileDescription\"  \"ProductName\"\n");

    return;
}
