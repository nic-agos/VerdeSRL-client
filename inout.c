#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <stdbool.h>
#include <conio.h>

#include "defines.h"

bool yesOrNo(char* domanda, char yes, char no, bool predef, bool insensitive)
{

	// I caratteri 'yes' e 'no' devono essere minuscoli
	yes = tolower(yes);
	no = tolower(no);

	// Decide quale delle due lettere mostrare come predefinite
	char s, n;
	if (predef) {
		s = toupper(yes);
		n = no;
	}
	else {
		s = yes;
		n = toupper(no);
	}

	// Richiesta della risposta
	while (true) {
		// Mostra la domanda
		printf("%s [%c/%c]: ", domanda, s, n);

		char c;
		fgets(&c, 2, stdin);

		// Controlla quale risposta è stata data
		if (c == '\0') { // getInput() non può restituire '\n'!
			return predef;
		}
		else if (c == yes) {
			return true;
		}
		else if (c == no) {
			return false;
		}
		else if (c == toupper(yes)) {
			if (predef || insensitive) return true;
		}
		else if (c == toupper(yes)) {
			if (!predef || insensitive) return false;
		}
	}
}

char multiChoice(char* domanda, char choices[], int num)
{
	char inputStr[3];

	// Genera la stringa delle possibilità
	char* possib = (char*)malloc(2 * num * sizeof(char));
	int i, j = 0;
	for (i = 0; i < num; i++) {
		possib[j++] = choices[i];
		possib[j++] = '/';
	}
	possib[j - 1] = '\0'; // Per eliminare l'ultima '/'

	// Chiede la risposta
	while (true) {
		// Mostra la domanda
		printf("%s [%s]: ", domanda, possib);

		fgets(inputStr, 3, stdin);
		char c = inputStr[0];

		// Controlla se è un carattere valido
		for (i = 0; i < num; i++) {
			if (c == choices[i])
				return c;
		}
	}
}

void insertPassword(char password[]) {
	int ch;
	int i = 0;
	while ((ch = _getch()) != EOF && ch != '\n' && ch != '\r' && i < sizeof(password) - 1) {
		if (ch == '\b' && i > 0) {
			printf("\b \b");		// Destructive backspace
			fflush(stdout);
			i--;
			password[i] = '\0';
		}
		else if (ch != '\b') {
			putchar('*');
			password[i++] = (char)ch;
		}
	}
	password[i] = '\0';

	printf("\n");
	fflush(stdout);
}