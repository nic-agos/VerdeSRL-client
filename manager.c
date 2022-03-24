#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <time.h>

#include "defines.h"

//inserire nella variabile PATH il percorso della cartella del client
#define PATH "C:/Users/nicco/source/repos/VerdeSRL-client/"

static void add_customer(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[10];

	char op;
	char options[2] = { '1', '2'};
	char options_bis[3] = { '1', '2', '3'};

	char id[18];
	char nome[32];
	char cognome[32];
	char via[52];
	char civicoStr[6];
	char citta[27];
	char nazione[22];
	char tipo[31];
	char recapito[72];
	char mezzoComunicazione[22];

	memset(param, 0, sizeof(param));

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

	set_binding_param(&param[7], MYSQL_TYPE_VAR_STRING, tipo, strlen(tipo));

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

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, id, strlen(id));

	printf("Name: ");
	fflush(stdout);
	fgets(nome, 31, stdin);
	nome[strlen(nome) - 1] = '\0';

	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, nome, strlen(nome));

	if(strcmp(tipo, "privato") == 0) {
		printf("Surname: ");
		fflush(stdout);
		fgets(cognome, 31, stdin);
		cognome[strlen(cognome) - 1] = '\0';

		set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, cognome, strlen(cognome));
	}

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

	set_binding_param(&param[8], MYSQL_TYPE_VAR_STRING, recapito, strlen(recapito));

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

	set_binding_param(&param[9], MYSQL_TYPE_VAR_STRING, mezzoComunicazione, strlen(mezzoComunicazione));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for add_customer\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while adding customer.\n");
	}
	else {
		printf("\nCustomer correctly added...\n");
	}

	mysql_stmt_close(prepared_stmt);

}

static void add_referent(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[6];
	char op;
	char options[3] = { '1', '2', '3' };

	char cf[18];
	char nome[32];
	char cognome[32];
	char cliente[18];
	char recapito[72];
	char mezzoComunicazione[22];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call add_referent(?, ?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize referent insertion statement\n", false);
	}

	getchar();
	printf("\nFiscal Code: ");
	fgets(cf, 18, stdin);
	cf[strlen(cf) - 1] = '\0';

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cf, strlen(cf));

	printf("Name: ");
	fflush(stdout);
	fgets(nome, 31, stdin);
	nome[strlen(nome) - 1] = '\0';

	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, nome, strlen(nome));

	printf("Surname: ");
	fflush(stdout);
	fgets(cognome, 31, stdin);
	cognome[strlen(cognome) - 1] = '\0';

	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, cognome, strlen(cognome));

	printf("Customer's fiscal Code: ");
	fflush(stdout);
	fgets(cliente, 18, stdin);
	cliente[strlen(cliente) - 1] = '\0';

	set_binding_param(&param[3], MYSQL_TYPE_VAR_STRING, cliente, strlen(cliente));

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

	set_binding_param(&param[4], MYSQL_TYPE_VAR_STRING, recapito, strlen(recapito));

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

	set_binding_param(&param[5], MYSQL_TYPE_VAR_STRING, mezzoComunicazione, strlen(mezzoComunicazione));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for add_referent\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while adding referent.");
	}
	else {
		printf("\nReferent correctly added...\n");
	}

	mysql_stmt_close(prepared_stmt);

}

