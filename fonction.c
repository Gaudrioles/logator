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

	fprintf(fichier, "BUILD %.1f\n%s%s;\n", AppVersion, "-*- Add -*- ", commentaire);

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

char* changelog_file_to_char_cut(const char* path)
{
    FILE* fichier = NULL;
    
    char* buffer = NULL;

    int nombre_ligne = 0;
    int caractereActuel = 0;
    int compteur = 0;
    int i = 0;

    size_t len = 0;

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
    nombre_ligne = nombre_ligne - 2;

    /* Taille du buffer */
    do
    {
        caractereActuel = fgetc(fichier);
        if(caractereActuel == '\n')
        {
            compteur++;
        }
        if(compteur < nombre_ligne)
        {
            len++;
        }

    } while (caractereActuel != EOF);

    len++;
    compteur = 0;
    rewind(fichier);

    buffer = malloc(len * sizeof(char));
    if(buffer == NULL)
    {
        fclose(fichier);
        return NULL;
    }

    /* Fichier vers buffer */
    do
    {
        caractereActuel = fgetc(fichier);
        if(caractereActuel == '\n')
        {
            compteur++;
        }
        if(compteur < nombre_ligne)
        {
            buffer[i] = (char)caractereActuel;
            i++;
        }

    } while (caractereActuel != EOF);
    
    buffer[i] = '\0';
    fclose(fichier);    

    return buffer;
}

bool remove_last_changelog_entry(const char* path)
{
    FILE* fichier = NULL;
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

    buffer = changelog_file_to_char_cut(path);
    if(buffer == NULL)
    {
        return false;
    }
    
    fichier = fopen(path, "w+");
    if(!fichier)
    {
        fprintf(stderr, "Ouverture impossible du fichier %s\n", path);
        free(buffer);
        return false;
    }

    fprintf(fichier, "%s\n", buffer);

    fclose(fichier);
    free(buffer);

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