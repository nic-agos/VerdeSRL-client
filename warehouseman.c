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
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[2];

	char fornitore[12];
	char pianta[8];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call add_treated_plant(?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize treated plant adding statement\n", false);
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

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for add_treated_plant\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while adding treated plant.\n");
	}
	else {
		printf("\nTreated plant correctly added...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void add_package(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[4];

	char ordine[42];
	char piante[502] = "";
	char quantita[102] = "";
	int num_plants = 0;

	char temp_plant[10];
	char temp_quant[10];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call add_package(?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize package adding statement\n", false);
	}

	getchar();

	printf("Order code: ");
	fflush(stdout);
	fgets(ordine, 42, stdin);
	ordine[strlen(ordine) - 1] = '\0';

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, ordine, strlen(ordine));

	while (1) {
		printf("Code of the plant to be included in the package (quit to stop): ");
		fflush(stdout);
		fgets(temp_plant, 8, stdin);
		temp_plant[strlen(temp_plant) - 1] = '\0';

		if (strcmp(temp_plant, "quit") == 0) {
			break;
		}

		strcat(piante, temp_plant);
		strcat(piante, ",");

		printf("Quantity of the plant to add to the package: ");
		fflush(stdout);
		fgets(temp_quant, 8, stdin);
		temp_quant[strlen(temp_quant) - 1] = '\0';
		strcat(quantita, temp_quant);
		strcat(quantita, ",");

		num_plants = num_plants + 1;
	}

	if (strlen(piante) < 1) {
		fprintf(stderr, "\nPackage must contain at least one plant!\n");
		exit(EXIT_FAILURE);
	}

	piante[strlen(piante) - 1] = '\0';
	quantita[strlen(quantita) - 1] = '\0';

	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, piante, strlen(piante));

	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, quantita, strlen(quantita));

	set_binding_param(&param[3], MYSQL_TYPE_LONG, &num_plants, sizeof(num_plants));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for add_package\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while adding package.\n");
	}
	else {
		printf("\nPackage correctly added...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void get_suppliers(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;

	if (!setup_prepared_stmt(&prepared_stmt, "call get_suppliers()", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize get suppliers statement\n", false);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while getting suppliers.");
	}

	dump_result_set(conn, prepared_stmt, "\n\nSuppliers:\n");

	mysql_stmt_next_result(prepared_stmt);

	dump_result_set(conn, prepared_stmt, "\n\nTreated plants:\n");

	mysql_stmt_next_result(prepared_stmt);

	mysql_stmt_close(prepared_stmt);
}

static void get_supplies(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[1];

	char fornitore[12];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call get_supplies(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize get supplies statement\n", false);
	}

	getchar();
	printf("Supplier code: ");
	fflush(stdout);
	fgets(fornitore, 12, stdin);

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, fornitore, strlen(fornitore));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for get_supplies\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while getting supplies.");
	}

	dump_result_set(conn, prepared_stmt, "\n\nSupplies:\n");

	mysql_stmt_next_result(prepared_stmt);

	mysql_stmt_close(prepared_stmt);
}

static void get_plants_stock(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;

	if (!setup_prepared_stmt(&prepared_stmt, "call get_plants_stock()", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize get plants stock statement\n", false);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while getting customer orders.");
	}

	dump_result_set(conn, prepared_stmt, "\n\nPlants stock:\n");

	mysql_stmt_next_result(prepared_stmt);

	mysql_stmt_close(prepared_stmt);
}

static void get_plant_suppliers(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[1];

	char pianta[8];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call get_plant_suppliers(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize get plant suppliers statement\n", false);
	}

	getchar();
	printf("Plant code: ");
	fflush(stdout);
	fgets(pianta, 8, stdin);

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, pianta, strlen(pianta));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for get_plant_suppliers\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while getting plant suppliers.");
	}

	dump_result_set(conn, prepared_stmt, "\n\nPlant suppliers:\n");

	mysql_stmt_next_result(prepared_stmt);

	mysql_stmt_close(prepared_stmt);
}

static void get_pack_info(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[1];

	char pacco_str[8];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call get_pack_info(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize get pack info statement\n", false);
	}

	getchar();
	printf("Package code: ");
	fflush(stdout);
	fgets(pacco_str, 8, stdin);

	int pacco = atoi(pacco_str);

	set_binding_param(&param[0], MYSQL_TYPE_LONG, &pacco, sizeof(pacco));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for get_pack_info\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while getting pack info.");
	}

	dump_result_set(conn, prepared_stmt, "\n\nPackage info:\n");

	mysql_stmt_next_result(prepared_stmt);

	mysql_stmt_close(prepared_stmt);
}

static void get_orders_remaining_plants(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;

	if (!setup_prepared_stmt(&prepared_stmt, "call get_orders_remaining_plants()", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize get orders remaining plants statement\n", false);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while getting orders remaining plants.");
	}

	dump_result_set(conn, prepared_stmt, "\n\nOrders remaining plants:\n");

	mysql_stmt_next_result(prepared_stmt);

	mysql_stmt_close(prepared_stmt);

}

static void remove_package(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[1];

	char pacco_str[8];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call remove_package(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize package removing statement\n", false);
	}

	getchar();

	printf("Package code: ");
	fflush(stdout);
	fgets(pacco_str, 8, stdin);
	
	int pacco = atoi(pacco_str);

	set_binding_param(&param[0], MYSQL_TYPE_LONG, &pacco, sizeof(pacco));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for remove_package\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while removing package.\n");
	}
	else {
		printf("\nPackage correctly removed...\n");
	}

	mysql_stmt_close(prepared_stmt);


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
	
		printf("\n*** What should I do for you? ***\n\n");
		printf("1) Add supplier\n");
		printf("2) Add supply\n");
		printf("3) Add treated plant for supplier\n");
		printf("4) Add package\n");
		printf("5) Get suppliers\n");
		printf("6) Get supplies\n");
		printf("7) Get plants stock\n");
		printf("8) Get plant suppliers\n");
		printf("9) Get pack info\n");
		printf("10) Get orders remaining plants\n");
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
			add_treated_plant(conn);
			break;

		case 4:
			printf("\033[2J\033[H");
			add_package(conn);
			break;

		case 5:
			printf("\033[2J\033[H");
			get_suppliers(conn);
			break;

		case 6:
			printf("\033[2J\033[H");
			get_supplies(conn);
			break;

		case 7:
			printf("\033[2J\033[H");
			get_plants_stock(conn);
			break;

		case 8:
			printf("\033[2J\033[H");
			get_plant_suppliers(conn);
			break;

		case 9:
			printf("\033[2J\033[H");
			get_pack_info(conn);
			break;

		case 10:
			printf("\033[2J\033[H");
			get_orders_remaining_plants(conn);
			break;
		
		case 11:
			printf("\033[2J\033[H");
			remove_package(conn);
			break;

		case 12:
			return;

		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			abort();
		}

	}
}