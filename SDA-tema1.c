/* IMBREA Giulia-Stefania - 311CB */ 
#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "tlg.h"

#include "thash.h"

#define M 26
#define dimCuv 30
#define dimTEXT 100
/*--------------------------------STRUCTURI--------------------------------------------*/
typedef struct {
    char * cuvant;
    int frecv;
}
TCuvant; //exemplu: (verde, 1)

typedef struct {
    int dim; //lungimea cuvantului
    TLG lista_cuv; //lista cu cuvinte de lungimea dim
}
TCuvinte;

/*---------------------------ALOCARI-STRUCTURI-----------------------------------------*/
TCuvant * Aloca_Init_TCuvant(char * cuvant) {
    TCuvant * cuv = (TCuvant * ) malloc(1 * sizeof(TCuvant));
    if (!cuvant) {
        free(cuvant);
        return NULL;
    }
    cuv -> cuvant = (char * ) malloc(dimCuv * sizeof(char));
    if (!cuv -> cuvant) {
        free(cuv -> cuvant);
        free(cuvant);
        return NULL;
    }
    strcpy(cuv -> cuvant, cuvant);
    cuv -> frecv = 1;
    return cuv;
}

TCuvinte * Aloca_Init_TCuvinte(TCuvant * cuv) {
    TCuvinte * cuvinte = (TCuvinte * ) malloc(1 * sizeof(TCuvinte));
    if (!cuvinte) {
        free(cuvinte);
        return NULL;
    }
    cuvinte -> lista_cuv = alocaCelulaG((void * ) cuv); //CUVANT TRB SA FIE VOID*
    cuvinte -> dim = strlen(cuv -> cuvant);
    return cuvinte;
}

/*---------------------------AFISARE STRUCTURI-----------------------------------------*/
void AfisTCuvant(void * c) {
    TCuvant * cuv = (TCuvant * ) c;
    printf("%s/%d", cuv -> cuvant, cuv -> frecv);
}

void AfisTCuvinte(void * c) { //afisare primul strat adica informatia dintr o celula a lui v
    TCuvinte * cuvinte = (TCuvinte * ) c;
    printf("(%d:", cuvinte -> dim);
    Afisare(cuvinte -> lista_cuv, AfisTCuvant);
    printf(")");
}
/*---------------------------ELIBERARE STRUCTURI------------------------------------------*/
void freeTCuvant(void * elem) {
    free(((TCuvant * ) elem) -> cuvant);
    free(((TCuvant * ) elem));
}

void freeTCuvinte(void * elem) {
    DistrugeLG( & ((TCuvinte * ) elem) -> lista_cuv, freeTCuvant);
    free((TCuvinte * ) elem);
}
/*--------------------------------FUNCTII UTILE-----------------------------------------*/
int FHash(void * cuvant) {
    char * c = (char * ) cuvant;
    if ((( * c) >= 'a') && (( * c) <= 'z'))
        return ( * c) - 'a'; //*c = prima litera din cuvant => functia returneaza pozitia din alfabet a primei litere
    return ( * c) - 'A';
}

int fcmp_dim(void * e1, void * e2) {
    TCuvinte * E1 = (TCuvinte * ) e1;
    TCuvinte * E2 = (TCuvinte * ) e2;
    return E1 -> dim - E2 -> dim;
}

