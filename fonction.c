#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>

#include "fichier.h"
#include "fonction.h"
#include "message.h"
#include "main.h"
#include "log.h"


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

int creation_fichier_changelog(void)
{
	FILE *fichier = NULL;
	

	if(VerifExiste(CHANGELOG_FILE) == 1)
	{
		fprintf(stdout, "%s ", CHANGELOG_FILE);
		if (DemandeAccord("exsite, voulez - vous le remplacer") != 1)
		{
			return -1;
		}
	}

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

int creation_fichier_resource_h(void)
{
	FILE *fichier = NULL;

	if(VerifExiste(RESOURCE_H_FILE) == 1)
	{
		fprintf(stdout, "%s ", RESOURCE_H_FILE);
		if (DemandeAccord("exsite, voulez - vous le remplacer") != 1)
		{
			return -1;
		}
	}

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
					"#endif /* !RESOURCE_H_INCLUDED */");

	fclose(fichier);

	return 0;
}

int creation_fichier_resource_rc(char *FileDescription, char *ProductName)
{
	FILE *fichier = NULL;
	char *tampon = NULL;

	if (VerifExiste(RESOURCE_RC_FILE) == 1)
	{
		fprintf(stdout, "%s ", RESOURCE_RC_FILE);
		if (DemandeAccord("exsite, voulez - vous le remplacer") != 1)
		{
			return -1;
		}
	}

	fichier = fopen(RESOURCE_RC_FILE, "w");

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

int creation_fichier_gitignore(void)
{
	FILE *fichier = NULL;

	if(VerifExiste(GITIGNORE_FILE) == 1)
	{
		fprintf(stdout, "%s ", GITIGNORE_FILE);
		if (DemandeAccord("exsite, voulez - vous le remplacer") != 1)
		{
			return -1;
		}
	}

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
					"*.exe\n\n"
					" CMAKE\n"
					"build/\n");
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
	FILE *fichier = NULL;
	char *appname = NULL;
	int x = 0;

	x = innosetup_status();
	
	if(x == -1)
	{
		return -1;
	}
	
	appname = application_get_name();
	
	if(appname == NULL)
	{
		return -1;
	}

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
						"#endif /* !RESOURCE_H_INCLUDED */\n", version, appname);
		break;
	case 0 :
		fprintf(fichier, "#ifndef RESOURCE_H_INCLUDED\n"
						"#define RESOURCE_H_INCLUDED\n\n"
						"#define APP_VERSION \"%.1f\"\n"
						"#define APP_NAME \"%s\"\n"
						"#define INNOSETUP \"FALSE\"\n\n"
						"#endif /* !RESOURCE_H_INCLUDED */\n", version, appname);
		break;
	default:
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
						"#endif /* !RESOURCE_H_INCLUDED */\n", version, name);
		break;
	default:
		fprintf(fichier, "#ifndef RESOURCE_H_INCLUDED\n"
						"#define RESOURCE_H_INCLUDED\n\n"
						"#define APP_VERSION \"%.1f\"\n"
						"#define APP_NAME \"%s\"\n"
						"#define INNOSETUP \"FALSE\"\n\n"
						"#endif /* !RESOURCE_H_INCLUDED */\n", version, name);
		break;
	}

	fclose(fichier);    

	return 0;
}

