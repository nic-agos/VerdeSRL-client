#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

#include "defines.h"

//inserire nella variabile PATH il percorso della cartella del client
#define PATH "C:/Users/nicco/source/repos/VerdeSRL-client/"

typedef enum {
	MANAGER = 1,
	WAREHOUSEMAN,
	FAILED_LOGIN
} role_t;

struct configuration conf;

static MYSQL* conn;

static role_t attempt_login(MYSQL* conn, char* username, char* password) {

	/*dichiaro il prepared statement*/
	MYSQL_STMT* login_procedure;

	/*dichiaro un vettore di struct MYSQL_BIND perchè la procedura prevede 3 paramentri*/
	MYSQL_BIND param[3]; // Used both for input and output

	int role = 0;

	if (!setup_prepared_stmt(&login_procedure, "call login(?, ?, ?)", conn)) {
		print_stmt_error(login_procedure, "Unable to initialize login statement\n");
		goto err2;
	}

	/*azzero il contenuto del vettore per evitare di leggere vecchi parametri*/
	memset(param, 0, sizeof(param));

	// Prepare parameters
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, username, strlen(username));

	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, password, strlen(password));

	/*bisogna fare il binding anche dei parametri in output*/
	set_binding_param(&param[2], MYSQL_TYPE_LONG, &role, sizeof(role));

	/*collego i parametri al prepared statement*/
	if (mysql_stmt_bind_param(login_procedure, param) != 0) { // Note _param
		print_stmt_error(login_procedure, "Could not bind parameters for login");
		goto err;
	}

	// Run procedure
	if (mysql_stmt_execute(login_procedure) != 0) {
		print_stmt_error(login_procedure, "Could not execute login procedure");
		goto err;
	}

	// Prepare output parameters
	memset(param, 0, sizeof(param));

	/*eseguo il binding al contrario, per i parametri in output*/
	set_binding_param(&param[0], MYSQL_TYPE_LONG, &role, sizeof(role));

	if (mysql_stmt_bind_result(login_procedure, param)) {
		print_stmt_error(login_procedure, "Could not retrieve output parameter");
		goto err;
	}

	// Retrieve output parameter
	if (mysql_stmt_fetch(login_procedure)) {
		print_stmt_error(login_procedure, "Could not buffer results");
		goto err;
	}

	mysql_stmt_close(login_procedure);
	return role;

err:
	mysql_stmt_close(login_procedure);
err2:
	return FAILED_LOGIN;
}

int main(void) {
	role_t role;
	char *path;

	printf("Welcome on VerdeSRL client\n");

	path = (char *)malloc(256*sizeof(char));
	if (path == NULL) {
		fprintf(stderr, "Unable to allocate space for path\n");
		exit(EXIT_FAILURE);
	}
	
	strcpy(path, PATH);
	strcat(path, "users/login.json");

	if (!parse_config(path, &conf)) {
		fprintf(stderr, "Unable to load login configuration\n");
		exit(EXIT_FAILURE);
	}

	conn = mysql_init(NULL);
	if (conn == NULL) {
		fprintf(stderr, "mysql_init() failed (probably out of memory)\n");
		exit(EXIT_FAILURE);
	}

	if (mysql_real_connect(conn, conf.host, conf.db_username, conf.db_password, conf.database, conf.port, NULL, CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS) == NULL) {
		fprintf(stderr, "mysql_real_connect() failed\n");
		mysql_close(conn);
		exit(EXIT_FAILURE);
	}

	printf("Username: ");
	fflush(stdout);
	fgets(conf.username, 128, stdin);
	conf.username[strlen(conf.username) - 1] = '\0';
	printf("Password: ");
	fflush(stdout);
	fgets(conf.password, 128, stdin);
	conf.password[strlen(conf.password) - 1] = '\0';

	/*provo ad eseguire il login con le credenziali ricevute in input da tastiera*/
	role = attempt_login(conn, conf.username, conf.password);

	switch (role) {
	case MANAGER:
		printf("\033[2J\033[H");
		run_as_manager(conn);
		break;

	case WAREHOUSEMAN:
		printf("\033[2J\033[H");
		run_as_warehouseman(conn);
		break;

	case FAILED_LOGIN:
		fprintf(stderr, "Invalid credentials\n");
		exit(EXIT_FAILURE);
		break;

	default:
		fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
		abort();
	}

	printf("Bye!\n");

	mysql_close(conn);
	return 0;
}