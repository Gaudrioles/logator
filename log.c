
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
#include <sys/stat.h>
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

int DemandeAccord(char* FichierNom)
{
    printf("Le fichier %s existe, voulez-vous le remplacer ? [O]ui / [N]on : ", FichierNom);
    
    int caractereActuel = getchar();

    if(caractereActuel == 'Y' || caractereActuel == 'y'|| caractereActuel == 'O'|| caractereActuel == 'o')
    {
        rewind(stdin);
        return 1;
    }
    else if(caractereActuel == 'N'|| caractereActuel == 'n')
    {
        rewind(stdin);
        return 0;
    }
    else
    {
        rewind(stdin);
        printf("\n");
        return -1;
    }
}

int VerifAccord(char* FichierNom)
{
    int compteur = 0;
    int i;
    while (compteur < 3)
    {
        i = DemandeAccord(FichierNom);
        switch (i)
        {
        case -1:
            break;
        default:
            compteur = 3;
            break;
        }
        compteur++;
    }

	return i;
}

size_t TailleTampon(const char* FichierNom)
{
    FILE* fichier = NULL;
    size_t compteur = 0;

    fichier = fopen(FichierNom, "r");

    if (fichier == NULL)
    {
        fprintf(stderr, "Lecture du fichier impossible %s\n", FichierNom);
        return 0;
    }

    while (fgetc(fichier) != EOF)
    {
        compteur++;
    }

    fclose(fichier);

    return compteur;
}

char* FichierToChar(const char* FichierNom)
{
    FILE* fichier = NULL;
    size_t taille = TailleTampon(FichierNom);
    int compteur = 0;
    int caractereActuel = 0;

    if (taille == 0)
    {
        return NULL;
    }

    fichier =  fopen(FichierNom, "r");

    if (fichier == NULL)
    {
        fprintf(stderr, "Lecture du fichier impossible %s\n", FichierNom);
        return NULL;
    }

    char* chaine = malloc(sizeof(char) * (taille + 1));
    if (chaine == NULL)
    {
        fprintf(stderr, "Allocation memoire impossible FichierToChar %s\n", FichierNom);
        return NULL;
    }

    while((caractereActuel = fgetc(fichier)) != EOF)
    {
        chaine[compteur++] = caractereActuel;
    }
    chaine[compteur] = '\0';

    fclose(fichier);


    fclose(fichier);
    
    return chaine;
}