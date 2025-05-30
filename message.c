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
    fprintf(stdout, "\t*                logator -creation \"AppName\"                                                *\n");
    fprintf(stdout, "\t*                                                                                           *\n");
    fprintf(stdout, "\t*         -> Ajout de nouvelle version :                                                    *\n");
    fprintf(stdout, "\t*                logator -new -Major/-minor \"commentaire\"                                   *\n");
    fprintf(stdout, "\t*                                                                                           *\n");
    fprintf(stdout, "\t*         -> Creation fichier Resource.rc                                                   *\n");
    fprintf(stdout, "\t*                logator -resource \"FileDescription\"                                        *\n");
    fprintf(stdout, "\t*                                                                                           *\n");
    fprintf(stdout, "\t*         -> Creation fichier .gitignore :                                                  *\n");
    fprintf(stdout, "\t*                logator -gitignore                                                         *\n");
    fprintf(stdout, "\t*                                                                                           *\n");
    fprintf(stdout, "\t*         -> Activation innosetup :                                                         *\n");
    fprintf(stdout, "\t*                logator -innosetup \"true\" || \"false\"                                       *\n");
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

void printf_err_msg(void)
{
    fprintf(stdout, "-- !! -- Erreur -- !! --\n");
    return;
}

void printf_new(void)
{
    printf_err_msg();
    fprintf(stdout, "logator -new -Major/-minor \"commentaire\"\n");

    return;
}

void printf_update_fichier(char *fichier)
{
    fprintf(stdout, "Mise a jour du fichier %s\n", fichier);

    return;
}

void printf_innosetup(void)
{
    printf_err_msg();
    fprintf(stdout, "logator -innosetup \"true\" || \"false\"\n");

    return;
}

void printf_bad_flags(void)
{
     fprintf(stdout, "\nLogator %s (x64) : Copyright (c) 2024 Bubi GAUDRIOLES : 18-02-2024\n\n", APP_VERSION);
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
    printf_err_msg();
    fprintf(stdout, "logator -resource \"FileDescription\"\n");

    return;
}

void printf_fonction_creation(void)
{
    printf_err_msg();
    fprintf(stdout, "logator -creation \"AppName\"\n");
    return;
}

void printf_msg_empty(void)
{
    fprintf(stdout, "Version initiale Suppression impossible\n");
    return;
}

void printf_msg_changelog(const char* file)
{
    fprintf(stdout, "Ouverture du fichier %s impossible\n", file);
    return;
}
