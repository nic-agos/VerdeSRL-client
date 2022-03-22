#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

#include "defines.h"

//inserire nella variabile PATH il percorso della cartella del client
#define PATH "C:/Users/nicco/source/repos/VerdeSRL-client/"

static void add_customer(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[10];
	int num_params = 10;

	char **array;
	char **types;

	char op;
	char options[2] = { '1', '2'};
	char options_bis[3] = { '1', '2', '3'};

	char id[18];
	char nome[31];
	char cognome[31];
	char via[51];
	char civicoStr[6];
	char citta[26];
	char nazione[21];
	char tipo[31];
	char recapito[71];
	char mezzoComunicazione[21];

	memset(param, 0, sizeof(param));

	array = malloc(num_params * sizeof(void*));
	types = malloc(num_params * sizeof(char*));

	if (array == NULL || types == NULL) {
		fprintf(stderr, "Unable to allocate space\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < num_params; i++) {
		types[i] = malloc(512 * sizeof(char));
		array[i] = malloc(512 * sizeof(char));
		if (array[i] == NULL || types[i] == NULL) {
			fprintf(stderr, "Unable to allocate space for array n. %d\n", i);
			exit(EXIT_FAILURE);
		}
	}

	if (!setup_prepared_stmt(&prepared_stmt, "call add_customer(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize customer insertion statement\n", false);
	}

	printf("\nChoose the type of customer:\n");
	printf("\t1) Privato\n");
	printf("\t2) Rivendita\n");
	fflush(stdout);

	op = multiChoice("Select type of customer", options, 2);

	switch (op) {
	case '1':
		strcpy(tipo, "privato");
		break;

	case '2':
		strcpy(tipo, "rivendita");
		break;

	default:
		fprintf(stderr, "\nInvalid condition at %s:%d\n", __FILE__, __LINE__);
		abort();
	}

	strcpy(array[7], tipo);
	strcpy(types[7], "string");

	if(strcmp(tipo, "privato") == 0) {
		printf("\nFiscal code: ");
		fflush(stdout);
	}
	else {
		printf("\nVAT number: ");
		fflush(stdout);
	}

	fgets(id, 18, stdin);
	id[strlen(id) - 1] = '\0';

	strcpy(array[0], id);
	strcpy(types[0], "string");

	printf("Name: ");
	fflush(stdout);
	fgets(nome, 31, stdin);
	nome[strlen(nome) - 1] = '\0';

	strcpy(array[1], nome);
	strcpy(types[1], "string");

	if(strcmp(tipo, "privato") == 0) {
		printf("Surname: ");
		fflush(stdout);
		fgets(cognome, 31, stdin);
		cognome[strlen(cognome) - 1] = '\0';

		strcpy(array[2], cognome);
		strcpy(types[2], "string");
	}

	printf("Street: ");
	fflush(stdout);
	fgets(via, 51, stdin);
	via[strlen(via) - 1] = '\0';

	strcpy(array[3], via);
	strcpy(types[3], "string");

	printf("Street number: ");
	fflush(stdout);
	fgets(civicoStr, 6, stdin);
	civicoStr[strlen(civicoStr) - 1] = '\0';

	strcpy(array[4], civicoStr);
	strcpy(types[4], "int");
	
	printf("City: ");
	fflush(stdout);
	fgets(citta, 26, stdin);
	citta[strlen(citta) - 1] = '\0';

	strcpy(array[5], citta);
	strcpy(types[5], "string");

	printf("Nation: ");
	fflush(stdout);
	fgets(nazione, 21, stdin);
	nazione[strlen(nazione) - 1] = '\0';

	strcpy(array[6], nazione);
	strcpy(types[6], "string");

	printf("\nChoose the type of contact:\n");
	printf("\t1) Telephone\n");
	printf("\t2) Mobile\n");
	printf("\t3) E-mail\n");
	fflush(stdout);
	op = multiChoice("Select type of contact", options_bis, 3);

	printf("telephone/mobile/e-mail: ");
	fflush(stdout);
	fgets(recapito, 71, stdin);
	recapito[strlen(recapito) - 1] = '\0';

	strcpy(array[8], recapito);
	strcpy(types[8], "string");

	switch (op) {
	case '1':
		strcpy(mezzoComunicazione, "telefono");
		break;

	case '2':
		strcpy(mezzoComunicazione, "cellulare");
		break;

	case '3':
		strcpy(mezzoComunicazione, "e-mail");
		break;

	default:
		fprintf(stderr, "\nInvalid condition at %s:%d\n", __FILE__, __LINE__);
		abort();
	}

	strcpy(array[9], mezzoComunicazione);
	strcpy(types[9], "string");


	bind_par(param, array, types, num_params);
	
	
	int civico = atoi(civicoStr);
	param[4].buffer_type = MYSQL_TYPE_LONG;
	param[4].buffer = &civico;
	param[4].buffer_length = sizeof(civico);
	

	int temp = *(int*)param[4].buffer;
	printf("mem add in manager: %p\n", param[4].buffer);
	printf("value in manager: %d\n", temp);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for add_customer\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while adding customer.");
	}
	else {
		printf("\nCustomer correctly added...\n");
	}

	mysql_stmt_close(prepared_stmt);

}

