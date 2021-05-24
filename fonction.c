#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "fichier.h"
#include "fonction.h"
#include "message.h"

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

    fichier = fopen("CHANGELOG.md", "w");

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

    fichier = fopen("resource.h", "w");

    if(fichier == NULL)
    {
        return -1;
    }

    fprintf(fichier, "#ifndef RESOURCE_H_INCLUDED\n");
    fprintf(fichier, "#define RESOURCE_H_INCLUDED\n\n");
    fprintf(fichier, "#define APP_VERSION \"1.0\"\n");
    fprintf(fichier, "#define APP_NAME \"\"\n");
    fprintf(fichier, "#define INNOSETUP \"FALSE\"\n\n");
    fprintf(fichier, "#endif // RESOURCE_H_INCLUDED\n");

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

    fprintf(fichier, "#include \"resource.h\"\n");
    fprintf(fichier, "#include <windows.h>\n\n");
    fprintf(fichier, "#define VER_FILEVERSION             1,0,0,0\n");
    fprintf(fichier, "#define VER_FILEVERSION_STR         \"1.0.0.0\\0\"\n\n");
    fprintf(fichier, "IDI_ICON1               ICON                    \"%s\"\n\n", "icons/icon.ico");
    fprintf(fichier, "VS_VERSION_INFO VERSIONINFO\n");
    fprintf(fichier, " FILEVERSION 1,0,0,0\n");
    fprintf(fichier, " FILEFLAGSMASK 0x3fL\n");
    fprintf(fichier, "#ifdef _DEBUG\n");
    fprintf(fichier, " FILEFLAGS 0x1L\n");
    fprintf(fichier, "#else\n");
    fprintf(fichier, " FILEFLAGS 0x0L\n");
    fprintf(fichier, "#endif\n");
    fprintf(fichier, " FILEOS 0x40004L\n");
    fprintf(fichier, " FILETYPE 0x1L\n");
    fprintf(fichier, " FILESUBTYPE 0x0L\n");
    fprintf(fichier, "BEGIN\n");
    fprintf(fichier, "    BLOCK \"StringFileInfo\"\n");
    fprintf(fichier, "    BEGIN\n");
    fprintf(fichier, "        BLOCK \"040c04b0\"\n");
    fprintf(fichier, "        BEGIN\n");
    fprintf(fichier, "            VALUE \"CompanyName\", \"https://github.com/Gaudrioles\"\n");
    fprintf(fichier, "            VALUE \"FileDescription\", \"%s\"\n", FileDescription);
    fprintf(fichier, "            VALUE \"FileVersion\", VER_FILEVERSION_STR\n");
    fprintf(fichier, "            VALUE \"LegalCopyright\", \"Copyright (C) %s\"\n", tampon);
    fprintf(fichier, "            VALUE \"OriginalFilename\", \"%s.exe\"\n", ProductName);
    fprintf(fichier, "            VALUE \"ProductName\", \"%s\"\n", ProductName);
    fprintf(fichier, "            VALUE \"ProductVersion\", APP_VERSION \"\\0\"\n");
    fprintf(fichier, "        END\n");
    fprintf(fichier, "    END\n");
    fprintf(fichier, "    BLOCK \"VarFileInfo\"\n");
    fprintf(fichier, "    BEGIN\n");
    fprintf(fichier, "        VALUE \"Translation\", 0x40c, 1200\n");
    fprintf(fichier, "    END\n");
    fprintf(fichier, "END\n");

    fclose(fichier);

    free(tampon);

    update_name_resource_h(ProductName);

    return 0;
}

int creation_fichier_gitignore()
{
    FILE *fichier = NULL;

    fichier = fopen(".gitignore", "w");

    if(fichier == NULL)
    {
        return -1;
    }

    fprintf(fichier, "# CodeBlocks Files\n*.depend\n*.layout\nbin/\nobj/\n\n");
    fprintf(fichier, "# VSCode Files\n.vs/\nx64/\nx86/\n");


    fclose(fichier);

    return 0;
}

int update_fichier_changelog(char *version, char *commentaire)
{
    FILE *fichier = NULL;

    if(version == NULL)
    {
        printf_new();
        return -1;
    }

    if(commentaire == NULL)
    {
        printf_new();
        return -1;
    }

    fichier = fopen("CHANGELOG.md", "a");

    if(fichier == NULL)
    {
        return -1;
    }

    fprintf(fichier, "\nBUILD %s\n%s%s;", version, "-*- Add -*- ", commentaire);

    fclose(fichier);

    return 0;
}

int update_fichier_resource_h(char *version)
{
    FILE *fichier = NULL;
    FILE *fichierTampon = NULL;

    int nombre_ligne = 0;
    int compteur = 0;
    char chaine[1025] = "";

    if(version == NULL)
    {
        printf_new();
        return -1;
    }

    nombre_ligne = nombre_de_ligne("resource.h");

    if(nombre_ligne != -1)
    {
        fichier = fopen("resource.h", "r");
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
                    fprintf(fichierTampon, "#define APP_VERSION \"%s\"\n", version);
                    break;
                default:
                    fprintf(fichierTampon, "%s", chaine);
                    break;
            }
        }

        fclose(fichier);
        fclose(fichierTampon);

        if (remove("resource.h") != 0)
        {
            return -1;
        }
        if (rename("update_fichier_resource_h.old", "resource.h") != 0)
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

    nombre_ligne = nombre_de_ligne("resource.h");

    if(nombre_ligne != -1)
    {
        fichier = fopen("resource.h", "r");
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

        if (remove("resource.h") != 0)
        {
            return -1;
        }
        if(rename("update_name_resource_h.old", "resource.h") != 0)
        {
            return -1;
        }
    }

    return 0;
}

int update_innosetup(char *version)
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
                    fprintf(fichierTampon, "#define MyAppVersion \"%s\"\n", version);
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
        nombre_ligne = nombre_de_ligne("resource.h");

        if(nombre_ligne != -1)
        {
            fichier = fopen("resource.h", "r");
            fichierTampon = fopen("activation_innosetup.old", "w");

            if(fichier == NULL || fichierTampon == NULL)
            {
                return -1;
            }

            for(compteur = 0; compteur < nombre_ligne+1; compteur++)
            {
                fgets(chaine, 1024, fichier);
                switch(compteur)
                {
                    case 5 :
                        fprintf(fichierTampon, "#define INNOSETUP \"%s\"\n", TRUE);
                        break;
                    default:
                        fprintf(fichierTampon, "%s", chaine);
                        break;
                }
            }

            fclose(fichier);
            fclose(fichierTampon);

            if(remove("resource.h") != 0)
            {
                return -1;
            }
            if(rename("activation_innosetup.old", "resource.h") != 0)
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
