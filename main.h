#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#define CHANGELOG_FILE      "CHANGELOG.md"
#define RESOURCE_H_FILE     "resource.h"
#define RESOURCE_RC_FILE    "Resource.rc"
#define GITIGNORE_FILE      ".gitignore"

#define SIZE_BUFFER         2048
#define SIZE_READ           2047

typedef struct
{
    char AppName[SIZE_BUFFER];
    double AppVersion;
    bool AppInno;
}ST_logator;

#endif // MAIN_H_INCLUDED