static void add_referent(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[6];
	int num_params = 6;
	char op;
	char options[3] = { '1', '2', '3' };

	char** array;
	char** types;

	char cf[18];
	char nome[32];
	char cognome[32];
	char cliente[18];
	char recapito[72];
	char mezzoComunicazione[22];

	memset(param, 0, sizeof(param));

	array = malloc(num_params * sizeof(void*));
	types = malloc(num_params * sizeof(char*));

	if (array == NULL || types == NULL) {
		fprintf(stderr, "Unable to allocate space\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < num_params; i++) {
		types[i] = malloc(512 * sizeof(char));
		array[i] = malloc(512 * sizeof(char));
		if (array[i] == NULL || types[i] == NULL) {
			fprintf(stderr, "Unable to allocate space for array n. %d\n", i);
			exit(EXIT_FAILURE);
		}
	}

	if (!setup_prepared_stmt(&prepared_stmt, "call add_referent(?, ?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize referent insertion statement\n", false);
	}

	getchar();
	printf("\nFiscal Code: ");
	fgets(cf, 18, stdin);
	cf[strlen(cf) - 1] = '\0';

	strcpy(array[0], cf);
	strcpy(types[0], "string");

	printf("Name: ");
	fflush(stdout);
	fgets(nome, 31, stdin);
	nome[strlen(nome) - 1] = '\0';

	strcpy(array[1], nome);
	strcpy(types[1], "string");

	printf("Surname: ");
	fflush(stdout);
	fgets(cognome, 31, stdin);
	cognome[strlen(cognome) - 1] = '\0';

	strcpy(array[2], cognome);
	strcpy(types[2], "string");

	printf("Customer's fiscal Code: ");
	fflush(stdout);
	fgets(cliente, 18, stdin);
	cliente[strlen(cliente) - 1] = '\0';

	strcpy(array[3], cliente);
	strcpy(types[3], "string");

	printf("\nChoose the type of contact:\n");
	printf("\t1) Telephone\n");
	printf("\t2) Mobile\n");
	printf("\t3) E-mail\n");
	fflush(stdout);
	op = multiChoice("Select type of contact", options, 3);

	printf("telephone/mobile/e-mail: ");
	fflush(stdout);
	fgets(recapito, 71, stdin);
	recapito[strlen(recapito) - 1] = '\0';

	strcpy(array[4], recapito);
	strcpy(types[4], "string");

	switch (op) {
	case '1':
		strcpy(mezzoComunicazione, "telefono");
		break;

	case '2':
		strcpy(mezzoComunicazione, "cellulare");
		break;

	case '3':
		strcpy(mezzoComunicazione, "e-mail");
		break;

	default:
		fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
		abort();
	}

	strcpy(array[5], mezzoComunicazione);
	strcpy(types[5], "string");

	bind_par(param, array, types, num_params);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for add_referent\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while adding referent.");
	}
	else {
		printf("Referent correctly added...\n");
	}

	mysql_stmt_close(prepared_stmt);

}

