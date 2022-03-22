#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

#include "defines.h"

//inserire nella variabile PATH il percorso della cartella del client
#define PATH "C:/Users/nicco/source/repos/VerdeSRL-client/"

static void add_supplier(MYSQL* conn) {

}

static void add_supply(MYSQL* conn) {

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
		printf("11) Quit\n\n");

		if (scanf("%d", &op) == EOF) {
			fprintf(stderr, "Unable to read from terminal\n");
			exit(EXIT_FAILURE);
		}

		switch (op) {
		case 1:
			printf("op 1\n");
			break;
		
		case 2:
			printf("op 2\n");
			break;

		case 3:
			printf("op 3\n");
			break;

		case 4:
			printf("op 4\n");
			break;

		case 5:
			printf("op 5\n");
			break;

		case 6:
			printf("op 6\n");
			break;

		case 7:
			printf("op 7\n");
			break;

		case 8:
			printf("op 8\n");
			break;

		case 9:
			printf("op 9\n");
			break;

		case 10:
			printf("op 10\n");
			break;

		case 11:
			return;

		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			abort();
		}

	}
}