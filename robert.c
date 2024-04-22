#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <math.h>

#pragma warning (disable:4996)

typedef struct Comanda Comanda;
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;
typedef struct ListaSimplaNod;

struct Comanda {
    unsigned int id;
    char* data_lansare_c;
    char* data_livrare_c;
    char* nume_client;
    int status_comanda;
};
struct Nod {
    Comanda info;
    Nod* next;
    Nod* prev;
};
struct ListaDubla {
    Nod* primulNod;
    Nod* ultimulNod;
};
struct ListaSimplaNod {
    Comanda info;
    Nod* next;
};

void afisareComanda(Comanda c) {
    printf("Id: %u, Data_Lansare: %s, Data_Livrare: %s, Nume client: %s, Status: %d \n",
        c.id, c.data_lansare_c, c.data_livrare_c, c.nume_client, c.status_comanda);
}

ListaDubla inserareInceput(ListaDubla lista, Comanda c) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = c;
    nou->next = lista.primulNod;
    nou->prev = NULL;
    if (lista.primulNod != NULL) {
        lista.primulNod->prev = nou;
    }
    else {
        lista.ultimulNod = nou;
    }
    lista.primulNod = nou;
    return lista;
}

void afisareListaDubla(ListaDubla lista) {
    while (lista.primulNod != NULL) {
        afisareComanda(lista.primulNod->info);
        lista.primulNod = lista.primulNod->next;
    }
}
void afisareListaDublaInvers(ListaDubla lista) {
    while (lista.ultimulNod != NULL) {
        afisareComanda(lista.ultimulNod->info);
        lista.ultimulNod = lista.ultimulNod->prev;
    }
}

int nrComenziClient(ListaDubla lista, char* numeCautat) {
    int nr_comenzi = 0;
    Nod* current = lista.primulNod;
    while (current != NULL) {
        if (strcmp(current->info.nume_client, numeCautat) == 0) {
            nr_comenzi += 1;
        }
        current = current->next;
    }
    return nr_comenzi;
}

void afisareListaSimpla(Nod* cap) {
    while (cap != NULL) {
        afisareComanda(cap->info);
        cap = cap->next;
    }
}

void modificaDataLivrare(ListaDubla lista, const char* numeClient_cautat, int status_cautat, const char* livrareNoua) {
    Nod* temp = lista.primulNod;
    while (temp != NULL) {
        if (strcmp(temp->info.nume_client, numeClient_cautat) == 0 && temp->info.status_comanda == status_cautat) {
            free(temp->info.data_livrare_c);
            temp->info.data_livrare_c = (char*)malloc(sizeof(char) * (strlen(livrareNoua) + 1));
            strcpy(temp->info.data_livrare_c, livrareNoua);
        }
        temp = temp->next;
    }
}

void inserareInceputLista(Comanda c, Nod** cap) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = c;
    nou->next = NULL;

    if (*cap == NULL) {
        *cap = nou;
    }
    else {
        Nod* curent = *cap;
        while (curent->next != NULL) {
            curent = curent->next;
        }
        curent->next = nou;
    }
}

Nod* conversieListaDublaInSimpla(ListaDubla lista) {
    Nod* cap = NULL;
    Nod* current = lista.primulNod;
    while (current != NULL) {
        inserareInceputLista(current->info, &cap);
        current = current->next;
    }
    return cap;
}

int main() {
    int nrComenzi;
    char buffer[100];

    Comanda c;
    ListaDubla lista;
    lista.primulNod = NULL;
    lista.ultimulNod = NULL;

    FILE* f = fopen("comenzi.txt", "r");
    fscanf(f, "%d", &nrComenzi);
    for (int i = 0; i < nrComenzi; i++) {
        fscanf(f, "%u", &c.id);

        fscanf(f, "%s", buffer);
        c.data_lansare_c = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
        strcpy(c.data_lansare_c, buffer);

        fscanf(f, "%s", buffer);
        c.data_livrare_c = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
        strcpy(c.data_livrare_c, buffer);

        fscanf(f, "%s", buffer);
        c.nume_client = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
        strcpy(c.nume_client, buffer);

        fscanf(f, "%d", &c.status_comanda);

        lista = inserareInceput(lista, c);
    }
    fclose(f);

    printf("Lista dubla:\n");
    afisareListaDubla(lista);

    printf("\nExercitiul 2\n");
    int proiecteAndrei = nrComenziClient(lista, "Andrei");
    printf("Andrei are %d comenzi\n", proiecteAndrei);

    printf("\nExercitiul 3\n");
    modificaDataLivrare(lista, "Andrei", 10, "2002-09-28");
    printf("Lista dubla invers:\n");
    afisareListaDublaInvers(lista);

    printf("\nExercitiul 4\n");
    Nod* listaSimpla = conversieListaDublaInSimpla(lista);
    printf("Lista simpla:\n");
    afisareListaSimpla(listaSimpla);

    while (listaSimpla != NULL) {
        Nod* temp = listaSimpla;
        listaSimpla = listaSimpla->next;
        free(temp->info.data_lansare_c);
        free(temp->info.data_livrare_c);
        free(temp->info.nume_client);
        free(temp);
    }

    return 0;
}