static void add_plant(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[8];
	int num_params = 8;
	char op;
	char options[7] = { '1', '2', '3', '4', '5', '6', '7'};

	char** array;
	char** types;

	char codice[8];
	char nomeComune[32];
	char nomeLatino[32];
	char giacenzaStr[7];
	char prezzoStr[7];
	char colore[22];
	char tipo[502] = "";

	int chosen[6];

	memset(param, 0, sizeof(param));

	array = malloc(num_params * sizeof(void*));
	types = malloc(num_params * sizeof(char*));

	if (array == NULL || types == NULL) {
		fprintf(stderr, "Unable to allocate space\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < num_params; i++) {
		types[i] = malloc(512 * sizeof(char));
		array[i] = malloc(512 * sizeof(char));
		if (array[i] == NULL || types[i] == NULL) {
			fprintf(stderr, "Unable to allocate space for array n. %d\n", i);
			exit(EXIT_FAILURE);
		}
	}

	if (!setup_prepared_stmt(&prepared_stmt, "call add_plant(?, ?, ?, ?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize plant insertion statement\n", false);
	}

	getchar();
	printf("Code: ");
	fflush(stdout);
	fgets(codice, 8, stdin);
	codice[strlen(codice) - 1] = '\0';

	strcpy(array[0], codice);
	strcpy(types[0], "string");

	printf("Common name: ");
	fflush(stdout);
	fgets(nomeComune, 31, stdin);
	nomeComune[strlen(nomeComune) - 1] = '\0';

	strcpy(array[1], nomeComune);
	strcpy(types[1], "string");

	printf("Latin name: ");
	fflush(stdout);
	fgets(nomeLatino, 31, stdin);
	nomeLatino[strlen(nomeLatino) - 1] = '\0';

	strcpy(array[2], nomeLatino);
	strcpy(types[2], "string");


	int chosenNum = 0;

	do {
		printf("\nChoose the type of plant:\n");
		printf("\t1) Exotic\n");
		printf("\t2) Not exotic\n");
		printf("\t3) Flowering\n");
		printf("\t4) Green\n");
		printf("\t5) Indoor\n");
		printf("\t6) Outdoor\n");
		printf("\t7) Stop choose\n");
		fflush(stdout);
		op = multiChoice("Select type of plant", options, 7);

		switch (op) {
		case '1':
			for (int i = 0; i < chosenNum; i++) {
				if (chosen[i] == 2) {
					fprintf(stderr, "Plant cannot be both exotic and non-exotic\n");
					abort();
				}
			}
			chosen[chosenNum] = 1;
			strcat(tipo, "esotica,");
			chosenNum = chosenNum + 1;
			break;

		case '2':
			for (int i = 0; i < chosenNum; i++) {
				if (chosen[i] == 1) {
					fprintf(stderr, "Plant cannot be both exotic and non-exotic\n");
					abort();
				}
			}
			chosen[chosenNum] = 2;
			strcat(tipo, "non esotica,");
			chosenNum = chosenNum + 1;
			break;

		case '3':
			for (int i = 0; i < chosenNum; i++) {
				if (chosen[i] == 4) {
					fprintf(stderr, "Plant cannot be both flowered and green\n");
					abort();
				}
			}
			chosen[chosenNum] = 3;
			strcat(tipo, "fiorita,");
			chosenNum = chosenNum + 1;
			break;

		case '4':
			for (int i = 0; i < chosenNum; i++) {
				if (chosen[i] == 3) {
					fprintf(stderr, "Plant cannot be both flowered and green\n");
					abort();
				}
			}
			chosen[chosenNum] = 4;
			strcat(tipo, "verde,");
			chosenNum = chosenNum + 1;
			break;

		case '5':
			for (int i = 0; i < chosenNum; i++) {
				if (chosen[i] == 6) {
					fprintf(stderr, "Plant cannot be both indoor and outdoor\n");
					abort();
				}
			}
			chosen[chosenNum] = 5;
			strcat(tipo, "interno,");
			chosenNum = chosenNum + 1;
			break;

		case '6':
			for (int i = 0; i < chosenNum; i++) {
				if (chosen[i] == 5) {
					fprintf(stderr, "Plant cannot be both indoor and outdoor\n");
					abort();
				}
			}
			chosen[chosenNum] = 6;
			strcat(tipo, "esterno,");
			chosenNum = chosenNum + 1;
			break;

		case '7':
			break;

		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			abort();
		}
		
	} while (op != '7');

	if (chosenNum < 1) {
		fprintf(stderr, "You have to choose at least one type for the plant");
		abort();
	}

	tipo[strlen(tipo) - 1] = '\0';

	printf("Selected types: %s\n", tipo);

	strcpy(array[6], tipo);
	strcpy(types[6], "string");

	param[7].buffer_type = MYSQL_TYPE_LONG;
	param[7].buffer = &chosenNum;
	param[7].buffer_length = sizeof(chosenNum);


	for (int i = 0; i < chosenNum; i++) {
		if (chosen[i] == 3) {
			printf("Plant color: ");
			fflush(stdout);
			fgets(colore, 21, stdin);
			colore[strlen(colore) - 1] = '\0';

			strcpy(array[5], colore);
			strcpy(types[5], "string");
		}
	}

	printf("Price: ");
	fflush(stdout);
	fgets(prezzoStr, 6, stdin);

	int prezzo = atoi(prezzoStr);

	param[4].buffer_type = MYSQL_TYPE_LONG;
	param[4].buffer = &prezzo;
	param[4].buffer_length = sizeof(prezzo);
	
	printf("Stock: ");
	fflush(stdout);
	fgets(giacenzaStr, 6, stdin);

	int giacenza = atoi(giacenzaStr);

	param[3].buffer_type = MYSQL_TYPE_LONG;
	param[3].buffer = &giacenza;
	param[3].buffer_length = sizeof(giacenza);

	bind_par(param, array, types, num_params);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for add_plant\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while adding plant.");
	}
	else {
		printf("Plant correctly added...\n");
	}

	mysql_stmt_close(prepared_stmt);


}

