#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

char* get_date_annee()
{
    time_t temps;
    struct tm* tm_info;
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
    FILE* fichier = NULL;

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
    FILE* fichier = NULL;

    fichier = fopen("resource.h", "w");

    if(fichier == NULL)
    {
        return -1;
    }

    fprintf(fichier, "#ifndef RESOURCE_H_INCLUDED\n#define RESOURCE_H_INCLUDED\n\n#define APP_VERSION \"1.0\"\n\n#endif // RESOURCE_H_INCLUDED");

    fclose(fichier);

    return 0;
}

int update_fichier_changelog(char* version, char* commentaire)
{
    FILE* fichier = NULL;

    if(version == NULL)
    {
        printf("-new erreur version\n");
        return -1;
    }

    if(commentaire == NULL)
    {
        printf("-new mauvais commentaire\n");
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

int update_fichier_resource_h(char* version)
{
    FILE* fichier = NULL;

    if(version == NULL)
    {
        printf("-new erreur version\n");
        return -1;
    }

    fichier = fopen("resource.h", "w");

    if(fichier == NULL)
    {
        return -1;
    }

    fprintf(fichier, "#ifndef RESOURCE_H_INCLUDED\n#define RESOURCE_H_INCLUDED\n\n#define APP_VERSION \"%s\"\n\n#endif // RESOURCE_H_INCLUDED", version);

    fclose(fichier);

    return 0;
}

void fonction_aide()
{
    printf("\n\n*********************************************************************************************\n");
    printf("*                                                                                           *\n");
    printf("*         -> Liste des commandes                                                            *\n");
    printf("*                logator -help                                                              *\n");
    printf("*                                                                                           *\n");
    printf("*         -> Creation fichier CHANGELOG.md et resource.h :                                  *\n");
    printf("*                logator -creation                                                          *\n");
    printf("*                                                                                           *\n");
    printf("*         -> Ajout de nouvelle version :                                                    *\n");
    printf("*                logator -new \"version\" \"commentaire\"                                       *\n");
    printf("*                                                                                           *\n");
    printf("*         -> Creation fichier Resource.rc                                                   *\n");
    printf("*                logator -resource \"FileDescription\" \"Executable_name\" \"ProductName\"        *\n");
    printf("*                                                                                           *\n");
    printf("*         -> Creation fichier .gitignore :                                                  *\n");
    printf("*                logator -gitignore                                                         *\n");
    printf("*                                                                                           *\n");
    printf("*********************************************************************************************\n");
}

int creation_fichier_resource_rc(char* FileDescription, char* Executable_name, char* ProductName)
{
    FILE* fichier = NULL;
    char* tampon = NULL;

    fichier = fopen("Resource.rc", "w");

    if(fichier == NULL)
    {
        return -1;
    }

    tampon = get_date_annee();

    fprintf(fichier, "#include \"resource.h\"\n");
    fprintf(fichier, "#include <windows.h>\n\n");
    fprintf(fichier, "IDI_ICON1               ICON                    \"%s\"\n\n", "icons/icon.ico");
    fprintf(fichier, "VS_VERSION_INFO VERSIONINFO\n");
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
    fprintf(fichier, "            VALUE \"FileVersion\", APP_VERSION\n");
    fprintf(fichier, "            VALUE \"LegalCopyright\", \"Copyright (C) %s\"\n", tampon);
    fprintf(fichier, "            VALUE \"OriginalFilename\", \"%s.exe\"\n", Executable_name);
    fprintf(fichier, "            VALUE \"ProductName\", \"%s\"\n", ProductName);
    fprintf(fichier, "            VALUE \"ProductVersion\", APP_VERSION\n");
    fprintf(fichier, "        END\n");
    fprintf(fichier, "    END\n");
    fprintf(fichier, "    BLOCK \"VarFileInfo\"\n");
    fprintf(fichier, "    BEGIN\n");
    fprintf(fichier, "        VALUE \"Translation\", 0x40c, 1200\n");
    fprintf(fichier, "    END\n");
    fprintf(fichier, "END\n");

    fclose(fichier);

    free(tampon);

    return 0;
}

int creation_fichier_gitignore()
{
    FILE* fichier = NULL;

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