static void add_plant(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[8];
	char op;
	char options[7] = { '1', '2', '3', '4', '5', '6', '7'};

	char codice[8];
	char nomeComune[32];
	char nomeLatino[32];
	char giacenzaStr[7];
	char prezzoStr[7];
	char colore[22];
	char tipo[502] = "";

	int chosen[6];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call add_plant(?, ?, ?, ?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize plant insertion statement\n", false);
	}

	getchar();
	printf("Code: ");
	fflush(stdout);
	fgets(codice, 8, stdin);
	codice[strlen(codice) - 1] = '\0';

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, codice, strlen(codice));

	printf("Common name: ");
	fflush(stdout);
	fgets(nomeComune, 31, stdin);
	nomeComune[strlen(nomeComune) - 1] = '\0';

	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, nomeComune, strlen(nomeComune));

	printf("Latin name: ");
	fflush(stdout);
	fgets(nomeLatino, 31, stdin);
	nomeLatino[strlen(nomeLatino) - 1] = '\0';

	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, nomeLatino, strlen(nomeLatino));

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
			fprintf(stderr, "\nInvalid condition at %s:%d\n", __FILE__, __LINE__);
			abort();
		}
		
	} while (op != '7');

	if (chosenNum < 1) {
		fprintf(stderr, "\nYou have to choose at least one type for the plant!\n");
		abort();
	}

	tipo[strlen(tipo) - 1] = '\0';

	printf("Selected types: %s\n", tipo);

	set_binding_param(&param[6], MYSQL_TYPE_VAR_STRING, tipo, strlen(tipo));

	set_binding_param(&param[7], MYSQL_TYPE_LONG, &chosenNum, sizeof(chosenNum));


	for (int i = 0; i < chosenNum; i++) {
		if (chosen[i] == 3) {
			printf("Plant color: ");
			fflush(stdout);
			fgets(colore, 21, stdin);
			colore[strlen(colore) - 1] = '\0';

			set_binding_param(&param[5], MYSQL_TYPE_VAR_STRING, colore, strlen(colore));
		}
	}

	printf("Price: ");
	fflush(stdout);
	fgets(prezzoStr, 6, stdin);

	int prezzo = atoi(prezzoStr);
	set_binding_param(&param[4], MYSQL_TYPE_LONG, &prezzo, sizeof(prezzo));
	
	printf("Stock: ");
	fflush(stdout);
	fgets(giacenzaStr, 6, stdin);

	int giacenza = atoi(giacenzaStr);
	set_binding_param(&param[3], MYSQL_TYPE_LONG, &giacenza, sizeof(giacenza));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for add_plant\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while adding plant.");
	}
	else {
		printf("\nPlant correctly added...\n");
	}

	mysql_stmt_close(prepared_stmt);

}

static void add_order(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[10];

	char codice[42];
	char contatto[17];
	char via[52];
	char civicoStr[6];
	char citta[27];
	char nazione[22];
	char cliente[18];
	char piante[502] = "";
	char quantita[102] = "";
	int num_plants = 0;

	char temp_plant[10];
	char temp_quant[10];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call add_order(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize order insertion statement\n", false);
	}

	time_t t;

	t = time(NULL);

	struct tm s = *localtime(&t);

	int day, month, year;
	int hour, minutes, seconds;

	day = s.tm_mday;
	month = s.tm_mon + 1;
	year = s.tm_year + 1900;

	hour = s.tm_hour;
	minutes = s.tm_min;
	seconds = s.tm_sec;

	getchar();
	printf("Customer fiscal code: ");
	fflush(stdout);
	fgets(cliente, 18, stdin);
	cliente[strlen(cliente) - 1] = '\0';

	set_binding_param(&param[6], MYSQL_TYPE_VAR_STRING, cliente, strlen(cliente));

	sprintf(codice, "%s_%d-%02d-%02d_%02d:%02d:%02d", cliente, year, month, day, hour, minutes, seconds);

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, codice, strlen(codice));

	while (1) {
		printf("Code of the plant to be ordered (quit to stop): ");
		fflush(stdout);
		fgets(temp_plant, 8, stdin);
		temp_plant[strlen(temp_plant) - 1] = '\0';

		if (strcmp(temp_plant, "quit") == 0) {
			break;
		}

		strcat(piante, temp_plant);
		strcat(piante, ",");

		printf("Quantity to order: ");
		fflush(stdout);
		fgets(temp_quant, 8, stdin);
		temp_quant[strlen(temp_quant) - 1] = '\0';
		strcat(quantita, temp_quant);
		strcat(quantita, ",");

		num_plants = num_plants + 1;

	}

	if (strlen(piante) < 1) {
		fprintf(stderr, "\nOrder must contain at least one plant!\n");
		exit(EXIT_FAILURE);
	}
	piante[strlen(piante) - 1] = '\0';
	quantita[strlen(quantita) - 1] = '\0';

	set_binding_param(&param[7], MYSQL_TYPE_VAR_STRING, piante, strlen(piante));

	set_binding_param(&param[8], MYSQL_TYPE_VAR_STRING, quantita, strlen(quantita));

	set_binding_param(&param[9], MYSQL_TYPE_LONG, &num_plants, sizeof(num_plants));

	printf("Order contact: ");
	fflush(stdout);
	fgets(contatto, 17, stdin);
	contatto[strlen(contatto) - 1] = '\0';

	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, contatto, strlen(contatto));

	printf("Street: ");
	fflush(stdout);
	fgets(via, 52, stdin);
	via[strlen(via) - 1] = '\0';

	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, via, strlen(via));

	printf("Street number: ");
	fflush(stdout);
	fgets(civicoStr, 6, stdin);
	civicoStr[strlen(civicoStr) - 1] = '\0';

	int civico = atoi(civicoStr);
	set_binding_param(&param[3], MYSQL_TYPE_LONG, &civico, sizeof(civico));

	printf("City: ");
	fflush(stdout);
	fgets(citta, 27, stdin);
	citta[strlen(citta) - 1] = '\0';

	set_binding_param(&param[4], MYSQL_TYPE_VAR_STRING, citta, strlen(citta));

	printf("Nation: ");
	fflush(stdout);
	fgets(nazione, 22, stdin);
	nazione[strlen(nazione) - 1] = '\0';

	set_binding_param(&param[5], MYSQL_TYPE_VAR_STRING, nazione, strlen(nazione));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for add_order\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while adding order.");
	}
	else {
		printf("\nOrder correctly added...\n");
	}

	mysql_stmt_close(prepared_stmt);

}

