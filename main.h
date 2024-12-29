#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#define CHANGELOG_FILE      "CHANGELOG.md"
#define RESOURCE_H_FILE     "resource.h"
#define RESOURCE_RC_FILE    "Resource.rc"
#define GITIGNORE_FILE      ".gitignore"

#define KEYWORD_HELP        "-help"
#define KEYWORD_CREATE      "-creation"
#define KEYWORD_NEW         "-new"
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

#endif // MAIN_H_INCLUDED