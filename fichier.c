#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "main.h"

int nombre_de_ligne(char *fichier_nom)
{
    FILE *fichier = NULL;
    int nombre_ligne = 0;
    int caractereActuel = 0;

    fichier = fopen(fichier_nom, "r");

    if(fichier == NULL)
    {
        return -1;
    }

    do
    {
        caractereActuel = fgetc(fichier);
        if(caractereActuel == '\n')
        {
            nombre_ligne = nombre_ligne + 1;
        }
    } while (caractereActuel != EOF);

    fclose(fichier);

    return nombre_ligne;
}

int innosetup_status()
{
    FILE *fichier = NULL;

    char chaine[1025] = "";
    char buffer[1025] = "";

    int compteur = 0;


    fichier = fopen("resource.h", "r");

    if(fichier == NULL)
    {
        return -1;
    }

    for(compteur =0;compteur < 6;compteur++)
    {
        switch(compteur)
        {
            case 5:
                fscanf(fichier,"#define INNOSETUP %s", chaine);
                break;
            default:
                fgets(buffer, 1024, fichier);
                break;
        }
    }

    fclose(fichier);

    char detecteur[] = "\"\"";
    char *chaine_remove = strtok(chaine, detecteur);

    if(strcmp(chaine_remove, "TRUE") == 0)
    {
        return 1;
    }
    else if(strcmp(chaine_remove, "FALSE") == 0)
    {
        return 0;
    }

    return 0;
}

char *application_get_name()
{
    FILE *fichier = NULL;

    char chaine[1025] = "";
    char buffer[1025] = "";

    int compteur = 0;

    fichier = fopen("resource.h", "r");

    if(fichier == NULL)
    {
        return NULL;
    }

    for(compteur =0;compteur < 6;compteur++)
    {
        switch(compteur)
        {
            case 4:
                fscanf(fichier,"#define APP_NAME %s", chaine);
                break;
            default:
                fgets(buffer, 1024, fichier);
                break;
        }
    }

    fclose(fichier);

    size_t len = strlen (chaine) + 1;
    char *result = (char*) malloc (len);

    return (char*) memcpy (result, chaine, len);
}

double get_version()
{
    FILE *fichier = NULL;

    char chaine[1025] = "";
    char buffer[1025] = "";

    double version = 0;

    int compteur = 0;

    fichier = fopen(RESOURCE_H_FILE, "r");

    if(fichier == NULL)
    {
        return 0;
    }

    for(compteur =0;compteur < 6;compteur++)
    {
        switch(compteur)
        {
            case 3:
                fscanf(fichier,"#define APP_VERSION \"%s\"", buffer);
                break;
            default:
                fgets(chaine, 1024, fichier);
                break;
        }
    }

    version = atof(buffer);

    version = version + 0.1;

    fclose(fichier);

    return version;
}

int verif_fichier_existe(char *fichier_nom)
{
    if(access(fichier_nom, F_OK) == 0)
    {
        return 1;
    }
    else
    {
        printf("Le fichier %s est introuvable\n", fichier_nom);
        return 0;
    }
    return 0;
}