#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "tool.h"
#include "fonction.h"
#include "fichier.h"
#include "message.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf_bad_flags();
		return -1;
	}

	if(strcmp(argv[1], KEYWORD_CREATE) == 0)
	{
		if(argc < 3)
		{
			printf_fonction_creation();
			return -1;			
		}
		
		if(!creation_fonction(argv[2]))
		{
			fprintf(stderr, "Erreur creation_fonction");
			return -1;
		}
	}
	else if(strcmp(argv[1], KEYWORD_NEW) == 0)
	{
		int versionType = 0;
		if(argc < 4)
		{
			printf_new();
			return -1;			
		}

		if(strcmp(argv[2], KEYWORD_MAJOR) == 0)
		{
			versionType = 1;
		}
		else if(strcmp(argv[2], KEYWORD_MINOR) == 0)
		{
			versionType = 0;
		}
		else
		{
			printf_new();
			return -1;			
		}
		
		if(!newVersion_fonction(versionType, argv[3]))
		{
			fprintf(stderr, "Erreur newVersion_fonction");
			return -1;
		}

	}
	else if(strcmp(argv[1], KEYWORD_RESOURCE) == 0)
	{
		if(argc < 3)
		{
			printf_resource();
			return -1;			
		}

		if(!resource_fonction(argv[2]))
		{
			fprintf(stderr, "Erreur resource_fonction");
			return -1;
		}
	}
	else if(strcmp(argv[1], KEYWORD_GIT) == 0)
	{
		if(!gitignore_fonction())
		{
			fprintf(stderr, "Erreur gitignore_fonction");
			return -1;
		}
	}
	else if(strcmp(argv[1], KEYWORD_HELP) == 0)
	{
		printf_aide();
	}
	else if(strcmp(argv[1], KEYWORD_INNO) == 0)
	{
		if(argc < 3)
		{
			printf_innosetup();
			return -1;			
		}

		bool innosetup = false;
		if((strcmp(argv[2], "TRUE") == 0) || (strcmp(argv[2], "true") == 0))
		{
			innosetup = true;
		}
		else if((strcmp(argv[2], "FALSE") == 0) || (strcmp(argv[2], "false") == 0))
		{
			innosetup = false;
		}
		
		if(!innosetup_fonction(innosetup))
		{
			fprintf(stderr, "Erreur innosetup_fonction");
			return -1;
		}
	}
	else if(strcmp(argv[1], KEYWORD_REMOVE) == 0)
	{
		if(!remove_fonction())
		{
			fprintf(stderr, "Erreur remove_fonction");
			return -1;
		}
	}
	else if(strcmp(argv[1], KEYWORD_VIEW) == 0)
	{
		if(!Printf__changelog(CHANGELOG_FILE))
		{
			printf_msg_changelog(CHANGELOG_FILE);
			return -1;
		}
	}
	else
	{
		printf_bad_flags();
		return -1;
	}

	return 0;
}
