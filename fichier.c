#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "fonction.h"

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

bool loadResourceToStruct(const char *path, ST_logator *st)
{
    if(!st)
	{
		return false;
	}
    FILE *fichier = NULL;
    char buffer[SIZE_BUFFER] = {0};
    char bufferVersion[SIZE_BUFFER] = {0};
    char bufferName[SIZE_BUFFER] = {0};
    char bufferInno[SIZE_BUFFER] = {0};
    char *zBuffer = NULL;
    int compteur = 0;
    
    fichier = fopen(path, "r");
    if(!fichier)
    {
        fprintf(stderr, "Ouverture impossible du fichier %s\n", path);
        return false;
    }

    while(fgets(buffer, SIZE_BUFFER, fichier) != NULL)
    {
        switch (compteur)
        {
            case 3:
                zBuffer = buffer + 21;
                strncpy(bufferVersion, zBuffer, SIZE_BUFFER);
                removeQuote(bufferVersion);
                break;
            case 4:
                zBuffer = buffer + 18;
                strncpy(bufferName, zBuffer, SIZE_BUFFER);
                removeQuote(bufferName);                
                break;
            case 5:
                zBuffer = buffer + 19;
                strncpy(bufferInno, zBuffer, SIZE_BUFFER);
                removeQuote(bufferInno);
                break;
            
            default:
                break;
        }

        compteur++;
    }

    fclose(fichier);

    /* APP_VERSION */
    char *endptr = NULL;
    st->AppVersion = strtod(bufferVersion, &endptr);
    
    if (endptr == bufferVersion)
    {
        return false;
    }
    
    /* APP_NAME */
    strncpy(st->AppName, bufferName, SIZE_BUFFER);
    
    /* INNOSETUP */
    if(strcmp(bufferInno, "TRUE") == 0)
    {
        st->AppInno = true;
    }
    else
    {
        st->AppInno = false;
    }

    return true;
}

bool loadResourceToVariable(const char *path, char *AppName, double *AppVersion, bool *AppInno)
{
    FILE *fichier = NULL;
    char buffer[SIZE_BUFFER] = {0};
    char bufferVersion[SIZE_BUFFER] = {0};
    char bufferName[SIZE_BUFFER] = {0};
    char bufferInno[SIZE_BUFFER] = {0};
    char *zBuffer = NULL;
    int compteur = 0;
    
    fichier = fopen(path, "r");
    if(!fichier)
    {
        fprintf(stderr, "Ouverture impossible du fichier %s\n", path);
        return false;
    }

    while(fgets(buffer, SIZE_BUFFER, fichier) != NULL)
    {
        switch (compteur)
        {
            case 3:
                zBuffer = buffer + 21;
                strncpy(bufferVersion, zBuffer, SIZE_BUFFER);
                removeQuote(bufferVersion);
                break;
            case 4:
                zBuffer = buffer + 18;
                strncpy(bufferName, zBuffer, SIZE_BUFFER);
                removeQuote(bufferName);                
                break;
            case 5:
                zBuffer = buffer + 19;
                strncpy(bufferInno, zBuffer, SIZE_BUFFER);
                removeQuote(bufferInno);
                break;
            
            default:
                break;
        }

        compteur++;
    }

    fclose(fichier);

    /* APP_VERSION */
    char *endptr = NULL;
    *AppVersion = strtod(bufferVersion, &endptr);
    
    if (endptr == bufferVersion)
    {
        return false;
    }
    
    /* APP_NAME */
    strncpy(AppName, bufferName, SIZE_BUFFER);
    
    /* INNOSETUP */
    if(strcmp(bufferInno, "TRUE") == 0)
    {
        *AppInno = true;
    }
    else
    {
        *AppInno = false;
    }

    return true;
}

bool creation_fichier_resource_h(const char *path, ST_logator *st)
{
    if(!st)
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
    fprintf(fichier, "#define APP_VERSION \"%.1f\"\n", st->AppVersion);
    fprintf(fichier, "#define APP_NAME \"%s\"\n", st->AppName);
    fprintf(fichier, "#define INNOSETUP \"%s\"\n\n",  st->AppInno ? "TRUE" : "FALSE");
    fprintf(fichier, "#endif /* !RESOURCE_H_INCLUDED */\n");

    fclose(fichier);

    return true;
}

bool write_fichier_resource_h(const char *path, ST_logator *st)
{
    if(!st)
	{
		return false;
	}
    if(!st)
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
    fprintf(fichier, "#define APP_VERSION \"%.3f\"\n", st->AppVersion);
    fprintf(fichier, "#define APP_NAME \"%s\"\n", st->AppName);
    fprintf(fichier, "#define INNOSETUP \"%s\"\n\n",  st->AppInno ? "TRUE" : "FALSE");
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
	fprintf(fichier, "#  -*- Add -*-  ;\n\n\nBUILD 0.001\n-*- Add -*- premiere version realisee;\n");

	fclose(fichier);

	return true;
}

bool creation_fichier_resource_rc(const char *path, const char *Description, ST_logator *st)
{
    if(!st)
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
					"END\n", Description, tampon, st->AppName, st->AppName);
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