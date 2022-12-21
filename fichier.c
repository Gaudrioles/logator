#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

int innosetup_status(void)
{
    FILE *fichier = NULL;

    char chaine[1025] = { 0 };
    char buffer[1025] = { 0 };

    int compteur = 0;


    fichier = fopen(RESOURCE_H_FILE, "r");

    if(fichier == NULL)
    {
        return -1;
    }

    for(compteur =0;compteur < 6;compteur++)
    {
        switch(compteur)
        {
            case 5:
                if (fscanf(fichier, "#define INNOSETUP %s", chaine) == EOF)
                {
                    printf("Erreur fonction setup_status()\n");
                }
                break;
            default:
                if(fgets(buffer, 1024, fichier) == NULL)
                {
                    return -1;
                }
                break;
        }
    }

    fclose(fichier);

    if(strcmp(chaine, "\"TRUE\"") == 0)
    {
        return 1;
    }
    else if(strcmp(chaine, "\"FALSE\"") == 0)
    {
        return 0;
    }

    return 0;
}

char *application_get_name(void)
{
    FILE *fichier = NULL;

    char chaine[1025] = { 0 };
    char buffer[1025] = { 0 };

    size_t len = 0;

    int compteur = 0;

    fichier = fopen("resource.h", "r");

    if(fichier == NULL)
    {
        return NULL;
    }

    for(compteur = 0; compteur < 5 ;compteur++)
    {
        switch(compteur)
        {
            case 4:
                if(fgets(buffer, 1024, fichier) == NULL)
                {
                    return NULL;
                }
                break;
            default:
                if(fgets(buffer, 1024, fichier) == NULL)
                {
                    return NULL;
                }
                break;
        }
    }

    fclose(fichier);

    len = strlen(buffer) - 2;
    int i = len;

    for(compteur = 18; compteur < i; compteur++)
    {
        chaine[compteur - 18] = buffer[compteur];
    }

    len = strlen (chaine) + 1;
    char *result = (char*) malloc (len);

    if (result == NULL)
    {
        return NULL;
    }

    return (char*) memcpy (result, chaine, len);
}

double get_version(void)
{
    FILE *fichier = NULL;

    char chaine[1025] = { 0 };
    char buffer[1025] = { 0 };

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
                if (fscanf(fichier, "#define APP_VERSION \"%s\"", buffer) == EOF)
                {
                    printf("Erreur fonction get_version()\n");
                }
                break;
            default:
                if(fgets(chaine, 1024, fichier) == NULL)
                {
                    return 0;
                }
                break;
        }
    }

    version = atof(buffer);

    version = version + 0.1;

    fclose(fichier);

    return version;
}