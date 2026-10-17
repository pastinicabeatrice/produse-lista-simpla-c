#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int cod;
	char* denumire;
	float pret;
	float cantitate;
} Produs;

typedef struct Nod {
	Produs inf;
	struct Nod* next;
} Nod;

Nod* inserareFinal(Nod* cap, Produs p) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->inf = p;
	nou->inf.denumire = (char*)malloc(strlen(p.denumire) + 1);
	strcpy(nou->inf.denumire, p.denumire);
	nou->next = NULL;

	if (cap == NULL) {
		return nou;
	}
	Nod* t = cap;
	while (t->next != NULL) {
		t = t->next;
	}
	t->next = nou;
	return cap;
}

void afisareProdus(Produs p) {
	printf("Cod: %d | %-10s | Pret: %7.2f | Cantitate: %6.2f\n", p.cod, p.denumire, p.pret, p.cantitate);
}

void traversare(Nod* cap) {
	for (Nod* t = cap; t != NULL; t = t->next) {
		afisareProdus(t->inf);
	}
}

Nod* citireFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (f == NULL) {
		printf("Fisierul %s nu a putut fi deschis.\n", numeFisier);
		return NULL;
	}
	Nod* cap = NULL;
	Produs p;
	char buffer[50];
	while (fscanf(f, "%d %49s %f %f", &p.cod, buffer, &p.pret, &p.cantitate) == 4) {
		p.denumire = buffer;
		cap = inserareFinal(cap, p);
	}
	fclose(f);
	return cap;
}

float valoareStoc(Nod* cap) {
	float total = 0;
	for (Nod* t = cap; t != NULL; t = t->next) {
		total += t->inf.pret * t->inf.cantitate;
	}
	return total;
}

void afisareSubPret(Nod* cap, float pretMaxim) {
	for (Nod* t = cap; t != NULL; t = t->next) {
		if (t->inf.pret < pretMaxim) {
			afisareProdus(t->inf);
		}
	}
}

int modificarePret(Nod* cap, int cod, float pretNou) {
	for (Nod* t = cap; t != NULL; t = t->next) {
		if (t->inf.cod == cod) {
			t->inf.pret = pretNou;
			return 1;
		}
	}
	return 0;
}

Nod* stergereDupaCod(Nod* cap, int cod) {
	Nod* anterior = NULL;
	Nod* t = cap;
	while (t != NULL && t->inf.cod != cod) {
		anterior = t;
		t = t->next;
	}
	if (t == NULL) {
		return cap;
	}
	if (anterior == NULL) {
		cap = t->next;
	}
	else {
		anterior->next = t->next;
	}
	free(t->inf.denumire);
	free(t);
	return cap;
}

void dezalocare(Nod** cap) {
	while (*cap != NULL) {
		Nod* urm = (*cap)->next;
		free((*cap)->inf.denumire);
		free(*cap);
		*cap = urm;
	}
}

int main() {
	Nod* cap = citireFisier("fisier.txt");

	printf("Produse in stoc:\n");
	traversare(cap);
	printf("Valoare stoc: %.2f\n", valoareStoc(cap));

	printf("\nProduse cu pret sub 10:\n");
	afisareSubPret(cap, 10.0f);

	if (modificarePret(cap, 36, 89.90f)) {
		printf("\nPretul produsului 36 a fost actualizat.\n");
	}

	cap = stergereDupaCod(cap, 13);
	printf("\nDupa stergerea produsului 13:\n");
	traversare(cap);
	printf("Valoare stoc: %.2f\n", valoareStoc(cap));

	dezalocare(&cap);
	return 0;
}
