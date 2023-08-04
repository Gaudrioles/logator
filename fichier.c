#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "log.h"

#if defined(__GNUC__) || defined(__GNUG__)
#define _strdup strdup
#endif

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
            nombre_ligne++;
        }
    } while (caractereActuel != EOF);

    fclose(fichier);

    return nombre_ligne;
}

int innosetup_status(void)
{
    FILE *fichier = NULL;

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

char *application_get_name(void)
{
    FILE* fichier = NULL;
    char buffer[SIZE_BUFFER];
    char name[SIZE_BUFFER];

    size_t len = 0;

    int compteur = 0;
    int i = 0;
    int j = 0;
    int k = 0;

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
        if(compteur == 4)
        {
            len = strlen(buffer);
            for( i =  0; i < (int)len; i++)
            {
                if(buffer[i] == '"')
                {
                    switch (j)
                    {
                    case 0:
                        j = 1;
                        i++;
                        break;
                    case 1:
                        j = 0;
                        break;
                    }
                }
                

                if(j == 1)
                {
                    name[k] = buffer[i];
                    k++;
                }
            }

            
        }
        compteur++;
    }

    name[k] = '\0';

    fclose(fichier);

    return _strdup(name);
}

double get_version(void)
{
    FILE *fichier = NULL;

    char chaine[SIZE_BUFFER] = { 0 };
    char buffer[SIZE_BUFFER] = { 0 };

    double version = 0;

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

    while(fgets(chaine, SIZE_READ, fichier) != NULL)
    {
        if(compteur == 2)
        {
            if(fscanf(fichier, "#define APP_VERSION \"%s\"", buffer) == EOF)
            {
                fprintf(stderr, "Erreur fonction get_version()\n");
                fclose(fichier);
                return -1;
            }
        }

        compteur++;
    }

    version = atof(buffer);

    version = version + 0.1;

    fclose(fichier);

    return version;
}
