#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#define LOG_VERSION "1.4"
#define log_folder "logs"
#define log_file "logs/Erreur.log"

char* GetTime(void);

int VerifExiste(char* emplacement);
int CreationFichierLog(char* log_texte);
int DemandeAccord(char* FichierNom);
int DemandeAccord2(void);
int CreationRepertoire(char* repertoire);

#endif /* !LOG_H_INCLUDED */
