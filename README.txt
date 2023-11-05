/* IMBREA Giulia-Stefania - 311CB */

[10-21]Definire structuri
TCuvant retine un char* cuvant si int frecventa. Exemplu: (verde, 1)
TCuvinte retine un int dim si o lista generica lista_cuv. Exemplu: (5, [ (verde,1) |   ]-|)

[21-50]Alocari Structuri
TCuvant * Aloca_Init_TCuvant(char *cuvant) Aloca o structura de tip tcuvant si o initializeaza cu frecv = 1 si cuvantul primit ca parametru
TCuvinte * Aloca_Init_TCuvinte(TCuvant * cuv) Aloca o structura de tip TCuvinte

[50-61]Afisare structuri
void AfisTCuvant(void* c) => verde/1//presupunem ca lista are doar o celula
void AfisTCuvinte(void* c) => (5: verde/1)

[62-71]Eliberare structuri
void freeTCuvant(void *elem) elibereaza structura TCuvant(se elibereaza mai intai cuvantul si dupa structura in sine)
void freeTCuvinte(void *elem) elibereaza structura TCuvinte ( mai intai Distruge lista lista_cuv, care contine ca informatie TCuvant(de aceea folosim freeTCuvant); apoi se da free la structura in sine)

[72-90]Functii Utile
int FHash(void * cuvant) returneaza pozitia din alfabet a primei litere din cuvant (exemplu: pt Ana => 0)
int fcmp_dim(void *e1, void *e2) compara dimensiunea dintre 2 cuvinte, scazandu-le 
int fcmp_cuv(void*e1, void*e2) echivalent cu strcmp

[91-177]Inserari
voi inceepe cu Inserare1[146-177] pt a intelege mai bine
int Inserare1(TH*h, void* elem, TFCmp fcmp) 
-reprezinta inserarea in primul "strat", lista din v[cod] dupa dimensiunea cuvantului (aceeasi litera de inceput)
-[149] parcurg lista mentionata si compar dimensiunea cuvantului meu de inserat cu cele existente in lista 
-CAZ 1: exista deja o celula ce contine o structura TCuvinte, avand aceeasi dimensiune ca cuvantul de inserat
    => apelez functia Inserare2: Inserare2(p->info, elem, fcmp_cuv
-CAZ 2: nu exista o celula cu dimensiunea cuvantului nostru => trebuie creata
    -parcurg lista pana cand dau de un cuvant cu lungimea mai mare decat cuvantul meu 
    -am pozitionat ant si p=>putem face inserarea:
        caz1:cuvantul meu are cea mai mica lungime(ant=NULL)
        caz2:ant!=null, exista un cuvant cu lungimea mai mica decat cea a cuvantului meu 
int Inserare2(void *dest, void*elem, TFCmp fcmp)     
-reprezinta inserarea in al doilea "strat", sublista cu cuvinte de aceeasi lungime si initiala
-[96] parcurg lista cu ant1 si p1 si verific daca exista deja cuvantul meu in lista 
-CAZ 1: exista cuvantul in lista => maresc frecventa cuvantului existent
    caz 1: daca cuvantul este primul din lista, inseamna ca ramane cel cu cea mai mare frecventa
    caz 2: daca cuvantul nu este primul din lista, s ar putea ca in urma maririi frecventei, lista sa nu mai fie ordonata descrescator dupa frecv
        ant1->urm = p1->urm; //elimin legaturile lui p1 la locul in care a fost cuvantul a carei frecventa s a marit
        [104-119] reparcurg lista, si verific daca este ordonata dupa frecv, daca nu realizez o inserare in locul potrivit
-CAZ 2: nu exista cuvantul in lista => trebuie inserat 
    -parcurg sublista si pozitionez ant si p1
    -realizez inserarea
    - pentru a rezolva cazurile de memory leak :
    //fiindca el este o structura cu o celula si dim , modificand legaturile catre lista, nu scap de scheletul structurii in sine(dim)
    //asadar ma folosesc de o celula AUX, care retine celula din el
    aux->frecv =  ((TCuvant*)(el->lista_cuv->info))->frecv;//aux contine  informatia din celula el(cuv+frecv)
    TLG AUX = alocaCelulaG((void*)aux); //celula din sublista cu info=aux
    freeTCuvinte((void*)el);//nu mai avem nevoie de structura fiindca avem celula AUX

[177-258]  main 
-argv[0] = ./tema1; argv[1] = fisier.txt
[195]pentru extragerea cuvintelor din fisierul text m am  folosit de fgets impreuna cu strtok
[198-207] insert (este evident ce se intampla acolo, in principiu apelez functia Inserare1)
[211-225] print c 4 (afiseaza cuvintele care incep cu c si au dim=4)
    - transform caracterul '4' in cifra 4, folosind functia atoi
    - transform litera 'c' in 2, cu ajutorul functiei fh 
    -parcurg lista v[2], pana cand dau de o celula cu info->dim = 4
[226] print (afisez tabela hash)(daca cuv==NULL <=> dupa print nu mai am nimic)
[229-252] print 1 (printeaza cuvintele cu frecventa <=1 )
    -parcurg tabela h(v[cod], listele, sublistele) 
    -cand dau de un cuvant cu frecventa <=1 printez pozitia(in fct de ok), printez restul cuvintelor din lista fiindca sunt sortate descrescator

-----SFARSIT---------:-)