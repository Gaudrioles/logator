#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#define LOG_VERSION "2.0"
#define log_folder "logs"
#define log_file "logs/Erreur.log"

char* GetTime(void);

int VerifExiste(char* emplacement);
int CreationFichierLog(char* log_texte);
int VerifAccord(char* message);
int CreationRepertoire(char* repertoire);

#endif /* !LOG_H_INCLUDED */