int update_innosetup(double version)
{
	FILE *fichier = NULL;
	FILE *fichierTampon = NULL;

	char* buffer = NULL;
	char FichierNom[SIZE_BUFFER];
	char chaine[SIZE_BUFFER];

	int compteur = 0;


	buffer = application_get_name();

	if (buffer == NULL)
	{
		return -1;
	}

	if((strlen(buffer) + strlen(".iss") + 1) >= SIZE_BUFFER)
	{
		fprintf(stderr, "SIZE_BUFFER trop petit \n");
		free(buffer);
		return -1;
	}

	sprintf(FichierNom, "%s.iss", buffer);
	free(buffer);

	if (VerifExiste(FichierNom) != 1)
	{
		printf("Fichier %s introuvable\n", FichierNom);
		return -1;
	}

	fichier = fopen(FichierNom, "r");
	fichierTampon = fopen("update_innosetup.old", "w+");

	if (fichier == NULL || fichierTampon == NULL)
	{
		return -1;
	}

	while(fgets(chaine, SIZE_READ, fichier) != NULL)
	{
		if(compteur == 4)
		{
			fprintf(fichierTampon, "#define MyAppVersion \"%.1f\"\n", version);
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
		return -1;
	}
	if (rename("update_innosetup.old", FichierNom) != 0)
	{
		return -1;
	}

	printf_update_fichier(FichierNom);

	return 0;
}

int activation_innosetup(char *Valeur)
{
	FILE *fichier = NULL;
	char* name = NULL;
	int x = -1;
	if(VerifExiste(RESOURCE_H_FILE) != 1)
	{
		return -1;
	}

	if(Valeur == NULL)
	{
		printf_innosetup();
		return -1;
	}
	else if(strcmp(Valeur, "TRUE") == 0)
	{
		x = 1;
	}
	else if(strcmp(Valeur, "FALSE") == 0)
	{
		x = 0;
	}

	double version = get_version() - 0.1;
	name = application_get_name();
	if(name == NULL)
	{
		return -1;
	}

	fichier = fopen(RESOURCE_H_FILE, "w+");

	if(fichier == NULL)
	{
		return -1;
	}

	if(x == 1 )
	{
		fprintf(fichier, "#ifndef RESOURCE_H_INCLUDED\n"
						"#define RESOURCE_H_INCLUDED\n\n"
						"#define APP_VERSION \"%.1f\"\n"
						"#define APP_NAME \"%s\"\n"
						"#define INNOSETUP \"TRUE\"\n\n"
						"#endif /* !RESOURCE_H_INCLUDED */\n", version, name);	
	}
	else
	{
		fprintf(fichier, "#ifndef RESOURCE_H_INCLUDED\n"
						"#define RESOURCE_H_INCLUDED\n\n"
						"#define APP_VERSION \"%.1f\"\n"
						"#define APP_NAME \"%s\"\n"
						"#define INNOSETUP \"FALSE\"\n\n"
						"#endif /* !RESOURCE_H_INCLUDED */\n", version, name);

	}

	fclose(fichier);
	free(name);

	return 0;
}

char *get_last_changelog_entry(void)
{
	FILE *fichier = NULL;
	char chaine[SIZE_BUFFER] = { 0 };
	int compteur = 0;
	int nombre_ligne= nombre_de_ligne(CHANGELOG_FILE);

	fichier = fopen(CHANGELOG_FILE, "r");

	if(fichier == NULL)
	{
		return NULL;
	}

	while(fgets(chaine, SIZE_READ, fichier) != NULL)
	{
		if(compteur == nombre_ligne)
		{
			break;
		}
		
		compteur++;
	}

	fclose(fichier);

	return _strdup(chaine);
}

int remove_last_changelog_entry(void)
{
	FILE *fichier = NULL;
	FILE *fichierTampon = NULL;

	int ligne = 0;
	int compteur = 0;

	char chaine[SIZE_BUFFER];
	char buffer[SIZE_BUFFER];

	if(VerifExiste(CHANGELOG_FILE) != 1)
	{
		return -1;
	}

	ligne = nombre_de_ligne(CHANGELOG_FILE);

	if (ligne == -1)
	{
		return -1;
	}
	
	fichier = fopen(CHANGELOG_FILE, "r");
	fichierTampon = fopen("changelog.md.old", "w");
	
	if(fichier == NULL || fichierTampon == NULL)
	{
		return -1;
	}

	while(fgets(chaine, SIZE_READ, fichier) != NULL)
	{
		if(compteur <= (ligne - 3))
		{
			fprintf(fichierTampon, "%s", chaine);
		}
		if(compteur == (ligne - 2))
		{
			size_t len = strlen(chaine);
			for(int i = 0; i < (int)len ; i++)
			{
				if(chaine[i] != '\n')
				{
					buffer[i] = chaine[i];
				}
				else
				{
					buffer[i] = '\0';
				}
			}
			fprintf(fichierTampon, "%s", buffer);
		}

		compteur++;
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

int fonction_remove(void)
{
	char* chaine = NULL;
	double version =  get_version();

	if(version == 1.0)
	{
		fprintf(stdout, "Suppression impossible il y a aucune entree\n");
		return -1;
	}
	else if(version == -1)
	{
		fprintf(stdout, "Lecture impossible du fichier %s\n", RESOURCE_H_FILE);
		return -1;
	}

	chaine = get_last_changelog_entry();
	if(chaine == NULL)
	{
		return -1;
	}

	printf("Suppression %s\n", chaine);
	free(chaine);
	if(DemandeAccord2() != 1)
	{		
		fprintf(stdout, "Suppression impossible\n");
		return -1;
	}

	if(remove_last_changelog_entry() != 0)
	{
		printf_creation_fichier(CHANGELOG_FILE, -1);
	}

	version = version - 0.2;

	if(update_fichier_resource_h(version) == -1)
	{
		printf_creation_fichier(RESOURCE_H_FILE, -1);
	}

	if(innosetup_status() == 1)
	{
		update_innosetup(version);
	}

	fprintf(stdout, "Suppression avec succes\n");

	return 0;
}

int lecture_fichier_changelog(void)
{
	FILE *fichier = NULL;
	char chaine[100];
	int caractereActuel = 0;
	int nombreDeLigne = 0;
	int compteur = 0;

	if(VerifExiste(CHANGELOG_FILE) != 1)
	{
		fprintf(stderr, "Le fichier %s existe pas\n", CHANGELOG_FILE);
		return -1;
	}

	fichier = fopen(CHANGELOG_FILE, "r");

	if(fichier == NULL)
	{
		fprintf(stderr, "Ouverture du fichier %s impossible\n", CHANGELOG_FILE);
		return -1;
	}

	do
	{
		caractereActuel = fgetc(fichier);
		if(caractereActuel == '\n')
		{
			nombreDeLigne++;
		}
	
	} while (caractereActuel != EOF);

	rewind(fichier);

	while(fgets(chaine, 99, fichier) != NULL)
    {
		compteur++;
		if(compteur > 7)
		{
			fprintf(stdout, "%s", chaine);
		}
		
	}

	fprintf(stdout, "\n");

	fclose(fichier);

	return 0;
}