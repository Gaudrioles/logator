#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "tool.h"

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#include <direct.h>
#include <io.h>

#define F_OK 0
#define access _access
#elif __linux__
#include <unistd.h>
#endif

void removeQuote(char *chaine)
{
    if (chaine == NULL || strlen(chaine) < 2)
    {
        return; // Aucun traitement si la chaîne est NULL ou trop courte
    }

    size_t len = strlen(chaine);
    chaine[len - 2] = '\0'; // Supprime les deux derniers caractères
}

bool VerifExiste(const char *path)
{
    if (access(path, F_OK) == 0)
    {
        return true;
    }
    return false;
}

bool DemandeAccordFichier(const char *FichierNom, int valeur)
{
    int ouinon = -1;
    char chaine[3];

    do
    {
        switch (valeur)
        {
            case 0:
                if(FichierNom != NULL)
                {
                    fprintf(stdout, "%s Merci de confirmer, [O]ui / [N]on : ", FichierNom);
                }
                else
                {
                    fprintf(stdout, "Merci de confirmer, [O]ui / [N]on : ");
                }
                
                break;
            case 1:
                fprintf(stdout, "Le fichier %s existe, voulez-vous le remplacer ? [O]ui / [N]on : ", FichierNom);
                break;
            default:
                fprintf(stdout, "Voulez-vous supprimer Le fichier %s ? [O]ui / [N]on : ", FichierNom);
                break;
        }

        if(scanf("%2s", chaine) != EOF);
        if(scanf("%*[^\n]") != EOF);
        getchar();
        
        if((strcmp(chaine, "o") == 0)|| (strcmp(chaine, "O") == 0))
        {
             ouinon = 1;
        }
        if((strcmp(chaine, "n") == 0)|| (strcmp(chaine, "N") == 0))
        {
             ouinon = 0;
        }

    }while(ouinon != 0 && ouinon != 1);

    if(ouinon == 0)
    {
        return false;
    }

    return true;
}

bool creation_fichier_resource_h(const char *path, ResourceData *data)
{
    /* Check */
    if(!data)
	{
		return false;
	}

	FILE *fichier = NULL;
    if(VerifExiste(path) == true)
	{
		fprintf(stdout, "%s ", path);
		if(!DemandeAccordFichier(path, 1))
		{
			return false;
		}
	}

	fichier = fopen(path, "w");
    if(!fichier)
    {
        fprintf(stderr, "Ouverture impossible du fichier %s\n", path);
        return false;
    }

    fprintf(fichier, "#ifndef RESOURCE_H_INCLUDED\n");
    fprintf(fichier, "#define RESOURCE_H_INCLUDED\n\n");
    fprintf(fichier, "#define APP_VERSION \"%sf\"\n", data->appVersion);
    fprintf(fichier, "#define APP_NAME \"%s\"\n", data->appName);
    fprintf(fichier, "#define INNOSETUP \"%s\"\n\n",  data->innoSetup);
    fprintf(fichier, "#endif /* !RESOURCE_H_INCLUDED */\n");

    fclose(fichier);

    return true;
}

bool write_fichier_resource_h(const char *path, ResourceData *data)
{
    if(!data || !path)
	{
		return false;
	}

    FILE *fichier = NULL;

    fichier = fopen(path, "w");
    if(!fichier)
    {
        fprintf(stderr, "Ouverture impossible du fichier %s\n", path);
        return false;
    }

    fprintf(fichier, "#ifndef RESOURCE_H_INCLUDED\n");
    fprintf(fichier, "#define RESOURCE_H_INCLUDED\n\n");
    fprintf(fichier, "#define APP_VERSION \"%s\"\n", data->appVersion);
    fprintf(fichier, "#define APP_NAME \"%s\"\n", data->appName);
    fprintf(fichier, "#define INNOSETUP \"%s\"\n\n",  data->innoSetup);
    fprintf(fichier, "#endif /* !RESOURCE_H_INCLUDED */\n");

    fclose(fichier);

    return true;
}

bool creation_fichier_changelog(const char *path)
{
	FILE *fichier = NULL;
    
    if(VerifExiste(path) == true)
	{
		fprintf(stdout, "%s ", path);
		if (!DemandeAccordFichier(path, 1))
		{
			return false;
		}
	}

	fichier = fopen(path, "w");
    if(fichier == NULL)
	{
		return false;
	}

	fprintf(fichier, "---------------------------------------------\n-----             CHANGELOG             -----\n---------------------------------------------\n\n");
	fprintf(fichier, "#  -*- Add -*-  ;\n\n\nBUILD 0.1\n-*- Add -*- premiere version realisee;\n");

	fclose(fichier);

	return true;
}

