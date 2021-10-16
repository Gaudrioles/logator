#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fichier.h"
#include "fonction.h"
#include "message.h"
#include "main.h"

#if defined(__GNUC__) || defined(__GNUG__)
#include <time.h>

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

char* get_date_annee(void)
{
	char buffer[21];
	SYSTEMTIME st = { 0 };
	size_t Longueur = 0;
	char* ChaineRetour = NULL;

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



int creation_fichier_changelog()
{
	FILE *fichier = NULL;

	fichier = fopen(CHANGELOG_FILE, "w");

	if(fichier == NULL)
	{
		return -1;
	}

	fprintf(fichier, "---------------------------------------------\n-----             CHANGELOG             -----\n---------------------------------------------\n\n");
	fprintf(fichier, "#  -*- Add -*-  ;\n\n\nBUILD 1.0\n-*- Add -*- premiere version realisee;");

	fclose(fichier);

	return 0;
}

int creation_fichier_resource_h()
{
	FILE *fichier = NULL;

	fichier = fopen(RESOURCE_H_FILE, "w");

	if(fichier == NULL)
	{
		return -1;
	}

	fprintf(fichier, "#ifndef RESOURCE_H_INCLUDED\n"
					"#define RESOURCE_H_INCLUDED\n\n"
					"#define APP_VERSION \"1.0\"\n"
					"#define APP_NAME \"\"\n"
					"#define INNOSETUP \"FALSE\"\n\n"
					"#endif // RESOURCE_H_INCLUDED");

	fclose(fichier);

	return 0;
}

int creation_fichier_resource_rc(char *FileDescription, char *ProductName)
{
	FILE *fichier = NULL;
	char *tampon = NULL;

	fichier = fopen("Resource.rc", "w");

	if(fichier == NULL)
	{
		return -1;
	}

	tampon = get_date_annee();

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
					"END\n", FileDescription, tampon, ProductName, ProductName);
	fclose(fichier);

	free(tampon);

	update_name_resource_h(ProductName);

	return 0;
}

int creation_fichier_gitignore()
{
	FILE *fichier = NULL;

	fichier = fopen(GITIGNORE_FILE, "w");

	if(fichier == NULL)
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
					"*.exe\n");
	fclose(fichier);

	return 0;
}

int update_fichier_changelog(double version, char *commentaire)
{
	FILE *fichier = NULL;

	if(commentaire == NULL)
	{
		printf_new();
		return -1;
	}

	if(VerifExiste(CHANGELOG_FILE) != 1)
	{
		return -1;
	}

	fichier = fopen(CHANGELOG_FILE, "a");

	if(fichier == NULL)
	{
		return -1;
	}

	fprintf(fichier, "\nBUILD %.1f\n%s%s;", version, "-*- Add -*- ", commentaire);

	fclose(fichier);

	return 0;
}

int update_fichier_resource_h(double version)
{
	int x = innosetup_status();
	char* appname = application_get_name();

	FILE *fichier = NULL;

	fichier = fopen(RESOURCE_H_FILE, "w");

	if(fichier == NULL)
	{
		free(appname);
		return -1;
	}

	switch (x)
	{
	case 1 :
		fprintf(fichier, "#ifndef RESOURCE_H_INCLUDED\n"
						"#define RESOURCE_H_INCLUDED\n\n"
						"#define APP_VERSION \"%.1f\"\n"
						"#define APP_NAME \"%s\"\n"
						"#define INNOSETUP \"TRUE\"\n\n"
						"#endif // RESOURCE_H_INCLUDED\n", version, appname);
		break;
	default:
		fprintf(fichier, "#ifndef RESOURCE_H_INCLUDED\n"
						"#define RESOURCE_H_INCLUDED\n\n"
						"#define APP_VERSION \"%.1f\"\n"
						"#define APP_NAME \"%s\"\n"
						"#define INNOSETUP \"FALSE\"\n\n"
						"#endif // RESOURCE_H_INCLUDED\n", version, appname);
		break;
	}

	fclose(fichier);
	free(appname);    

	return 0;
}

