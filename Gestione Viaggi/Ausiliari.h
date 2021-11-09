//Qui sono conservate fuznioni e definizioni generali

#ifndef AUX_H
#define AUX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <math.h>

#define ADMINPASSWORD "admin"

typedef char String[1024];

typedef enum {false = 0, true = 1} bool;
typedef enum {COSTO, DURATA, FAST} variante;  //Differenzia cosa la funzione deve calcolare

int max (int a, int b) {
    return (a >= b ? a : b);
}

bool isLetter (char c) {
    return ( (c >= 97 && c <= 122) ? true : false );
}
bool isLiteral (String stringa) {

    //Controlla che una stringa sia formata solo da lettere, apostrofi e spazi

    int i;
    for (i=0; i<strlen(stringa); i++) {
        if (stringa[i] != 32 && stringa[i] != 39 && !((stringa[i] >= 65 && stringa[i] <= 90) || (stringa[i] >= 97 && stringa[i] <= 122)))
            return false;
    }
    return true;

}
int strcmp_literal (String stringa_1, String stringa_2) {

    //Questa funzione confronta due stringhe esattamente come strcmp(), ma ignora qualunque carattere non sia una lettera

    int l_1 = strlen(stringa_1);
    int l_2 = strlen(stringa_2);
    int max_l = max(l_1, l_2);

    int i, j;
    for (i=0, j=0; i<max_l, j<max_l; i++, j++) {

        while(!isLetter(tolower(stringa_1[i])) && stringa_1[i] != '\0') {i++;}
        while(!isLetter(tolower(stringa_2[j])) && stringa_2[j] != '\0') {j++;}

        if (tolower(stringa_1[i]) < tolower(stringa_2[j])) {return -1;}
        if (tolower(stringa_2[j]) < tolower(stringa_1[i])) {return 1;}

    }
    return 0;

}

void waitKey () {
    getch();
    fflush(stdin);
}

#endif
