#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sqlite3.h>
#include "operations.h"

sqlite3 *registered_db;		//Base de datos de usuarios registrados
sqlite3 *active_db;			//Base de datos de usuarios activos
char *err = 0;				//Variable de control
int registered_rc;
int active_rc;
char *sql_op;   //BORRAR
int exists = 0;
//LISTA DE STRINGS

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   char user[4] = "USER";
   for(i = 0; i<argc; i++) {
        if(strcmp(azColName[i],user)!=0)printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");  //Solo nos interesan las columnas que no son 'USER'
   }
   printf("\n");
   return 0;
}

static int tableExists(void *NotUsed, int argc, char **argv, char **azColName) {
   if(argc>0) exists = 1;
   else exists = 0;
   return 0;
}

void checkError(){
    if( registered_rc != SQLITE_OK || active_rc != SQLITE_OK){
        fprintf(stderr, "SQL error: %s\n", err);
        sqlite3_free(err);
    }
}

/*
*   REGISTER USER
*/
int registerUser(char *user)
{
    //Abrir la base de datos de registrados
    registered_rc = sqlite3_open("registered.db", &registered_db);

	if(registered_rc)fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(registered_db));

    //Comprobar si la tabla usuarios existe
    sql_op = "SELECT * FROM sqlite_master WHERE type='table' AND name='USERS';";
    registered_rc = sqlite3_exec(registered_db, sql_op, tableExists, 0, &err);
    checkError();   //Comprobar errores

    //Si la tabla de usuarios no existe la creamos
    if(exists == 0){
        sql_op = "CREATE TABLE USERS(USER TEXT PRIMARY KEY NOT NULL);";  //Creamos la tabla de usuarios

        registered_rc = sqlite3_exec(registered_db, sql_op, callback, 0, &err);
        checkError();   //Comprobar errores

        //Si la tabla de usuarios no existía asumimos que la de archivos tampoco
        sql_op = "CREATE TABLE FILES("                              \
                 "USER           TEXT   NOT NULL,"  \
                 "NAME           TEXT   NOT NULL,"  \
                 "DESCRIPTION    TEXT   NOT NULL,"  \
                 "PRIMARY KEY(USER, NAME));";

        registered_rc = sqlite3_exec(registered_db, sql_op, callback, 0, &err);
        checkError();   //Comprobar errores
    }
    exists=0;

    //Comprobar si el usuario existe
    char concat_sql_op[80];
    strcpy(concat_sql_op, "SELECT * FROM USERS WHERE user='");
    strcat(concat_sql_op, user);
    strcat(concat_sql_op, "';");
    registered_rc = sqlite3_exec(registered_db, concat_sql_op, tableExists, 0, &err);
    checkError();   //Comprobar errores

    //Si el usuario no existe le podemos registrar
    if(exists == 0){
        //Concatenamos la instrucción con el usuario deseado
        strcpy(concat_sql_op, "INSERT INTO USERS VALUES('");
        strcat(concat_sql_op, user);
        strcat(concat_sql_op, "');");

        registered_rc = sqlite3_exec(registered_db, concat_sql_op, callback, 0, &err);
        checkError();   //Comprobar errores
    }else{
        printf("User already exists!\n");
        return -1;
    }
    exists=0;

    sqlite3_close(registered_db); //Cerramos la base de datos
    return 1;
}

