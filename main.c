#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "fonction.h"
#include "fichier.h"
#include "message.h"
#include "main.h"

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

		if(creation_fichier_changelog(CHANGELOG_FILE) != true)
		{
			printf_creation_fichier(CHANGELOG_FILE, 0);
			return -1;
		}
		
		printf_creation_fichier(CHANGELOG_FILE, 1);
		ST_logator st;
		st.AppVersion = 1.0f;
		strcpy(st.AppName, argv[2]);
		st.AppInno = false;
		
		if(creation_fichier_resource_h(RESOURCE_H_FILE, &st) != true)
		{
			printf_creation_fichier(RESOURCE_H_FILE, 0);
			return -1;
		}
		
		printf_creation_fichier(RESOURCE_H_FILE, 1);
	}
	else if(strcmp(argv[1], KEYWORD_NEW) == 0)
	{
		if(argc < 4)
		{
			printf_new();
			return -1;			
		}
		
		ST_logator st;
		int versionType = 0;
		if(!loadResourceToStruct(RESOURCE_H_FILE, &st))
		{
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

		st.AppVersion = newVersion(versionType, st.AppVersion);

		if(add_new_changelog(CHANGELOG_FILE, st.AppVersion, argv[3]) != true)
		{
			printf_new();
			return -1;
		}

		printf_update_fichier(CHANGELOG_FILE);
		
		if(write_fichier_resource_h(RESOURCE_H_FILE, &st) != true)
		{
			printf_new();
			return -1;
		}

		printf_update_fichier(RESOURCE_H_FILE);

		if(st.AppInno == true)
		{
			update_innosetup(&st);
		}

	}
	else if(strcmp(argv[1], KEYWORD_RESOURCE) == 0)
	{
		if(argc < 3)
		{
			printf_resource();
			return -1;			
		}

		ST_logator st;
		if(!loadResourceToStruct(RESOURCE_H_FILE, &st))
		{
			return -1;
		}
		
		if(creation_fichier_resource_rc(RESOURCE_RC_FILE, argv[2], &st) != true)
		{
			printf_creation_fichier(RESOURCE_RC_FILE, 0);
		}
		else
		{
			printf_creation_fichier(RESOURCE_RC_FILE, 1);
		}
	}
	else if(strcmp(argv[1], KEYWORD_GIT) == 0)
	{
		if(creation_fichier_gitignore(GITIGNORE_FILE) != true)
		{
			printf_creation_fichier(GITIGNORE_FILE, 0);
		}
		else
		{
			printf_creation_fichier(GITIGNORE_FILE, 1);
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

		ST_logator st;
		if(!loadResourceToStruct(RESOURCE_H_FILE, &st))
		{
			return -1;
		}

		if((strcmp(argv[2], "TRUE") == 0) || (strcmp(argv[2], "true") == 0))
		{
			st.AppInno = true;
		}
		else if((strcmp(argv[2], "FALSE") == 0) || (strcmp(argv[2], "false") == 0))
		{
			st.AppInno = false;
		}
		else
		{
			printf_innosetup();
			return -1;
		}

		if(activation_innosetup(RESOURCE_H_FILE, &st) != true)
		{
			printf_creation_fichier("Innosetup_file", -1);
			return -1;
		}
		
		printf_update_fichier("Innosetup_file");
	}
	else if(strcmp(argv[1], KEYWORD_REMOVE) == 0)
	{
		ST_logator st;
		if(!loadResourceToStruct(RESOURCE_H_FILE, &st))
		{
			return -1;
		}

		if(st.AppVersion < 0.1f)
		{
			printf_msg_empty();
			return -1;
		}

		st.AppVersion = ReadLastValue(CHANGELOG_FILE);

		if(remove_last_changelog_entry(CHANGELOG_FILE) != true)
		{
			return -1;
		}
		
		if(write_fichier_resource_h(RESOURCE_H_FILE, &st) != true)
		{
			return -1;
		}
	}
	else if(strcmp(argv[1], KEYWORD_VIEW) == 0)
	{
		if(Printf__changelog(CHANGELOG_FILE) != true)
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