static void add_order(MYSQL* conn) {

}

static void add_customer_contact(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[3];
	int num_params = 3;
	char op;
	char options[3] = { '1', '2', '3' };

	char** array;
	char** types;

	char recapito[71];
	char mezzoComunicazione[21];
	char cliente[18];

	memset(param, 0, sizeof(param));

	array = malloc(num_params * sizeof(void*));
	types = malloc(num_params * sizeof(char*));

	if (array == NULL || types == NULL) {
		fprintf(stderr, "Unable to allocate space\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < num_params; i++) {
		types[i] = malloc(512 * sizeof(char));
		array[i] = malloc(512 * sizeof(char));
		if (array[i] == NULL || types[i] == NULL) {
			fprintf(stderr, "Unable to allocate space for array n. %d\n", i);
			exit(EXIT_FAILURE);
		}
	}

	if (!setup_prepared_stmt(&prepared_stmt, "call add_customer_contact(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize customer contact insertion statement\n", false);
	}

	getchar();

	printf("Customer fiscal code: ");
	fflush(stdout);
	fgets(cliente, 18, stdin);

	strcpy(array[2], cliente);
	strcpy(types[2], "string");

	printf("\nChoose the type of contact:\n");
	printf("\t1) Telephone\n");
	printf("\t2) Mobile\n");
	printf("\t3) E-mail\n");
	fflush(stdout);
	op = multiChoice("Select type of contact", options, 3);

	printf("telephone/mobile/e-mail: ");
	fflush(stdout);
	fgets(recapito, 71, stdin);

	strcpy(array[0], recapito);
	strcpy(types[0], "string");

	switch (op) {
	case '1':
		strcpy(mezzoComunicazione, "telefono");
		break;

	case '2':
		strcpy(mezzoComunicazione, "cellulare");
		break;

	case '3':
		strcpy(mezzoComunicazione, "e-mail");
		break;

	default:
		fprintf(stderr, "\nInvalid condition at %s:%d\n", __FILE__, __LINE__);
		abort();
	}

	strcpy(array[1], mezzoComunicazione);
	strcpy(types[1], "string");

	bind_par(param, array, types, num_params);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for add_customer_contact\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while adding customer contact.");
	}
	else {
		printf("\nCustomer contact correctly added...\n");
	}

	mysql_stmt_close(prepared_stmt);

}