int update_name_resource_h(char *name)
{
	int x = innosetup_status();
	double version = get_version();

	FILE *fichier = NULL;

	fichier = fopen(RESOURCE_H_FILE, "w");

	if(fichier == NULL)
	{
		return -1;
	}

	switch (x)
	{
	case 1 :
		fprintf(fichier, "#ifndef RESOURCE_H_INCLUDED\n"
						"#define RESOURCE_H_INCLUDED\n\n"
						"#define APP_VERSION \"%.1f\"\n"
						"#define APP_NAME \"%s\"\n"
						"#define INNOSETUP \"TRUE\"\n\n"
						"#endif // RESOURCE_H_INCLUDED\n", version, name);
		break;
	default:
		fprintf(fichier, "#ifndef RESOURCE_H_INCLUDED\n"
						"#define RESOURCE_H_INCLUDED\n\n"
						"#define APP_VERSION \"%.1f\"\n"
						"#define APP_NAME \"%s\"\n"
						"#define INNOSETUP \"FALSE\"\n\n"
						"#endif // RESOURCE_H_INCLUDED\n", version, name);
		break;
	}

	fclose(fichier);    

	return 0;
}

int update_innosetup(double version)
{
	char* tampon = NULL;
	char* buffer = NULL;

	char chaine[1025] = "";
	size_t len = 0;

	tampon = application_get_name();

	if (tampon == NULL)
	{
		return -1;
	}

	len = strlen(tampon) + strlen(".iss") + 1;

	buffer = malloc(len * sizeof(*buffer));

	if (buffer == NULL)
	{
		return -1;
	}

	sprintf(buffer, "%s.iss", tampon);

	free(tampon);

	if (VerifExiste(buffer) != 1)
	{
		printf("Fichier %s introuvable\n", buffer);

		free(buffer);
		return -1;
	}
	
	FILE* fichier = NULL;
	FILE* fichierTampon = NULL;

	int nombre_ligne = 0;
	int compteur = 0;

	nombre_ligne = nombre_de_ligne(buffer);

	if (nombre_ligne != -1)
	{
		fichier = fopen(buffer, "r");
		fichierTampon = fopen("update_innosetup.old", "w");

		if (fichier == NULL || fichierTampon == NULL)
		{
			return -1;
		}

		for (compteur = 0; compteur < nombre_ligne; compteur++)
		{
			if(fgets(chaine, 1024, fichier) == NULL)
			{
			fclose(fichier);
			fclose(fichierTampon);
			return -1;
		}


			switch (compteur)
			{
			case 4:
				fprintf(fichierTampon, "#define MyAppVersion \"%.1f\"\n", version);
				break;
			default:
				fprintf(fichierTampon, "%s", chaine);
				break;
			}
		}

		fclose(fichier);
		fclose(fichierTampon);
	}

	if (remove(buffer) != 0)
	{
		return -1;
	}
	if (rename("update_innosetup.old", buffer) != 0)
	{
		return -1;
	}

	printf_update_fichier(buffer);

	free(buffer);

	return 0;
}

int activation_innosetup(char* Valeur)
{
	FILE *fichier = NULL;
	FILE *fichierTampon = NULL;

	if(VerifExiste(RESOURCE_H_FILE) != 1)
	{
		return -1;
	}

	int nombre_ligne = 0;
	int compteur = 0;
	char chaine[1025] = "";

	if(Valeur == NULL)
	{
		printf_innosetup();
		return -1;
	}
	else if(strcmp(Valeur, "TRUE") == 0 || strcmp(Valeur, "FALSE") == 0)
	{
		nombre_ligne = nombre_de_ligne(RESOURCE_H_FILE);

		if(nombre_ligne != -1)
		{
			fichier = fopen(RESOURCE_H_FILE, "r");
			fichierTampon = fopen("activation_innosetup.old", "w");

			if(fichier == NULL || fichierTampon == NULL)
			{
				return -1;
			}

			for(compteur = 0; compteur < nombre_ligne +1 ; compteur++)
			{
				if(fgets(chaine, 1024, fichier) == NULL)
				{
					fclose(fichier);
					fclose(fichierTampon);
					return -1;
				}

				if(compteur == 5)
				{
					fprintf(fichierTampon, "#define INNOSETUP \"%s\"\n", Valeur);
				}
				else
				{
					fprintf(fichierTampon, "%s", chaine);
				}
			}

			fclose(fichier);
			fclose(fichierTampon);

			if(remove(RESOURCE_H_FILE) != 0)
			{
				return -1;
			}
			if(rename("activation_innosetup.old", RESOURCE_H_FILE) != 0)
			{
				return -1;
			}
		}
	}
	else
	{
			printf_innosetup();
			return -1;
	}

	return 0;
}

