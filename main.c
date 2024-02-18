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

	if(strcmp(argv[1], "-creation") == 0)
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
	else if(strcmp(argv[1], "-new") == 0)
	{
		if(argc < 3)
		{
			printf_new();
			return -1;			
		}

		ST_logator st;
		chargement_fichier_resource_h(RESOURCE_H_FILE, &st);

		st.AppVersion = st.AppVersion + 0.1f;

		if(add_new_changelog(CHANGELOG_FILE, st.AppVersion, argv[2]) != true)
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
	else if(strcmp(argv[1], "-resource") == 0)
	{
		if(argc < 3)
		{
			printf_resource();
			return -1;			
		}

		ST_logator st;
		chargement_fichier_resource_h(RESOURCE_H_FILE, &st);
		
		if(creation_fichier_resource_rc(RESOURCE_RC_FILE, argv[2], &st) != true)
		{
			printf_creation_fichier(RESOURCE_RC_FILE, 0);
		}
		else
		{
			printf_creation_fichier(RESOURCE_RC_FILE, 1);
		}
	}
	else if(strcmp(argv[1], "-gitignore") == 0)
	{
		if(creation_fichier_gitignore() != true)
		{
			printf_creation_fichier(GITIGNORE_FILE, 0);
		}
		else
		{
			printf_creation_fichier(GITIGNORE_FILE, 1);
		}
	}
	else if(strcmp(argv[1], "-help") == 0 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-H") == 0)
	{
		printf_aide();
	}
	else if(strcmp(argv[1], "-innosetup") == 0)
	{
		if(argc < 3)
		{
			printf_innosetup();
			return -1;			
		}

		ST_logator st;
		chargement_fichier_resource_h(RESOURCE_H_FILE, &st);

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
	else if(strcmp(argv[1], "-remove") == 0)
	{
		ST_logator st;
		chargement_fichier_resource_h(RESOURCE_H_FILE, &st);
		if(st.AppVersion == 1.0f)
		{
			printf_msg_empty();
			return -1;
		}

		st.AppVersion = st.AppVersion - 0.1f;

		if(remove_last_changelog_entry(CHANGELOG_FILE) != true)
		{
			return -1;
		}		

		if(write_fichier_resource_h(RESOURCE_H_FILE, &st) != true)
		{
			return -1;
		}
	}
	else if(strcmp(argv[1], "-view") == 0 || strcmp(argv[1], "-v") == 0)
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
