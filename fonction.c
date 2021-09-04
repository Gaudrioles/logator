#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "fichier.h"
#include "fonction.h"
#include "message.h"
#include "main.h"

char *get_date_annee()
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
                    "x64/\n"
                    "x86/\n");
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

    if(verif_fichier_existe(CHANGELOG_FILE) != 1)
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
    FILE *fichierTampon = NULL;

    int nombre_ligne = 0;
    int compteur = 0;
    char chaine[1025] = "";

    if(verif_fichier_existe(RESOURCE_H_FILE) != 1)
    {
        return -1;
    }

    nombre_ligne = nombre_de_ligne(RESOURCE_H_FILE);

    if(nombre_ligne != -1)
    {
        fichier = fopen(RESOURCE_H_FILE, "r");
        fichierTampon = fopen("update_fichier_resource_h.old", "w");

        if(fichier == NULL || fichierTampon == NULL)
        {
            return -1;
        }

        for(compteur = 0; compteur < nombre_ligne; compteur++)
        {
            fgets(chaine, 1024, fichier);
            switch(compteur)
            {
                case 3 :
                    fprintf(fichierTampon, "#define APP_VERSION \"%.1f\"\n", version);
                    break;
                default:
                    fprintf(fichierTampon, "%s", chaine);
                    break;
            }
        }

        fclose(fichier);
        fclose(fichierTampon);

        if (remove(RESOURCE_H_FILE) != 0)
        {
            return -1;
        }
        if (rename("update_fichier_resource_h.old", RESOURCE_H_FILE) != 0)
        {
            return -1;
        }

    }

    return 0;
}

int update_name_resource_h(char *name)
{
    FILE *fichier = NULL;
    FILE *fichierTampon = NULL;

    int nombre_ligne = 0;
    int compteur = 0;
    char chaine[1025] = "";

    if(verif_fichier_existe(RESOURCE_H_FILE) != 1)
    {
        return -1;
    }

    nombre_ligne = nombre_de_ligne(RESOURCE_H_FILE);

    if(nombre_ligne != -1)
    {
        fichier = fopen(RESOURCE_H_FILE, "r");
        fichierTampon = fopen("update_name_resource_h.old", "w");

        if(fichier == NULL || fichierTampon == NULL)
        {
            return -1;
        }

        for(compteur = 0; compteur < nombre_ligne; compteur++)
        {
            fgets(chaine, 1024, fichier);
            switch(compteur)
            {
                case 4:
                    fprintf(fichierTampon, "#define APP_NAME \"%s\"\n", name);
                    break;
                default:
                    fprintf(fichierTampon, "%s", chaine);
                    break;
            }
        }

        fclose(fichier);
        fclose(fichierTampon);

        if (remove(RESOURCE_H_FILE) != 0)
        {
            return -1;
        }
        if(rename("update_name_resource_h.old", RESOURCE_H_FILE) != 0)
        {
            return -1;
        }
    }

    return 0;
}

int update_innosetup(double version)
{
    char *buffer = NULL;
    char *buffer_bis = NULL;
    char chaine[1025] = "";
    size_t taille = 0;

    buffer_bis = application_get_name();

    char detecteur[] = "\"\"";
    char *chaine_remove = strtok(buffer_bis, detecteur);
    free(buffer_bis);

    taille = strlen(chaine_remove) + strlen(".iss") + 1;

    buffer = malloc (taille * sizeof (char));

    sprintf(buffer, "%s.iss", chaine_remove);

    if(verif_fichier_existe(buffer) != 1)
    {
        free(buffer);
        return -1;
    }

    FILE *fichier = NULL;
    FILE *fichierTampon = NULL;

    int nombre_ligne = 0;
    int compteur = 0;

    nombre_ligne = nombre_de_ligne(buffer);

    if(nombre_ligne != -1)
    {
        fichier = fopen(buffer, "r");
        fichierTampon = fopen("update_innosetup.old", "w");

        if(fichier == NULL || fichierTampon == NULL)
        {
            return -1;
        }

        for(compteur = 0; compteur < nombre_ligne; compteur++)
        {
            fgets(chaine, 1024, fichier);
            switch(compteur)
            {
                case 4:
                    fprintf(fichierTampon, "#define MyAppVersion \"%.1f\"\n", version);
                    break;
                default:
                    fprintf(fichierTampon, "%s", chaine);
                    break;
            }
        }
    }

    fclose(fichier);
    fclose(fichierTampon);

    if(remove(buffer) != 0)
    {
        return -1;
    }
    if(rename("update_innosetup.old", buffer) != 0)
    {
        return -1;
    }

    printf_update_fichier(buffer);

    free(buffer);

    return 0;
}

int activation_innosetup(char *TRUE)
{
    FILE *fichier = NULL;
    FILE *fichierTampon = NULL;

    if(verif_fichier_existe(RESOURCE_H_FILE)!= 1)
    {
        return -1;
    }

    int nombre_ligne = 0;
    int compteur = 0;
    char chaine[1025] = "";

    if(TRUE == NULL)
    {
        printf_innosetup();
        return -1;
    }
    else if(strcmp(TRUE, "TRUE") == 0 || strcmp(TRUE, "FALSE") == 0)
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
                fgets(chaine, 1024, fichier);
                if(compteur == 5)
                {
                    fprintf(fichierTampon, "#define INNOSETUP \"%s\"\n", TRUE);
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
    char chaine[1025];
    char tampon[1025];
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
            fgets(chaine, 1024, fichier);
        }
        else
        {
            fgets(tampon, 1024, fichier);
        }
    }

    fclose(fichier);

    return strdup(chaine);
}

int remove_last_changelog_entry()
{
    FILE *fichier = NULL;
    FILE *fichierTampon = NULL;

    int nombre_ligne = 0;
    int compteur = 0;
    char chaine[1025] = "";

    if(verif_fichier_existe(CHANGELOG_FILE) != 1)
    {
        return -1;
    }

    nombre_ligne = nombre_de_ligne(CHANGELOG_FILE);

    if(nombre_ligne != -1)
    {
        fichier = fopen(CHANGELOG_FILE, "r");
        fichierTampon = fopen("changelog.md.old", "w");

        if(fichier == NULL || fichierTampon == NULL)
        {
            return -1;
        }

        for(compteur = 0; compteur < nombre_ligne - 1; compteur++)
        {
            if(compteur != nombre_ligne -2)
            {
                fgets(chaine, 1024, fichier);
                fprintf(fichierTampon, "%s", chaine);
            }
            else
            {
                fgets(chaine, 1024, fichier);

                size_t len =  strlen(chaine);

                int i = 0;

                for(i = 0; i < len-1; i++)
                {
                    fputc(chaine[i], fichierTampon);
                }

            }
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

    }
    else
    {
        return -1;
    }

    return 0;
}

int fonction_remove()
{
    char *chaine = NULL;
    char tampon[1];

    double version =  get_version();

    if(verif_fichier_existe(CHANGELOG_FILE) != 1)
    {
        return -1;
    }

    while(1)
    {
        chaine = get_last_changelog_entry(CHANGELOG_FILE);
        printf("Suppression de -> %s\nConfirmation [O]ui [N]on ? :", chaine);
        free(chaine);

        scanf("%s", tampon);

        if(strcmp(tampon, "O") == 0 || strcmp(tampon, "o") == 0)
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
        else if(strcmp(tampon, "N") == 0 || strcmp(tampon, "n") == 0)
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