static void add_customer_contact(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[3];
	char op;
	char options[3] = { '1', '2', '3'};

	char recapito[71];
	char mezzoComunicazione[21];
	char cliente[18];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call add_customer_contact(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize customer contact insertion statement\n", false);
	}

	getchar();

	printf("Customer fiscal code: ");
	fflush(stdout);
	fgets(cliente, 18, stdin);
	cliente[strlen(cliente) - 1] = '\0';

	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, cliente, strlen(cliente));

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

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, recapito, strlen(recapito));

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

	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, mezzoComunicazione, strlen(mezzoComunicazione));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for add_customer_contact\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while adding customer contact.\n");
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

	char recapito[71];
	char mezzoComunicazione[21];
	char referente[18];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call add_referent_contact(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize referent contact insertion statement\n", false);
	}

	getchar();

	printf("Referent fiscal code: ");
	fflush(stdout);
	fgets(referente, 18, stdin);
	referente[strlen(referente) - 1] = '\0';

	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, referente, strlen(referente));

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

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, recapito, strlen(recapito));

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

	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, mezzoComunicazione, strlen(mezzoComunicazione));

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
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[3];
	char op;
	char options[3] = { '1', '2', '3' };

	char recapito[71];
	char mezzoComunicazione[21];
	char cliente[18];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call modify_customer_favourite(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize customer favourite contact modifying statement\n", false);
	}

	getchar();

	printf("Customer fiscal code: ");
	fflush(stdout);
	fgets(cliente, 18, stdin);
	cliente[strlen(cliente) - 1] = '\0';

	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, cliente, strlen(cliente));

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

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, recapito, strlen(recapito));

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

	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, mezzoComunicazione, strlen(mezzoComunicazione));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for modify_customer_favourite\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while modifying customer favourite contact.");
	}
	else {
		printf("\nCustomer favourite contact correctly modified...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void modify_referent_favourite(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[3];
	char op;
	char options[3] = { '1', '2', '3' };

	char recapito[71];
	char mezzoComunicazione[21];
	char referente[18];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call modify_referent_favourite(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize referent favourite contact modifying statement\n", false);
	}

	getchar();

	printf("Referent fiscal code: ");
	fflush(stdout);
	fgets(referente, 18, stdin);
	referente[strlen(referente) - 1] = '\0';

	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, referente, strlen(referente));

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

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, recapito, strlen(recapito));

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

	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, mezzoComunicazione, strlen(mezzoComunicazione));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for modify_referent_favourite\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while modifying referent favourite contact.");
	}
	else {
		printf("\nReferent favourite contact correctly modified...\n");
	}

	mysql_stmt_close(prepared_stmt);
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
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[2];

	char pianta[8];
	char prezzoStr[8];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call modify_plant_price(?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize modify plant price statement\n", false);
	}

	getchar();
	
	printf("Plant code: ");
	fflush(stdout);
	fgets(pianta, 8, stdin);
	pianta[strlen(pianta) - 1] = '\0';

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, pianta, strlen(pianta));

	printf("New price: ");
	fflush(stdout);
	fgets(prezzoStr, 8, stdin);
	
	int prezzo = atoi(prezzoStr);

	set_binding_param(&param[1], MYSQL_TYPE_LONG, &prezzo, sizeof(prezzo));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for modify_plant_price\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while modifying plant price.");
	}
	else {
		printf("\nPlant price correctly modified...\n");
	}

	mysql_stmt_close(prepared_stmt);

}