static void add_referent_contact(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[3];
	int num_params = 3;
	char op;
	char options[3] = { '1', '2', '3' };

	char** array;
	char** types;

	char recapito[71];
	char mezzoComunicazione[21];
	char referente[18];

	memset(param, 0, sizeof(param));

	array = malloc(num_params * sizeof(void*));
	types = malloc(num_params * sizeof(char*));

	if (array == NULL || types == NULL) {
		fprintf(stderr, "Unable to allocate space\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < num_params; i++) {
		types[i] = malloc(512 * sizeof(char));
		array[i] = malloc(512 * sizeof(char));
		if (array[i] == NULL || types[i] == NULL) {
			fprintf(stderr, "Unable to allocate space for array n. %d\n", i);
			exit(EXIT_FAILURE);
		}
	}

	if (!setup_prepared_stmt(&prepared_stmt, "call add_referent_contact(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize referent contact insertion statement\n", false);
	}

	getchar();

	printf("Referent fiscal code: ");
	fflush(stdout);
	fgets(referente, 18, stdin);

	strcpy(array[2], referente);
	strcpy(types[2], "string");

	printf("\nChoose the type of contact:\n");
	printf("\t1) Telephone\n");
	printf("\t2) Mobile\n");
	printf("\t3) E-mail\n");
	fflush(stdout);
	op = multiChoice("Select type of contact", options, 3);

	printf("telephone/mobile/e-mail: ");
	fflush(stdout);
	fgets(recapito, 71, stdin);

	strcpy(array[0], recapito);
	strcpy(types[0], "string");

	switch (op) {
	case '1':
		strcpy(mezzoComunicazione, "telefono");
		break;

	case '2':
		strcpy(mezzoComunicazione, "cellulare");
		break;

	case '3':
		strcpy(mezzoComunicazione, "e-mail");
		break;

	default:
		fprintf(stderr, "\nInvalid condition at %s:%d\n", __FILE__, __LINE__);
		abort();
	}

	strcpy(array[1], mezzoComunicazione);
	strcpy(types[1], "string");

	bind_par(param, array, types, num_params);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for add_referent_contact\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while adding referent contact.");
	}
	else {
		printf("\nReferent contact correctly added...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void modify_customer_favourite(MYSQL* conn) {

}

static void modify_referent_favourite(MYSQL* conn) {

}

static void get_customer_info(MYSQL* conn) {

}

static void get_pack_info(MYSQL* conn) {

}

static void get_order_info(MYSQL* conn) {

}

static void get_order_packs_info(MYSQL* conn) {

}

static void get_plant_prices(MYSQL* conn) {

}

static void get_plants_stock(MYSQL* conn) {

}

static void get_customer_orders(MYSQL* conn) {

}

static void modify_plant_price(MYSQL* conn) {

}

static void modify_resale_referent(MYSQL* conn) {

}





void run_as_manager(MYSQL* conn) {
	char* path;
	int op;

	printf("Switching to manager role...\n");
	fflush(stdout);

	path = (char*)malloc(256 * sizeof(char));

	if (path == NULL) {
		fprintf(stderr, "Unable to allocate space for path\n");
		exit(EXIT_FAILURE);
	}

	strcpy(path, PATH);
	strcat(path, "users/manager.json");

	if(!parse_config(path, &conf)) {
		fprintf(stderr, "Unable to load manager configuration\n");
		exit(EXIT_FAILURE);
	}

	if (mysql_change_user(conn, conf.db_username, conf.db_password, conf.database)) {
		fprintf(stderr, "mysql_change_user() failed\n");
		exit(EXIT_FAILURE);
	}

	while (true) {

		printf("\n\n*** What should I do for you? ***\n\n");
		printf("1) Add customer\n");
		printf("2) Add referent\n");
		printf("3) Add plant\n");
		printf("4) Add order\n");
		printf("5) Add customer contact\n");
		printf("6) Add referent contact\n");
		printf("7) Modify customer favourite contact\n");
		printf("8) Modify referent favourite contact\n");
		printf("9) Get customer info\n");
		printf("10) Get pack info\n");
		printf("11) Get order info\n");
		printf("12) Get order packs info\n");
		printf("13) Get plant prices\n");
		printf("14) Get plants stock\n");
		printf("15) Get customer orders\n");
		printf("16) Modify plant price\n");
		printf("17) Modify resale referent\n");
		printf("18) Quit\n\n");

		if (scanf("%d", &op) == EOF) {
			fprintf(stderr, "Unable to read from terminal\n");
			exit(EXIT_FAILURE);
		}

		switch (op) {
		case 1:
			add_customer(conn);
			break;

		case 2:
			add_referent(conn);
			break;

		case 3:
			add_plant(conn);
			break;

		case 4:
			printf("op 4\n");
			break;

		case 5:
			add_customer_contact(conn);
			break;

		case 6:
			add_referent_contact(conn);
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
			printf("op 11\n");
			break;

		case 12:
			printf("op 12\n");
			break;

		case 13:
			printf("op 13\n");
			break;

		case 14:
			printf("op 14\n");
			break;

		case 15:
			printf("op 15\n");
			break;

		case 16:
			printf("op 16\n");
			break;

		case 17:
			printf("op 17\n");
			break;

		case 18:
			return;

		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			abort();
		}

	}


}