int fcmp_cuv(void * e1, void * e2) {
    TCuvant * E1 = (TCuvant * ) e1;
    TCuvant * E2 = (TCuvant * ) e2;
    return strcmp(E1 -> cuvant, E2 -> cuvant);
}
/*--------------------------------INSERARI-----------------------------------------*/
int Inserare2(void * dest, void * elem, TFCmp fcmp) { //Inserare In sublista (lista_cuv) toate cuvintele cu dim egale
    TCuvinte * d = (TCuvinte * ) dest;
    TCuvinte * el = ((TCuvinte * ) elem);
    TLG ant1, p1, ant, p;
    for (ant1 = NULL, p1 = d -> lista_cuv; p1 != NULL; ant1 = p1, p1 = p1 -> urm)
        if (fcmp(p1 -> info, el -> lista_cuv -> info) == 0) { //fcmp_cuv = verific daca este acelasi cuvant(CAZ1)
            ((TCuvant * )(p1 -> info)) -> frecv++; //maresc frecventa
            if (ant1 == NULL) { //daca cuvantul ales este deja primul din lista=> lista va ramane sortata
                freeTCuvinte((void * ) el);
                return 1; //altfel trebuie rearanjata lista in fucntie de frecventa
            }
            ant1 -> urm = p1 -> urm; //elimin locul in care a fost cuvantul a carei frecventa s a marit
            for (ant = NULL, p = d -> lista_cuv; p != NULL; ant = p, p = p -> urm) { //introduc cuvantul in dupa frecv
                if (((TCuvant * )(p1 -> info)) -> frecv >= ((TCuvant * )(p -> info)) -> frecv) {
                    if (ant != NULL) {
                        ant -> urm = p1;
                        p1 -> urm = p;
                    } else {
                        p1 -> urm = p; //daca devine cuvantul cu cea mai mare frecventa,
                        (d -> lista_cuv) = p1; //trb modificata adresa de inceput
                    }
                    break; //nu mai are sens sa reintram in for :))
                }
            }
            freeTCuvinte((void * ) el);
            return 1;
        }
    // (CAZ2)nu exista cuvantul in lista de  cuvinte=> trb inserarat
    for (ant = NULL, p1 = d -> lista_cuv; p1 != NULL; ant = p1, p1 = p1 -> urm) {
        if ((fcmp(p1 -> info, el -> lista_cuv -> info) > 0))
            if (((TCuvant * )(p1 -> info)) -> frecv == 1)
                break; //am pozitionat p1 si ant                                
    }
    TCuvant * aux = Aloca_Init_TCuvant(((TCuvant * )(el -> lista_cuv -> info)) -> cuvant);
    /* fiindca el este o structura cu o celula si dim , modificand legaturile catre lista
    nu scap de scheletul structurii in sine(dim)
    asadar ma folosesc de o celula AUX, care retine celula din el */
    aux -> frecv = ((TCuvant * )(el -> lista_cuv -> info)) -> frecv; //aux contine informatia din celula el(cuv+frecv)
    TLG AUX = alocaCelulaG((void * ) aux); //celula din sublista cu info=aux
    freeTCuvinte((void * ) el); //nu mai avem nevoie de structura fiindca avem celula AUX
    if (ant != NULL) {
        AUX -> urm = p1;
        ant -> urm = AUX;
    } else {
        if (Ins_IncLG( & (d -> lista_cuv), AUX -> info) == 1) {
            free(AUX); //elimin doar "scheletul celulei" , nu si informatia
            return 1; //reusit
        }
        DistrugeLG( & AUX, freeTCuvant); //daca nu merge inserarea la inceput, nu mai avem nevoie de aux
        return 0;
    }
    return 1; //a mers inserarea
}

