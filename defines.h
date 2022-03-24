#pragma once

#include <stdbool.h>
#include <mysql.h>

struct configuration {
	char* host;
	char* db_username;
	char* db_password;
	unsigned int port;
	char* database;

	char username[128];
	char password[128];
};

extern struct configuration conf;

extern int parse_config(char* path, struct configuration* conf);
extern bool yesOrNo(char* domanda, char yes, char no, bool predef, bool insensitive);
extern char multiChoice(char* domanda, char choices[], int num);
extern void print_error(MYSQL* conn, char* message);
extern void print_stmt_error(MYSQL_STMT* stmt, char* message);
extern void finish_with_error(MYSQL* conn, char* message);
extern void finish_with_stmt_error(MYSQL* conn, MYSQL_STMT* stmt, char* message, bool close_stmt);
extern bool setup_prepared_stmt(MYSQL_STMT** stmt, char* statement, MYSQL* conn);
extern void dump_result_set(MYSQL* conn, MYSQL_STMT* stmt, char* title);
extern void bind_par(MYSQL_BIND param[], char** parameters, char** types, int num_elements);
extern void set_binding_param(MYSQL_BIND* param, enum enum_field_types type, void* buffer, unsigned long len);
extern void run_as_warehouseman(MYSQL* conn);
extern void run_as_manager(MYSQL* conn);