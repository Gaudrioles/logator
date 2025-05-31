#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#define CHANGELOG_FILE      "CHANGELOG.md"
#define RESOURCE_H_FILE     "resource.h"
#define RESOURCE_RC_FILE    "Resource.rc"
#define GITIGNORE_FILE      ".gitignore"
#define T_CHANGELOG_FILE    "temp_CHANGELOG.md"
#define T_INNOSETUP         "temp_INNO.iss"

#define KEYWORD_HELP        "-help"
#define KEYWORD_CREATE      "-creation"
#define KEYWORD_NEW         "-new"
#define KEYWORD_MAJOR       "-M"
#define KEYWORD_MINOR       "-m"
#define KEYWORD_RESOURCE    "-resource"
#define KEYWORD_GIT         "-gitignore"
#define KEYWORD_INNO        "-innosetup"
#define KEYWORD_VIEW        "-view"
#define KEYWORD_REMOVE      "-remove"

#define SIZE_BUFFER         2048

typedef struct
{
    char AppName[SIZE_BUFFER];
    double AppVersion;
    bool AppInno;
}ST_logator;

typedef struct
{
    char appVersion[SIZE_BUFFER];
    char appName[SIZE_BUFFER];
    char innoSetup[SIZE_BUFFER];
} ResourceData;

#endif // MAIN_H_INCLUDED