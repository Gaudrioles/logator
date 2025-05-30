#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "fichier.h"
#include "fonction.h"
#include "message.h"
#include "main.h"

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

int nombreDeLigne(FILE *fichier)
{
    if (fichier == NULL)
    {
        return 0;
    }

    int ret = 0;
    char buffer[SIZE_BUFFER]; // Taille de buffer ajustable pour optimiser les lectures

    // Lire le fichier par blocs pour réduire les appels à fgetc
    while (fgets(buffer, sizeof(buffer), fichier) != NULL)
    {
        for (char *p = buffer; *p != '\0'; p++)
        {
            if (*p == '\n')
            {
                ret++;
            }
        }
    }

    return ret;
}

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

void getNewVersion(char *appVersion, int versionType, int entier, int decimal, VersionModif modif)
{
    if (versionType == 0) // Gestion avec partie décimale
    {
        int newDecimal = decimal;

        if (modif == versionAdd)
        {
            newDecimal++;
            if (newDecimal >= 1000) // Passage à l'entier suivant
            {
                entier++;
                snprintf(appVersion, SIZE_BUFFER, "%d.0", entier);
                return;
            }
        }
        else // VersionModif == versionRem
        {
            if (decimal == 0)
            {
                newDecimal = 999; // Retourne à 999 dans la partie décimale
                entier--;
            }
            else
            {
                newDecimal--;
            }
        }
        
        snprintf(appVersion, SIZE_BUFFER, "%d.%d", entier, newDecimal);
        return;
    }
    else if (versionType == 1) // Gestion entière sans partie décimale
    {
        switch (modif)
        {
            case versionAdd:
                entier++;
                snprintf(appVersion, SIZE_BUFFER, "%d.0", entier);
                break;
            case versionRem:
                entier--;
                snprintf(appVersion, SIZE_BUFFER, "%d.0", entier);
                break;
            default:
                break;
        }
        
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
    
    char chaine[SIZE_BUFFER]     = {0};
    char fichierPath[SIZE_BUFFER] = {0};

	int compteur = 0;
    
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
	fichierTampon = fopen("update_innosetup.old", "w");
	if (!fichier || !fichierTampon)
	{
        fprintf(stderr, "Ouverture %s %s impossible\n", fichierPath, "update_innosetup.old");
		return false;
	}
	
	/* Read File */
    while(fgets(chaine, sizeof(chaine), fichier) != NULL)
	{
		if(compteur == 4)
		{
			fprintf(fichierTampon, "#define MyAppVersion \"%s\"\n", data->appVersion);
		}
		else
		{
			fprintf(fichierTampon, "%s", chaine);
		}

		compteur++;
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
	if (rename("update_innosetup.old", fichierPath) != 0)
	{
		fprintf(stderr, "renommage update_innosetup.old impossible\n");
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

bool changelog_file_to_char_cut(const char *path)
{
    FILE *fichier = NULL;
    int nbligne = 0;

    /* Ouverture du fichier */
    fichier = fopen(path, "r");
    if (fichier == NULL)
    {
        return false;
    }

    /* Comptage des lignes et allocation du buffer */
    char **bufferArray = malloc(SIZE_BUFFER * sizeof(char*));
    if (!bufferArray)
    {
        fclose(fichier);
        return false; // Erreur d'allocation mémoire
    }

    char buffer[SIZE_BUFFER];
    
    // Lire les lignes et compter les lignes du fichier
    while (fgets(buffer, sizeof(buffer), fichier) != NULL)
    {
        if (nbligne >= SIZE_BUFFER) // Limiter le nombre de lignes dans le buffer
        {
            bufferArray = realloc(bufferArray, (nbligne + 1) * sizeof(char*));
            if (!bufferArray)
            {
                fclose(fichier);
                return false; // Erreur d'allocation mémoire
            }
        }
        bufferArray[nbligne] = _strdup(buffer);  // Copier la ligne dans le tableau
        if (!bufferArray[nbligne])
        {
            fclose(fichier);
            return false; // Erreur d'allocation mémoire
        }
        nbligne++;
    }

    fclose(fichier);

    // Vérification du nombre de lignes
    if (nbligne < 9)
    {
        // Libérer la mémoire avant de quitter
        for (int i = 0; i < nbligne; i++)
        {
            free(bufferArray[i]);
        }
        free(bufferArray);
        return false;
    }

    /* Réécriture du fichier sans les 2 dernières lignes */
    fichier = fopen(path, "w+");
    if (!fichier)
    {
        fprintf(stderr, "Ouverture impossible du fichier %s\n", path);
        // Libérer la mémoire avant de quitter
        for (int i = 0; i < nbligne; i++)
        {
            free(bufferArray[i]);
        }
        free(bufferArray);
        return false;
    }

    // Réécrire les lignes, en excluant les 2 dernières
    for (int i = 0; i < (nbligne - 2); i++)
    {
        fprintf(fichier, "%s", bufferArray[i]);
    }

    fclose(fichier);

    // Libération de la mémoire
    for (int i = 0; i < nbligne; i++)
    {
        free(bufferArray[i]);
    }
    free(bufferArray);

    return true;
}


bool remove_last_changelog_entry(const char *path)
{
    char *buffer = NULL;
	
    if(VerifExiste(path) != true)
	{
		return false;
	}

    buffer = get_last_changelog(path);
    if(!buffer)
    {
        return false;
    }

    if(!DemandeAccordFichier(buffer, 0))
    {
        free(buffer);
        return false;
    }

    free(buffer);

    if(!changelog_file_to_char_cut(path))
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