static void modify_resale_referent(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[6];
	char op;
	char options[3] = { '1', '2', '3' };

	char cf[18];
	char nome[32];
	char cognome[32];
	char cliente[18];
	char recapito[72];
	char mezzoComunicazione[22];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call modify_resale_referent(?, ?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize modify resale referent statement\n", false);
	}

	getchar();
	printf("\nFiscal Code: ");
	fgets(cf, 18, stdin);
	cf[strlen(cf) - 1] = '\0';

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cf, strlen(cf));

	printf("Name: ");
	fflush(stdout);
	fgets(nome, 31, stdin);
	nome[strlen(nome) - 1] = '\0';

	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, nome, strlen(nome));

	printf("Surname: ");
	fflush(stdout);
	fgets(cognome, 31, stdin);
	cognome[strlen(cognome) - 1] = '\0';

	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, cognome, strlen(cognome));

	printf("Customer's fiscal Code: ");
	fflush(stdout);
	fgets(cliente, 18, stdin);
	cliente[strlen(cliente) - 1] = '\0';

	set_binding_param(&param[3], MYSQL_TYPE_VAR_STRING, cliente, strlen(cliente));

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

	set_binding_param(&param[4], MYSQL_TYPE_VAR_STRING, recapito, strlen(recapito));

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

	set_binding_param(&param[5], MYSQL_TYPE_VAR_STRING, mezzoComunicazione, strlen(mezzoComunicazione));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for modify_resale_referent\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while modifying resale referent.");
	}
	else {
		printf("\nResale referent correctly modified...\n");
	}

	mysql_stmt_close(prepared_stmt);


}

static void remove_customer(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[1];

	char cliente[18];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call remove_customer(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize remove customer statement\n", false);
	}

	getchar();

	printf("Customer fiscal code: ");
	fflush(stdout);
	fgets(cliente, 18, stdin);
	cliente[strlen(cliente) - 1] = '\0';

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cliente, strlen(cliente));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for remove_customer\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while removing customer.");
	}
	else {
		printf("\nCustomer correctly removed...\n");
	}

	mysql_stmt_close(prepared_stmt);

}

static void remove_order(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[1];

	char ordine[72];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call remove_order(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize remove order statement\n", false);
	}

	getchar();

	printf("Order code: ");
	fflush(stdout);
	fgets(ordine, 72, stdin);
	ordine[strlen(ordine) - 1] = '\0';

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, ordine, strlen(ordine));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for remove_order\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while removing order.");
	}
	else {
		printf("\nOrder correctly removed...\n");
	}

	mysql_stmt_close(prepared_stmt);
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

		printf("\n*** What should I do for you? ***\n");
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
		printf("18) Remove customer\n");
		printf("19) Remove order\n");
		printf("20) Quit\n\n");

		if (scanf("%d", &op) == EOF) {
			fprintf(stderr, "Unable to read from terminal\n");
			exit(EXIT_FAILURE);
		}

		switch (op) {
		case 1:
			printf("\033[2J\033[H");
			add_customer(conn);
			break;

		case 2:
			printf("\033[2J\033[H");
			add_referent(conn);
			break;

		case 3:
			printf("\033[2J\033[H");
			add_plant(conn);
			break;

		case 4:
			printf("\033[2J\033[H");
			add_order(conn);
			break;

		case 5:
			printf("\033[2J\033[H");
			add_customer_contact(conn);
			break;

		case 6:
			printf("\033[2J\033[H");
			add_referent_contact(conn);
			break;

		case 7:
			printf("\033[2J\033[H");
			modify_customer_favourite(conn);
			break;

		case 8:
			printf("\033[2J\033[H");
			modify_referent_favourite(conn);
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
			printf("\033[2J\033[H");
			printf("op 12\n");
			break;

		case 13:
			printf("\033[2J\033[H");
			printf("op 13\n");
			break;

		case 14:
			printf("\033[2J\033[H");
			printf("op 14\n");
			break;

		case 15:
			printf("\033[2J\033[H");
			printf("op 15\n");
			break;

		case 16:
			printf("\033[2J\033[H");
			modify_plant_price(conn);
			break;

		case 17:
			printf("\033[2J\033[H");
			modify_resale_referent(conn);
			break;

		case 18:
			printf("\033[2J\033[H");
			remove_customer(conn);
			break;

		case 19:
			printf("\033[2J\033[H");
			remove_order(conn);
			break;

		case 20:
			return;

		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			abort();
		}

	}


}