int Inserare1(TH * h, void * elem, TFCmp fcmp) { //Inserare dupa dimensiunea cuvantului
    int cod = h -> fh((void * )(((TCuvant * )(((TCuvinte * ) elem) -> lista_cuv -> info)) -> cuvant));
    TLG p, ant;
    for (p = h -> v[cod]; p != NULL; p = p -> urm) {
    /* CAZ_1: am gasit in v[cod] o celula ce contine o structura de tip TCuvinte, 
    cu dimensiunea egala cu cuvantul de inserat(din elem) */
        if (fcmp(p -> info, elem) == 0) { //fcmp_dim = compar dimensiunea cuvantului din p->info si din elem 
            if (Inserare2(p -> info, elem, fcmp_cuv) == 1) { //Trebuie sa inseram cuvantul in sublista(lista_cuv)
                return 1; //reusit
            } else {
                freeTCuvinte(elem);
                return 0;
            }
        }
    }
    //CAZ 2:nu exista o celula cu dimensiunea cuvantului nostru => trebuie creata
    for (ant = NULL, p = h -> v[cod]; p != NULL && fcmp(p -> info, elem) < 0; ant = p, p = p -> urm);
    if (ant != NULL) { //lungimea cuvantului de inserat este mai mica decat cele existente
        TLG aux = alocaCelulaG(elem); //elem a fost de tip TCuvinte
        aux -> urm = p;
        ant -> urm = aux;
    } else {
        if (Ins_IncLG( & (h -> v[cod]), elem) == 1)
            return 1; //reusit
        else {
            freeTCuvinte(elem);
            return 0;
        }
    }
    return 1; //reusit
}
/*-------------------------------------------------------------------------------------------*/
int main(int argc, char ** argv) {
    FILE * f = fopen(argv[1], "r"); //argv[0] = ./tema1; argv[1] = fisier.txt
    if (f == NULL)
        exit(EXIT_FAILURE);
    TH * h = NULL;
    h = InitTH(M, FHash); //initializare tabela hash
    char lit;
    int len, cod, i, ok = 1;
    TLG p, x;
    TCuvinte * y;
    char * text = (char * ) malloc(dimTEXT * sizeof(char));
    if (text == NULL) {
        free(text);
        return -1;
    }
    char * cuv;
    char delimit[] = " ,.;!?\t\r\n";
    while (fgets(text, dimTEXT, f)) { //citire linie cu linie din fisier
        cuv = strtok(text, delimit); //iau cuv pana dau de un delimitator(' ', . etc)

        if (strcmp(cuv, "insert") == 0) { // INSERT : insert cutu
            while (cuv != NULL) {
                cuv = strtok(NULL, delimit);
                if (cuv && strlen(cuv) > 2 && ( * cuv >= 'A' && * cuv <= 'z')) {
                    TCuvant * cuvant = Aloca_Init_TCuvant(cuv); // am creat structura (cutu,1)
                    TCuvinte * cuvinte = Aloca_Init_TCuvinte(cuvant); // am creat struct(4, TLG cu structura de sus)
                    Inserare1(h, (void * ) cuvinte, fcmp_dim); //int Inserare1(TH*h, void* src, TFCmp fcmp)
                }
            }
        }

        if (cuv && strcmp(cuv, "print") == 0) {
            cuv = strtok(NULL, delimit);
            if (cuv && (( * cuv >= 'a' && * cuv <= 'z') || ( * cuv >= 'A' && * cuv <= 'Z')))
            { // print c 4 (afiseaza cuvintele care incep cu c si au dim=4)
                lit = * cuv; //lit=c
                cuv = strtok(NULL, delimit);
                if (cuv && (( * cuv > '0') && ( * cuv <= '9'))) {
                    len = atoi(cuv); //transform caracterul '4' in cifra 4
                    cod = h -> fh((void * )( & lit));
                    for (p = h -> v[cod]; p; p = p -> urm)
                        if (((TCuvinte * )(p -> info)) -> dim == len) {
                            AfisTCuvinte((void * )(p -> info));
                            printf("\n");
                            break; //nu mai are sens sa intru din nou in for
                        }
                }
            } else if (!cuv) // print (printeaza tabela h)
                AfiTH(h, AfisTCuvinte);

            else { //print 1 (printeaza cuvintele cu frecventa <=1 )
                for (i = 0; i < M; i++) {
                    p = h -> v[i];
                    for (; p; p = p -> urm) { //parcurg lista v[0]
                        y = (TCuvinte * )(p -> info); //retine info din celula(dupa dim)
                        x = y -> lista_cuv; //parcurg lista_cuv cu (aceeasi dim si lit)
                        for (; x; x = x -> urm)
                            if (((TCuvant * )(x -> info)) -> frecv <= (atoi(cuv))) {
                                if (ok == 1) {
                                    printf("pos%d: ", i);
                                    ok = 0;
                                }
                                printf("(%d: ", y -> dim); //vr sa printez de la x incolo
                                Afisare(x, AfisTCuvant); //pt ca elementele sunt deja sortate dupa frecv
                                printf(")");
                                break;
                            }
                    }
                    if (ok == 0)
                        printf("\n"); //trec la alta initiala
                    ok = 1;
                }
            }
        }
    }
    free(text);
    DistrTH( & h, freeTCuvinte);
    fclose(f);
    return 0;
}