char *get_last_changelog_entry(char *filename)
{
	FILE *fichier = NULL;
	char chaine[1025] = { 0 };
	char tampon[1025] = { 0 };
	int compteur = 0;
	int nombre_ligne= nombre_de_ligne(CHANGELOG_FILE);

	fichier = fopen(filename, "r");

	if(fichier == NULL)
	{
		return NULL;
	}

	for(compteur = 0; compteur < nombre_ligne +1; compteur++)
	{
		if(compteur == nombre_ligne)
		{
			if(fgets(chaine, 1024, fichier) == NULL)
			{
				fclose(fichier);
				return NULL;
			}

		}
		else
		{
			if(fgets(tampon, 1024, fichier) == NULL)
			{
				fclose(fichier);
				return NULL;
			}
		}
	}

	fclose(fichier);

	size_t len = strlen(chaine) + 1;
	char* result = (char*)malloc(len);

	if (result == NULL)
	{
		return NULL;
	}

	return (char*)memcpy(result, chaine, len);
}

int remove_last_changelog_entry()
{
	FILE *fichier = NULL;
	FILE *fichierTampon = NULL;

	int ligne = 0;
	int compteur = 0;

	char *chaine = NULL;

	if(VerifExiste(CHANGELOG_FILE) != 1)
	{
		return -1;
	}

	ligne = nombre_de_ligne(CHANGELOG_FILE);

	if (ligne == -1)
	{
		return -1;
	}

	ligne = ligne - 1;
	
	fichier = fopen(CHANGELOG_FILE, "r");
	fichierTampon = fopen("changelog.md.old", "w");
	
	if(fichier == NULL || fichierTampon == NULL)
	{
		return -1;
	}

	for (compteur = 0; compteur < ligne; compteur++)
	{
		chaine = malloc(1025 * sizeof(*chaine));
		
		if (chaine == NULL)
		{
			fclose(fichier);
			fclose(fichierTampon);
			return -1;
		}

		if(fgets(chaine, 1024, fichier) == NULL)
		{
			fclose(fichier);
			fclose(fichierTampon);
			return -1;
		}

		if(compteur != ligne -1)
		{
			fprintf(fichierTampon, "%s", chaine);
		}
		else
		{
			size_t len = strlen(chaine);
			char*  buffer = malloc(len * sizeof(*buffer));
			if(buffer != NULL)
			{
				snprintf(buffer, len, "%s", chaine);
				fprintf(fichierTampon, "%s", buffer);
				free(buffer);
			}
		}
		
		free(chaine);
	}
	
	fclose(fichier);
	fclose(fichierTampon);
	
	if (remove(CHANGELOG_FILE) != 0)
	{
		return -1;
	}
	if (rename("changelog.md.old", CHANGELOG_FILE) != 0)
	{
		return -1;
	}

	return 0;
}

int fonction_remove()
{
	char *chaine = NULL;
	char tampon = 0;

	double version =  get_version();

	if(VerifExiste(CHANGELOG_FILE) != 1)
	{
		return -1;
	}

	while(1)
	{
		chaine = get_last_changelog_entry(CHANGELOG_FILE);
		printf("Suppression de -> %s\nConfirmation [O]ui [N]on ? :", chaine);
		free(chaine);

		if(scanf("%c", &tampon) == 0)
		{
			continue;
		}

		if(tampon == 'O' || tampon == 'o')
		{
			if(remove_last_changelog_entry() == 0)
			{
				printf("Suppression avec succes\n");
				break;
			}
			else
			{
				printf("Suppression impossible\n");
			}


		}
		else if(tampon == 'N' || tampon == 'n')
		{
			printf("Annulation de la suppression\n");
			break;
		}
	}

	version = version - 0.2;

	if(update_fichier_resource_h(version) != 0)
	{
		printf_new();
	}

	if(innosetup_status() == 1)
	{
		update_innosetup(version);
	}

	return 0;
}