bool creation_fichier_resource_rc(const char *path, const char *Description, ResourceData *data)
{
    if(!data)
	{
		return false;
	}

	FILE *fichier = NULL;
	char *tampon = NULL;

	if (VerifExiste(path) == true)
	{
		fprintf(stdout, "%s ", path);
		if(!DemandeAccordFichier(path, 1))
		{
			return false;
		}
	}

	fichier = fopen(path, "w");
    if(fichier == NULL)
	{
		return false;
	}

	tampon = get_date_annee();
    if(tampon == NULL)
    {
        fclose(fichier);
        return false;        
    }

	fprintf(fichier, "#include \"resource.h\"\n"
					"#include <windows.h>\n\n"
					"#define VER_FILEVERSION             1,0,0,0\n"
					"#define VER_FILEVERSION_STR         \"1.0.0.0\\0\"\n\n"
					"IDI_ICON1               ICON                    \"icons/icon.ico\"\n\n"
					"VS_VERSION_INFO VERSIONINFO\n"
					" FILEVERSION 1,0,0,0\n"
					" FILEFLAGSMASK 0x3fL\n"
					"#ifdef _DEBUG\n"
					" FILEFLAGS 0x1L\n"
					"#else\n"
					" FILEFLAGS 0x0L\n"
					"#endif\n"
					" FILEOS 0x40004L\n"
					" FILETYPE 0x1L\n"
					" FILESUBTYPE 0x0L\n"
					"BEGIN\n"
					"    BLOCK \"StringFileInfo\"\n"
					"    BEGIN\n"
					"        BLOCK \"040c04b0\"\n"
					"        BEGIN\n"
					"            VALUE \"CompanyName\", \"https://github.com/Gaudrioles\"\n"
					"            VALUE \"FileDescription\", \"%s\"\n"
					"            VALUE \"FileVersion\", VER_FILEVERSION_STR\n"
					"            VALUE \"LegalCopyright\", \"Copyright (C) %s\"\n"
					"            VALUE \"OriginalFilename\", \"%s.exe\"\n"
					"            VALUE \"ProductName\", \"%s\"\n"
					"            VALUE \"ProductVersion\", APP_VERSION \"\\0\"\n"
					"        END\n"
					"    END\n"
					"    BLOCK \"VarFileInfo\"\n"
					"    BEGIN\n"
					"        VALUE \"Translation\", 0x40c, 1200\n"
					"    END\n"
					"END\n", Description, tampon, data->appName, data->appName);
	fclose(fichier);

	free(tampon);

	return true;
}

bool creation_fichier_gitignore(const char *path)
{
	FILE *fichier = NULL;

	if(VerifExiste(path) == true)
	{
		fprintf(stdout, "%s ", path);
		if(!DemandeAccordFichier(path, 1))
		{
			return false;
		}
	}

	fichier = fopen(path, "w+");
	if(!fichier)
	{
		return -1;
	}

	fprintf(fichier, "# CodeBlocks Files\n"
					"*.depend\n"
					"*.layout\n"
					"bin/\n"
					"obj/\n\n"
					"# VSCode Files\n"
					".vs/\n"
					".vscode/\n"
					"x64/\n"
					"x86/\n\n"
					"# MakeFile\n"
					"*.o\n"
					"*.exe\n\n"
					"# CMAKE\n"
					"build/\n");
	fclose(fichier);

	return true;
}

size_t NombreDeCaractere(const char *FichierNom)
{
    FILE *fichier = NULL;
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

char *FichierToChar(const char *FichierNom)
{
    FILE *fichier = NULL;
    size_t taille = NombreDeCaractere(FichierNom);
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

    char *chaine = malloc(sizeof(char) * (taille + 1));
    if (chaine == NULL)
    {
        fprintf(stderr, "Allocation memoire impossible FichierToChar %s\n", FichierNom);
        return NULL;
    }

    while((caractereActuel = fgetc(fichier)) != EOF)
    {
        chaine[compteur++] = (char)caractereActuel;
    }
    chaine[compteur] = '\0';

    fclose(fichier);
    
    return chaine;
}

bool parseResourceFile(const char *filePath, ResourceData *data)
{
    if (!filePath || !data)
    {
        return false;
    }

    FILE *fichier = fopen(filePath, "r");
    if (!fichier)
    {
        return false;
    }

    char line[256];
    while (fgets(line, sizeof(line), fichier))
    {
        if (strncmp(line, "#define", 7) == 0)
        {
            char key[128], value[128];
            if (sscanf(line, "#define %s \"%[^\"]\"", key, value) == 2)
            {
                if (strcmp(key, "APP_VERSION") == 0)
                {
                    snprintf(data->appVersion, sizeof(data->appVersion), "%s", value);
                }
                else if (strcmp(key, "APP_NAME") == 0)
                {
                    snprintf(data->appName, sizeof(data->appName), "%s", value);
                }
                else if (strcmp(key, "INNOSETUP") == 0)
                {
                    snprintf(data->innoSetup, sizeof(data->innoSetup), "%s", value);
                }
            }
        }
    }
    fclose(fichier);
    
    return true;
}

char *GetLastValue(void)
{
    FILE *fichier = fopen(CHANGELOG_FILE, "r");
    if (!fichier)
    {
        return NULL; // Impossible d'ouvrir le fichier
    }

    int ligne = nombreDeLigne(fichier);
    if(ligne == 0)
    {
        fclose(fichier);
        return NULL;
    }

    rewind(fichier);

    char buffer[SIZE_BUFFER];
    int compteur = 0;

    // Parcours du fichier ligne par ligne
    while (fgets(buffer, sizeof(buffer), fichier))
    {
        if(compteur == (ligne - 2))
        {
            // Stocker la dernière ligne contenant "BUILD"
            if (strncmp(buffer, "BUILD ", 6) == 0)
            {
                // Supprimer le saut de ligne
                buffer[strcspn(buffer, "\n")] = '\0';
                break;                
            }                
        }
        
        compteur++;
    }

    fclose(fichier);

    return strdup(buffer + 6);
}