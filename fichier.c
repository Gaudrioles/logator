#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "log.h"

#if defined(__GNUC__) || defined(__GNUG__)
#define _strdup strdup
#endif

int nombre_de_ligne(char* fichierNom)
{
    FILE* fichier = NULL;
    int nombre_ligne = 0;
    int caractereActuel = 0;

    fichier = fopen(fichierNom, "r");

    if(fichier == NULL)
    {
        return -1;
    }

    do
    {
        caractereActuel = fgetc(fichier);
        if(caractereActuel == '\n')
        {
            nombre_ligne++;
        }
    } while (caractereActuel != EOF);

    fclose(fichier);

    return nombre_ligne;
}

int innosetup_status(void)
{
    FILE* fichier = NULL;

    char chaine[SIZE_BUFFER] = { 0 };
    char buffer[SIZE_BUFFER] = { 0 };

    int compteur = 0;

    if(VerifExiste(RESOURCE_H_FILE) != 1)
    {
        fprintf(stderr, "Le fichier %s existe pas\n", RESOURCE_H_FILE);
        return -1;
    }

    fichier = fopen(RESOURCE_H_FILE, "r");

    if(fichier == NULL)
    {
        return -1;
    }

    while(fgets(buffer, SIZE_READ, fichier) != NULL)
    {
        if(compteur == 4)
        {
            if (fscanf(fichier, "#define INNOSETUP %s", chaine) == EOF)
            {
                fprintf(stderr, "Erreur fonction setup_status()\n");
                fclose(fichier);
                return -1;
            }

            break;
        }

        compteur++;
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

char* application_get_name(void)
{
    FILE* fichier = NULL;
    char buffer[SIZE_BUFFER];
    char name[SIZE_BUFFER];

    size_t len = 0;

    int compteur = 0;

    if(VerifExiste(RESOURCE_H_FILE) != 1)
    {
        fprintf(stderr, "Le fichier %s existe pas\n", RESOURCE_H_FILE);
        return NULL;
    }

    fichier = fopen(RESOURCE_H_FILE, "r");
    if(fichier == NULL)
    {
        fprintf(stderr, "Lecture impossible %s\n", RESOURCE_H_FILE);
    }

    while(fgets(buffer, SIZE_READ, fichier) != NULL)
    {
        if(compteur == 3)
        {
            if(fscanf(fichier, "#define APP_NAME \"%s\"", name) == EOF)
            {
                fprintf(stderr, "Erreur fonction getVersion()\n");
                fclose(fichier);
                return NULL;
            }

            break;
        }

        compteur++;
    }

    len = strlen(name);

    name[len -1] = '\0';

    fclose(fichier);

    return _strdup(name);
}

float get_version(void)
{
    FILE* fichier = NULL;
    char buffer[SIZE_BUFFER] = { 0 };

    float version = 0.0f;
    int compteur = 0;

    if(VerifExiste(RESOURCE_H_FILE) != 1)
    {
        fprintf(stderr, "Le fichier %s existe pas\n", RESOURCE_H_FILE);
        return -1;
    }

    fichier = fopen(RESOURCE_H_FILE, "r");

    if(fichier == NULL)
    {
        return -1;
    }
    
    while(fgets(buffer, SIZE_READ, fichier) != NULL)
    {
        if(compteur == 2)
        {
            if(fscanf(fichier, "#define APP_VERSION \"%f\"", &version) == EOF)
            {
                fprintf(stderr, "Erreur fonction getVersion()\n");
                fclose(fichier);
                return -1;
            }

            break;
        }

        compteur++;
    }

    fclose(fichier);

    version = version + 0.1f;
    
    return version;
}

