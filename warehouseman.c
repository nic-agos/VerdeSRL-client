#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

#include "defines.h"

//inserire nella variabile PATH il percorso della cartella del client
#define PATH "C:/Users/nicco/source/repos/VerdeSRL-client/"

static void add_supplier(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[7];

	char codice[12];
	char nome[32];
	char cf[18];
	char via[52];
	char civicoStr[6];
	char citta[27];
	char nazione[22];
	
	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call add_supplier(?, ?, ?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize supplier insertion statement\n", false);
	}

	getchar();

	printf("\nSupplier code: ");
	fflush(stdout);
	fgets(codice, 12, stdin);
	codice[strlen(codice) - 1] = '\0';

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, codice, strlen(codice));

	printf("Name: ");
	fflush(stdout);
	fgets(nome, 31, stdin);
	nome[strlen(nome) - 1] = '\0';

	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, nome, strlen(nome));

	printf("Fiscal code: ");
	fflush(stdout);
	fgets(cf, 18, stdin);
	cf[strlen(cf) - 1] = '\0';

	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, cf, strlen(cf));

	printf("Street: ");
	fflush(stdout);
	fgets(via, 51, stdin);
	via[strlen(via) - 1] = '\0';

	set_binding_param(&param[3], MYSQL_TYPE_VAR_STRING, via, strlen(via));

	printf("Street number: ");
	fflush(stdout);
	fgets(civicoStr, 6, stdin);
	civicoStr[strlen(civicoStr) - 1] = '\0';

	int civico = atoi(civicoStr);
	set_binding_param(&param[4], MYSQL_TYPE_LONG, &civico, sizeof(civico));

	printf("City: ");
	fflush(stdout);
	fgets(citta, 26, stdin);
	citta[strlen(citta) - 1] = '\0';

	set_binding_param(&param[5], MYSQL_TYPE_VAR_STRING, citta, strlen(citta));

	printf("Nation: ");
	fflush(stdout);
	fgets(nazione, 21, stdin);
	nazione[strlen(nazione) - 1] = '\0';

	set_binding_param(&param[6], MYSQL_TYPE_VAR_STRING, nazione, strlen(nazione));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for add_supplier\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while adding supplier.\n");
	}
	else {
		printf("\nSupplier correctly added...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void add_supply(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[3];

	char fornitore[12];
	char pianta[8];
	char quantitaStr[8];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call add_supply(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize supply insertion statement\n", false);
	}

	getchar();

	printf("\nSupplier code: ");
	fflush(stdout);
	fgets(fornitore, 12, stdin);
	fornitore[strlen(fornitore) - 1] = '\0';

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, fornitore, strlen(fornitore));

	printf("Plant code: ");
	fflush(stdout);
	fgets(pianta, 8, stdin);
	pianta[strlen(pianta) - 1] = '\0';

	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, pianta, strlen(pianta));

	printf("Required amount: ");
	fflush(stdout);
	fgets(quantitaStr, 8, stdin);

	int quantita = atoi(quantitaStr);

	set_binding_param(&param[2], MYSQL_TYPE_LONG, &quantita, sizeof(quantita));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for add_supply\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while adding supply.\n");
	}
	else {
		printf("\nSupply correctly added...\n");
	}

	mysql_stmt_close(prepared_stmt);

}

static void add_treated_plant(MYSQL* conn) {

}

static void add_package(MYSQL* conn) {

}

static void get_suppliers(MYSQL* conn) {

}

static void get_supplies(MYSQL* conn) {

}

static void get_plants_stock(MYSQL* conn) {

}

static void get_plant_suppliers(MYSQL* conn) {

}

static void get_pack_info(MYSQL* conn) {

}

static void get_order_remaining_plants(MYSQL* conn) {

}

static void remove_package(MYSQL* conn) {

}

void run_as_warehouseman(MYSQL* conn) {
	char* path;
	int op;

	printf("Switching to warehouseman role...\n");
	
	path = (char*)malloc(256 * sizeof(char));
	
	if (path == NULL) {
		fprintf(stderr, "Unable to allocate space for path\n");
		exit(EXIT_FAILURE);
	}

	strcpy(path, PATH);
	strcat(path, "users/magazziniere.json");

	if (!parse_config(path, &conf)) {
		fprintf(stderr, "Unable to load warehouseman configuration\n");
		exit(EXIT_FAILURE);
	}

	if (mysql_change_user(conn, conf.db_username, conf.db_password, conf.database)) {
		fprintf(stderr, "mysql_change_user() failed\n");
		exit(EXIT_FAILURE);
	}

	while (true) {
	
		printf("*** What should I do for you? ***\n\n");
		printf("1) Add supplier\n");
		printf("2) Add supply\n");
		printf("3) Add treated plant for supplier\n");
		printf("4) Add package\n");
		printf("5) Get suppliers\n");
		printf("6) Get supplies\n");
		printf("7) Get plants stock\n");
		printf("8) Get plant suppliers\n");
		printf("9) Get pack info\n");
		printf("10) Get order remaining plant\n");
		printf("11) Remove package\n");
		printf("12) Quit\n\n");

		if (scanf("%d", &op) == EOF) {
			fprintf(stderr, "Unable to read from terminal\n");
			exit(EXIT_FAILURE);
		}

		switch (op) {
		case 1:
			printf("\033[2J\033[H");
			add_supplier(conn);
			break;
		
		case 2:
			printf("\033[2J\033[H");
			add_supply(conn);
			break;

		case 3:
			printf("\033[2J\033[H");
			printf("op 3\n");
			break;

		case 4:
			printf("\033[2J\033[H");
			printf("op 4\n");
			break;

		case 5:
			printf("\033[2J\033[H");
			printf("op 5\n");
			break;

		case 6:
			printf("\033[2J\033[H");
			printf("op 6\n");
			break;

		case 7:
			printf("\033[2J\033[H");
			printf("op 7\n");
			break;

		case 8:
			printf("\033[2J\033[H");
			printf("op 8\n");
			break;

		case 9:
			printf("\033[2J\033[H");
			printf("op 9\n");
			break;

		case 10:
			printf("\033[2J\033[H");
			printf("op 10\n");
			break;
		
		case 11:
			printf("\033[2J\033[H");
			printf("op 11\n");
			break;

		case 12:
			return;

		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			abort();
		}

	}
}