#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "fichier.h"
#include "fonction.h"
#include "message.h"
#include "main.h"

#ifdef _WIN32
#define OS "WINDOWS"
#elif __linux__
#define OS "LINUX"
#endif

#if defined(__GNUC__) || defined(__GNUG__)
#include <time.h>

#define _strdup strdup

char *get_date_annee(void)
{
	time_t temps;
	struct tm *tm_info;
	char year[5];

	time(&temps);
	tm_info = localtime(&temps);
	strftime(year, 5, "%Y", tm_info);

	size_t len = strlen (year) + 1;
	void *new = malloc (len);

	if (new == NULL)
	{
		return NULL;
	}

	return (char *) memcpy (new, year, len);
}

#elif defined(_MSC_VER)
#include <Windows.h>

char *get_date_annee(void)
{
	char buffer[21];
	SYSTEMTIME st = { 0 };
	size_t Longueur = 0;
	char *ChaineRetour = NULL;

	GetLocalTime(&st);

	snprintf(buffer, 20, "%02d", st.wYear);

	Longueur = strlen(buffer) + 1;
	ChaineRetour = (char*)malloc(Longueur);
	if (ChaineRetour == (char*)0)
	{
		return (char*)0;
	}

	return (char*)memcpy(ChaineRetour, buffer, Longueur);
}

#endif

/* int to double */
double GoToDecimal(int x)
{
    if(x < 10)
    {
        return (double)x / 10;
    }
    else if(x >= 10 && x < 100)
    {
        return (double)x / 100;        
    }
    else if( x > 100 && x < 1000)
    {
        return (double)x / 1000;
    }

    return -1.0;
}

void getNewVersion(char *appVersion, int versionType, int entier, int decimal)
{
    if (versionType == 0)
    {
        int newDecimal = decimal;

        newDecimal++;
        if (newDecimal >= 1000) /* Si >999 Passage à l'entier suivant */
        {
            entier++;
            snprintf(appVersion, SIZE_BUFFER, "%d.0", entier);
            return;
        }
        
        snprintf(appVersion, SIZE_BUFFER, "%d.%d", entier, newDecimal);
        return;
    }
    else if (versionType == 1)
    {
        entier++;
        snprintf(appVersion, SIZE_BUFFER, "%d.0", entier);
        return;
    }

    return;
}

void extractNumbers(const char *src, int *entier, int *decimal)
{
    if (!src || !entier || !decimal)
    {
        fprintf(stderr, "Entree invalide\n");
        return;
    }

    char bufferInterger[SIZE_BUFFER] = {0};
    char bufferDecimal[SIZE_BUFFER] = {0};

    size_t len = strlen(src);
    bool parser = false;
    size_t intIndex = 0, decIndex = 0;

    for (size_t i = 0; i < len; i++)
    {
        if (src[i] == '.')
        {
            parser = true;
            continue;
        }

        if (!parser)
        {
            if (intIndex < sizeof(bufferInterger) - 1)
            {
                bufferInterger[intIndex++] = src[i];
            }
        } else
        {
            if (decIndex < sizeof(bufferDecimal) - 1)
            {
                bufferDecimal[decIndex++] = src[i];
            }
        }
    }

    bufferInterger[intIndex] = '\0';
    bufferDecimal[decIndex] = '\0';

    *entier = atoi(bufferInterger);
    *decimal = atoi(bufferDecimal);
}

bool update_innosetup(ResourceData *data)
{
    if(!data)
	{
		return false;
	}
    
	FILE *fichier       = NULL;
	FILE *fichierTampon = NULL;
    
    char ligne[SIZE_BUFFER]     = {0};
    char fichierPath[SIZE_BUFFER] = {0};
    
    /* File Path */
    if (strlen(data->appName) > SIZE_BUFFER - 5) // 5 pour ".iss\0"
    {
        fprintf(stderr, "Le nom de l'application est trop long\n");
        return false;
    }
    snprintf(fichierPath, sizeof(fichierPath), "%s.iss", data->appName);

	/* Verif Existe */
    if (!VerifExiste(fichierPath))
	{
		fprintf(stderr, "Fichier %s introuvable\n", fichierPath);
		return false;
	}

	/* Open file */
    fichier = fopen(fichierPath, "r");
	fichierTampon = fopen(T_INNOSETUP, "w");
	if (!fichier || !fichierTampon)
	{
        fprintf(stderr, "Ouverture %s %s impossible\n", fichierPath, T_INNOSETUP);
		return false;
	}
	
	/* Read File */
    while(fgets(ligne, sizeof(ligne), fichier) != NULL)
	{
        if (strncmp(ligne, "#define MyAppVersion", 20) == 0)
		{
			fprintf(fichierTampon, "#define MyAppVersion \"%s\"\n", data->appVersion);
		}
		else
		{
			fprintf(fichierTampon, "%s", ligne);
		}
	}

	/* Close file */
    fclose(fichier);
	fclose(fichierTampon);

	/* Remove file */
    if (remove(fichierPath) != 0)
	{
		fprintf(stderr, "Supression %s impossible\n", fichierPath);
		return false;
	}

    /* Rename */
	if (rename(T_INNOSETUP, fichierPath) != 0)
	{
		fprintf(stderr, "renommage %s impossible\n", T_INNOSETUP);
		return false;
	}

	/* MSG */
    printf_update_fichier(fichierPath);
    
    return true;
}

