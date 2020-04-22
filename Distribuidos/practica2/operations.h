void startServer();
int registerUser(char *localUser);
int unregisterUser(char *localUser);
int connectUser(char *localUser, char *ip, int port);
int disconnectUser(char *user);
int publishFile(char *ip, char *file, char *description);
int deleteFile(char *ip, char *file);
int list_users();
int list_content(char *localUser);