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

char* get_date_annee(void)
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

char* get_date_annee(void)
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

// Renvoie la partie entière d'un nombre
int GetWholePart(double x)
{
    return (int)x;
}

// Renvoie la partie décimale d'un nombre en tant qu'entier
int GetDecimalPart(double x)
{
    double partie_decimale = x - (int)x;
    return (int)(partie_decimale * 1000 + 0.5);
}

// Convertit un entier représentant la partie décimale en une valeur décimale
double GoToDecimal(int x)
{
    return (double)x / 1000;
}

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

double newVersion(int versionType, double OldVersion)
{
    int partie_entiere = GetWholePart(OldVersion);
    int partie_decimale = GetDecimalPart(OldVersion);

    if (versionType == 0)
    {
        return partie_entiere + GoToDecimal(partie_decimale + 1);
    }
    if (versionType == 1)
    {
        return partie_entiere + 1;
    }
    
    return 0;
}

double RemoveVersion(int versionType, double OldVersion)
{
    int partie_entiere = GetWholePart(OldVersion);
    int partie_decimale = GetDecimalPart(OldVersion);

    if (versionType == 0)
    {
        partie_decimale--;
        if(partie_decimale == -1)
        {
            partie_entiere--;
            partie_decimale = 999;
        }

        return partie_entiere + GoToDecimal(partie_decimale);
    }
    if (versionType == 1)
    {
        return partie_entiere - 1;
    }
    
    return 0;
}

bool update_innosetup(ST_logator* st)
{
    if(!st)
	{
		return false;
	}
	FILE* fichier = NULL;
	FILE* fichierTampon = NULL;

	char* FichierNom = NULL;
	char chaine[SIZE_BUFFER];

	int compteur = 0;
	size_t len = 0;

	if(!st)
	{
		return false;
	}

	len = strlen(st->AppName) + strlen(".iss") + 1;

	FichierNom = malloc(len * sizeof(char));
	if(!FichierNom)
	{
		return false;
	}	

	sprintf(FichierNom, "%s.iss", st->AppName);

	if (VerifExiste(FichierNom) != true)
	{
		printf("Fichier %s introuvable\n", FichierNom);
		free(FichierNom);
		return false;
	}

	fichier = fopen(FichierNom, "r");
	fichierTampon = fopen("update_innosetup.old", "w");
	if (fichier == NULL || fichierTampon == NULL)
	{
		free(FichierNom);
		return -1;
	}
	
	while(fgets(chaine, sizeof(chaine), fichier) != NULL)
	{
		if(compteur == 4)
		{
			fprintf(fichierTampon, "#define MyAppVersion \"%.1f\"\n", st->AppVersion);
		}
		else
		{
			fprintf(fichierTampon, "%s", chaine);
		}

		compteur++;
	}

	fclose(fichier);
	fclose(fichierTampon);

	if (remove(FichierNom) != 0)
	{
		free(FichierNom);
		return false;
	}

	if (rename("update_innosetup.old", FichierNom) != 0)
	{
		free(FichierNom);
		return false;
	}

	printf_update_fichier(FichierNom);

	free(FichierNom);

	return true;
}

bool activation_innosetup(const char* path, ST_logator* st)
{
	if(!st)
	{
		return false;
	}

	write_fichier_resource_h(path, st);

	return true;
}

bool add_new_changelog(const char* path, double AppVersion,const char* commentaire)
{
	FILE* fichier = NULL;

	if(VerifExiste(path) != true)
	{
		return false;
	}

	fichier = fopen(path, "a");
	if(fichier == NULL)
	{
		return false;
	}

	fprintf(fichier, "BUILD %.3f\n%s%s;\n", AppVersion, "-*- Add -*- ", commentaire);

	fclose(fichier);

	return true;
}

char* get_last_changelog(const char* path)
{
    FILE* fichier = NULL;
    char buffer[SIZE_BUFFER];
    int caractereActuel = 0;
    int nombre_ligne = 0;
    int compteur = 0;

    /* Ouverture du fichier */
    fichier = fopen(path, "r");
    if(fichier == NULL)
    {
        return NULL;
    }

    /* Nombre de ligne */
    do
    {
        caractereActuel = fgetc(fichier);
        if(caractereActuel == '\n')
        {
            nombre_ligne++;
        }

    } while (caractereActuel != EOF);

    /* Retour au debut */
    rewind(fichier);
    
    /* Fichier vers buffer */
    while(fgets(buffer, SIZE_BUFFER, fichier) != NULL)
    {
        if(compteur == nombre_ligne)
        {
            break;
        }

        compteur++;        
    }
    
    buffer[strlen(buffer) - 1] = '\0';
    fclose(fichier);    

    return _strdup(buffer);
}

bool changelog_file_to_char_cut(const char* path)
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


bool remove_last_changelog_entry(const char* path)
{
    char* buffer = NULL;
	
    if(VerifExiste(path) != true)
	{
		return false;
	}

    buffer = get_last_changelog(path);
    if(buffer == NULL)
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

bool Printf__changelog(const char* path)
{
    char* buffer = NULL;

    buffer = FichierToChar(path);
    if(buffer == NULL)
    {
        return false;
    }

    printf("%s\n", buffer);

    free(buffer);

    return true;
}

double ReadLastValue(const char *filename)
{
    FILE *fichier = fopen(filename, "r");
    if (!fichier)
    {
        printf("Ouverture du fichier %s impossible\n", filename);
        return 0;
    }

    // Variables pour la lecture
    char buffer[SIZE_BUFFER];
    int nbligne = 0;
    int compteur = 0;

    nbligne = nombreDeLigne(fichier);
    if(nbligne == 0 || nbligne < 9)
    {
        printf("nombreDeLigne %d\n", nbligne);
        return 0;
    }

    rewind(fichier);

    // Lire le fichier ligne par ligne en comptant les lignes
    while (fgets(buffer, sizeof(buffer), fichier) != NULL)
    {
        compteur++;
        if(compteur == (nbligne - 3))
        {
            break;
        }
    }
    
    char *str = buffer + 6; // Sauter "BUILD "
    char *endptr;
    double value = strtod(str, &endptr);
    
    fclose(fichier);
    
    // Vérification de la validité de la conversion
    if (endptr != str)
    {
        return value;
    }
    
    return 0;
}