bool activation_innosetup(const char *path, ResourceData *data)
{
	if(!data)
	{
		return false;
	}

	if(!write_fichier_resource_h(path, data))
    {
        return false;
    }

	return true;
}

bool add_new_changelog(const char *path, const char *appVersion, const char *commentaire)
{
    /* Verif Existe */
    if(!VerifExiste(path))
	{
		return false;
	} 
    
    /* Open file */
    FILE *fichier = NULL;
    fichier = fopen(path, "a");
	if(!fichier)
	{
		return false;
	}  

	/* Write DATA */
    fprintf(fichier, "BUILD %s\n%s%s;\n", appVersion, "-*- Add -*- ", commentaire);

	/* Close File */
    fclose(fichier);

	return true;
}

char *get_last_changelog(const char *path)
{
    char buffer[SIZE_BUFFER] = {0};
    FILE *fichier = NULL;    
    char *lastLine = NULL;
        
    /* Ouverture du fichier */
    fichier = fopen(path, "r");
    if(!fichier)
    {
        fprintf(stderr, "Ouverture %s impossible\n", path);
        return NULL;
    }

    /* Fichier vers buffer */
    while(fgets(buffer, SIZE_BUFFER, fichier) != NULL)
    {
        /* Cleanup */
        if (lastLine)
        {
            free(lastLine);
        }
        
        /* Copie */
        lastLine = _strdup(buffer);
    }

    /* Close fichier */
    fclose(fichier);

    /* Check */
    if (!lastLine)
    {
        return NULL;
    }
    
    /* Supprimer le saut de ligne */
    size_t len = strlen(lastLine);
    if (len > 0 && lastLine[len - 1] == '\n')
    {
        lastLine[len - 1] = '\0';
    }
    
    return lastLine;
}

bool removeLastEntry(const char *path)
{
    char buffer[SIZE_BUFFER] = {0};
    int compteur = 0;

    /* Open */
    FILE *fichier = fopen(path, "r");
    if (!fichier)
    {
        return false;
    }
    
    FILE *fichierTemp = fopen(T_CHANGELOG_FILE, "w");
    if (!fichierTemp)
    {
        fclose(fichier);
        return false;
    }

    /* Nombre de ligne */
    int nombreLigne = getNombreDeLigne(fichier);
    if(nombreLigne < 9)
    {
        fclose(fichier);
        fclose(fichierTemp);
        return false;
    }

    /* Lire le fichier et conserver les deux dernières lignes */
    while (fgets(buffer, sizeof(buffer), fichier) != NULL)
    {
        if (compteur < (nombreLigne - 2))
        {
            fprintf(fichierTemp, "%s", buffer);
        }

        compteur++;
    }

    /* Cleanup */
    fclose(fichier);
    fclose(fichierTemp);

    /* Remplacer le fichier original par le fichier temporaire */
    if (remove(path) != 0 || rename(T_CHANGELOG_FILE, path) != 0)
    {
        return false;
    }

    return true;
}

bool remove_last_changelog_entry(const char *path)
{
    /* Verif */
    if(!VerifExiste(path))
	{
        fprintf(stderr, "%s existe pas\n", path);
		return false;
	}

    /* Get Last changelog */
    char *buffer = NULL;
    buffer = get_last_changelog(path);
    if(!buffer)
    {
        return false;
    }

    /* Oui / Non */
    if(!DemandeAccordFichier(buffer, 0))
    {
        free(buffer);
        return false;
    }

    /* Cleanup */
    free(buffer);

    if(!removeLastEntry(path))
    {
        return false;
    }

    return true;
}

bool Printf__changelog(const char *path)
{
    char *buffer = NULL;

    buffer = FichierToChar(path);
    if(!buffer)
    {
        return false;
    }

    printf("%s\n", buffer);

    free(buffer);

    return true;
}
