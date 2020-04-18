#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "operations.h"

sqlite3 *registered_db;		//Base de datos de usuarios registrados
sqlite3 *active_db;			//Base de datos de usuarios activos
char *err = 0;				//Variable de control
int registered_rc;
int active_rc;
char *sql_op;
int exists = 0;

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
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
    if( registered_rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", err);
        sqlite3_free(err);
    }
}

int registerUser(char *user)
{
    printf("holi");
	//Abrir la base de datos de registrados
    registered_rc = sqlite3_open("registered.db", &registered_db);

	if(registered_rc)fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(registered_db));

    //Creamos la tabla de usuarios
    sql_op = "SELECT name FROM sqlite_master WHERE type='table' AND name='USERS';";

    registered_rc = sqlite3_exec(registered_db, sql_op, tableExists, 0, &err);
    checkError();   //Comprobar errores

    //Si la tabla de usuarios no existe la creamos
    if(exists == 0){
        sql_op = "CREATE TABLE USERS(USER TEXT PRIMARY KEY);";  //Creamos la tabla de usuarios

        registered_rc = sqlite3_exec(registered_db, sql_op, callback, 0, &err);
        checkError();   //Comprobar errores

        //Si la tabla de usuarios no existía asumimos que la de archivos tampoco
        sql_op = "CREATE TABLE FILES("  \
                 "USER TEXT PRIMARY KEY     NOT NULL," \
                 "NAME           TEXT    NOT NULL," \
                 "DESCRIPTION           TEXT    NOT NULL);";

        registered_rc = sqlite3_exec(registered_db, sql_op, callback, 0, &err);
        checkError();   //Comprobar errores
    }

    sql_op = strcat("INSERT INTO USERS(USER) " \
             "VALUES(", user);
    sql_op = strcat(sql_op, ");");

    registered_rc = sqlite3_exec(registered_db, sql_op, callback, 0, &err);
    checkError();   //Comprobar errores

	sqlite3_close(registered_db); //Cerramos la base de datos //SEGMENTATION FAULT AQUI----------------------------------------------------------------------
    return 1;
}

int connectUser()
{
	//Abrir la base de datos de registrados
    registered_rc = sqlite3_open("registered.db", &registered_db);
    active_rc = sqlite3_open("active.db", &active_db);
	if(registered_rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(registered_db));
      	return(0);
	}else fprintf(stderr, "Opened database successfully\n");
    //Abrir la base de datos de activos
	if(active_rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(active_db));
      	return(0);
	}else fprintf(stderr, "Opened database successfully\n");


	sqlite3_close(registered_db); //Cerramos la base de datos
	sqlite3_close(active_db);

    return -1;
}

int disconnectUser()
{
    //Abrir la base de datos de registrados
    registered_rc = sqlite3_open("registered.db", &registered_db);
    active_rc = sqlite3_open("active.db", &active_db);
	if(registered_rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(registered_db));
      	return(0);
	}else fprintf(stderr, "Opened database successfully\n");
    //Abrir la base de datos de activos
	if(active_rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(active_db));
      	return(0);
	}else fprintf(stderr, "Opened database successfully\n");

    
	sqlite3_close(registered_db); //Cerramos la base de datos
	sqlite3_close(active_db);

    return -1;
}

int publishFile()
{
    //Abrir la base de datos de activos
    active_rc = sqlite3_open("active.db", &active_db);
	if(active_rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(active_db));
      	return(0);
	}else fprintf(stderr, "Opened database successfully\n");


	sqlite3_close(active_db); //Cerramos la base de datos

    return -1;
}

int deleteFile()
{
    //Abrir la base de datos de activos
    active_rc = sqlite3_open("active.db", &active_db);
	if(active_rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(active_db));
      	return(0);
	}else fprintf(stderr, "Opened database successfully\n");


	sqlite3_close(active_db); //Cerramos la base de datos

    return -1;
}

int list_users()
{
    //Abrir la base de datos de activos
    active_rc = sqlite3_open("active.db", &active_db);
	if(active_rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(active_db));
      	return(0);
	}else fprintf(stderr, "Opened database successfully\n");


	sqlite3_close(active_db); //Cerramos la base de datos

    return -1;
}

int lsit_content()
{
    //Abrir la base de datos de activos
    active_rc = sqlite3_open("active.db", &active_db);
	if(active_rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(active_db));
      	return(0);
	}else fprintf(stderr, "Opened database successfully\n");


	sqlite3_close(active_db); //Cerramos la base de datos

    return -1;
}