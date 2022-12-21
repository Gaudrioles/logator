
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "log.h"

#ifdef _WIN32
#include <direct.h>
#include <io.h>
#define F_OK 0
#define access _access

    int CreationRepertoire(char* repertoire)
    {
        if(_mkdir(repertoire) != -1)
        {
            return 0;
        }
        
        return -1;
    }

#elif __linux__
#include <unistd.h>
    int CreationRepertoire(char* repertoire)
    {
        if(mkdir(repertoire, 0755) != -1)
        {
            return 0;
        }

        return -1;
    }
#endif

char* GetTime(void)
{
    time_t timestamp = time(NULL);
    struct tm* pTime = localtime(&timestamp);

    char buffer[80];
    strftime(buffer, 80, "%d/%m/%Y %H:%M:%S", pTime);

    size_t Longueur = 0;
    char* ChaineRetour = NULL;

    Longueur = strlen(buffer) + 1;
    ChaineRetour = (char*)malloc(Longueur);
    if (ChaineRetour == (char*)0)
    {
        return (char*)0;
    }

    return (char*)memcpy(ChaineRetour, buffer, Longueur);
}

int VerifExiste(char* emplacement)
{
    if (access(emplacement, F_OK) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int CreationFichierLog(char* log_texte)
{
    char* date = NULL;
    FILE* fichier = NULL;

    date = GetTime();

    if (VerifExiste(log_folder) != 1)
    {
        if(CreationRepertoire(log_folder) != 0)
        {
            free(date);
            return -1;
        }
    }

    if (VerifExiste(log_file) != 1)
    {
        fichier = fopen(log_file, "w+");
        if (fichier != NULL)
        {
            fprintf(fichier, "---------------------------------------------\n-----        Système de Log File        -----\n-----             Version %s           -----\n---------------------------------------------\n\n", LOG_VERSION);
            fprintf(fichier, "*********************************************\n\n");
            fprintf(fichier, "Date \t\t[%s]\n\n", date);
            fprintf(fichier, "Message ->\t%s\n\n", log_texte);
            fclose(fichier);
            free(date);
            return 0;
        }
        else
        {
            free(date);
            return -1;
        }
    }
    else
    {
        fichier = fopen(log_file, "a+");
        if (fichier != NULL)
        {
            fprintf(fichier, "*********************************************\n\n");
            fprintf(fichier, "Date \t\t[%s]\n\n", date);
            fprintf(fichier, "Message ->\t%s\n\n", log_texte);
            fclose(fichier);
            free(date);
            return 0;
        }
        else
        {
            free(date);
            return -1;
        }
    }
}

int VerifAccord(char *message)
{
    char approbation[2];
    int  compteur = 0;


    while(compteur < 3)
    {
        fprintf(stdout, "%s ? [O]ui / [N]on : ", message);
        scanf("%1s", approbation);
        if(strcmp(approbation,"Y") == 0 || strcmp(approbation,"y") == 0 ||  strcmp(approbation,"O") == 0 || strcmp(approbation,"o") == 0)
        {
            return 1;
        }
        else if(strcmp(approbation,"N") == 0 || strcmp(approbation,"n") == 0)
        {
            return 0;
        }

        compteur++;
    }

    return -1;    
}