/*
*   CONNECT USER IP PORT
*/
int connectUser(char *user, char *ip, int port)
{
	//Abrir la base de datos de registrados
    registered_rc = sqlite3_open("registered.db", &registered_db);
	if(registered_rc) fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(registered_db));

    //Comprobar si el usuario existe
    char concat_sql_op[80];
    strcpy(concat_sql_op, "SELECT * FROM USERS WHERE user='");
    strcat(concat_sql_op, user);
    strcat(concat_sql_op, "';");
    registered_rc = sqlite3_exec(registered_db, concat_sql_op, tableExists, 0, &err);
    checkError();   //Comprobar errores

    //Si el usuario existe le podemos conectar
    if(exists == 1){
        
        //Abrir la base de datos de activos
        active_rc = sqlite3_open("active.db", &active_db);
	    if(active_rc)fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(active_db));
        exists=0;

        //Comprobar si la tabla usuarios existe
        sql_op = "SELECT * FROM sqlite_master WHERE type='table' AND name='USERS';";
        active_rc = sqlite3_exec(active_db, sql_op, tableExists, 0, &err);
        checkError();   //Comprobar errores

        //Si la tabla de usuarios no existe la creamos
        if(exists == 0){
            //Creamos la tabla de usuarios
            sql_op = "CREATE TABLE USERS("                              \
                     "USER           TEXT   PRIMARY KEY     NOT NULL,"  \
                     "IP             TEXT                   NOT NULL,"  \
                     "PORT           INT                   NOT NULL);";

            active_rc = sqlite3_exec(active_db, sql_op, callback, 0, &err);
            checkError();   //Comprobar errores

            //Si la tabla de usuarios no existía asumimos que la de archivos tampoco
            sql_op = "CREATE TABLE FILES("                              \
                     "USER           TEXT   NOT NULL,"  \
                     "NAME           TEXT   NOT NULL,"  \
                     "DESCRIPTION    TEXT   NOT NULL,"  \
                     "PRIMARY KEY(USER, NAME));";

            active_rc = sqlite3_exec(active_db, sql_op, callback, 0, &err);
            checkError();   //Comprobar errores
        }
        exists=0;

        //Comprobar si el usuario existe
        strcpy(concat_sql_op, "SELECT * FROM USERS WHERE user='");
        strcat(concat_sql_op, user);
        strcat(concat_sql_op, "';");
        active_rc = sqlite3_exec(active_db, concat_sql_op, tableExists, 0, &err);
        checkError();   //Comprobar errores

        //Si el usuario no existe le podemos registrar
        if(exists == 0){
            char str_port[3];
            sprintf(str_port,"%d",port);
            //Concatenamos la instrucción con el usuario deseado
            strcpy(concat_sql_op, "INSERT INTO USERS (USER,IP,PORT) VALUES('");
            strcat(concat_sql_op, user);
            strcat(concat_sql_op, "','");
            strcat(concat_sql_op, ip);
            strcat(concat_sql_op, "',");
            strcat(concat_sql_op, str_port);
            strcat(concat_sql_op, ");");
            active_rc = sqlite3_exec(active_db, concat_sql_op, callback, 0, &err);
            checkError();   //Comprobar errores

            //Extraemos todos los archivos del usuario de la base de datos de registrados
            strcpy(concat_sql_op, "SELECT * FROM FILES WHERE user='");
            strcat(concat_sql_op, user);
            strcat(concat_sql_op, "';");
            registered_rc = sqlite3_exec(registered_db, concat_sql_op, callback, 0, &err);
            checkError();   //Comprobar errores
        }else{
            printf("User is already connected!\n");
            return -1;
        }
        exists=0;


    }else{
        printf("User cannot be connected because it hasn't been registered yet!\n");
        return -1;
    }
    exists=0;

    //Cerramos las bases de datos
	sqlite3_close(registered_db);
	sqlite3_close(active_db);
    return -1;
}

int disconnectUser()
{
    //Abrir la base de datos de registrados
    registered_rc = sqlite3_open("registered.db", &registered_db);
    active_rc = sqlite3_open("active.db", &active_db);
	if(registered_rc)fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(registered_db));
    //Abrir la base de datos de activos
	if(active_rc)fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(active_db));

    
	sqlite3_close(registered_db); //Cerramos la base de datos
	sqlite3_close(active_db);

    return -1;
}

int publishFile()
{
    //Abrir la base de datos de activos
    active_rc = sqlite3_open("active.db", &active_db);
	if(active_rc)fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(active_db));


	sqlite3_close(active_db); //Cerramos la base de datos
    return -1;
}

int deleteFile()
{
    //Abrir la base de datos de activos
    active_rc = sqlite3_open("active.db", &active_db);
	if(active_rc)fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(active_db));


	sqlite3_close(active_db); //Cerramos la base de datos
    return -1;
}

int list_users()
{
    //Abrir la base de datos de activos
    active_rc = sqlite3_open("active.db", &active_db);
	if(active_rc)fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(active_db));


	sqlite3_close(active_db); //Cerramos la base de datos
    return -1;
}

int list_content()
{
    //Abrir la base de datos de activos
    active_rc = sqlite3_open("active.db", &active_db);
	if(active_rc)fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(active_db));


	sqlite3_close(active_db); //Cerramos la base de datos
    return -1;
}