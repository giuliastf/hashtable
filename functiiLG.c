/* IMBREA Giulia-Stefania - 311CB */
/*--- functiiLG.c -- operatii de baza pentru lista simplu inlantuita generica---*/
#include "tlg.h"

int Ins_IncLG(TLG * aL, void * ae) {
    TLG aux = malloc(sizeof(TCelulaG));
    if (!aux)
        return 0;

    aux -> info = ae;
    aux -> urm = * aL;
    * aL = aux;

    return 1;
}

void DistrugeLG(TLG * aL, TF free_elem) /* distruge lista */ {
    while ( * aL != NULL) {
        TLG aux = * aL; /* adresa celulei eliminate */
        if (!aux)
            return;

        free_elem(aux -> info); /* elib.spatiul ocupat de element*/
        * aL = aux -> urm; /* deconecteaza celula din lista */
        free(aux); /* elibereaza spatiul ocupat de celula */
    }
}

size_t LungimeLG(TLG * a) /* numarul de elemente din lista */ {
    size_t lg = 0;
    TLG p = * a;

    /* parcurge lista, numarand celulele */
    for (; p != NULL; p = p -> urm)
        lg++;

    return lg;
}
/*
void Afisare(TLG* aL, TF afiEL)
{
	if(!*aL) {
        printf("Lista vida\n");
        return;
    }
	printf("[\n");
	for(; *aL; aL = &(*aL)->urm) {
		afiEL( (*aL)->info );
    }
	printf("\n]\n");
}

*/
void Afisare(TLG L, TF afiEl) {
    if (!L)
        printf("Lista vida\n");
    for (; L; L = L -> urm) {
        afiEl(L -> info);
        //L=NULL;
        if (L -> urm)
            printf(", ");
    }
}
TLG alocaCelulaG(void * x) {
    TLG aux;
    aux = (TLG) malloc(sizeof(TCelulaG));
    if (!aux)
        return NULL;
    aux -> info = x;
    aux -> urm = NULL;
    return aux;
}