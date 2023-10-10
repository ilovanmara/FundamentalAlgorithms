/*
Nume: Ilovan Mara
Grupa: 2
Tema nr 5: cautarea in tablela de dispersie
La aceasta tema am implementat tablelele de dispersie folosind open addressing si verificarea patratica.
Factorul de umplere reprezinta numarul de elemente din vector raportat la dimensiunea tabelei.
Evaluarea a fost facuta pe factori de umplere intre 0.80 si 0.99 si se poate observa cresterea efortului o data cu
cresterea numarului de elemente din tabela.
*/


#include <stdio.h>
#include "Profiler.h"
#include <string.h>
#pragma warning(disable : 4996)

Profiler p("hashTabeles");

typedef struct {
	int id;
	char name[30];
	int status;
}Entry;

//daca status = 0 inseamna ca tabela e ocupata
//daca status = -1 inseamna ca tabela e libera
//daca status = -2 inseamna ca de pe acea pozitie a fost sters un element

Entry* initTable(int m) {
	Entry* table = (Entry*)malloc(m * sizeof(Entry));
	for (int i = 0; i < m; i++) {
		table[i].status = -1;
		table[i].id = 0;
		strcpy(table[i].name, "NULL");
	}
	return table;
}

int hashFuncQuadraticProbing(int k, int m, int i) {
	return ((k % m) + 7 * i + 3 * i * i) % m;
}

void insertInTable(Entry* table, Entry key, int m) {
	int i = 0;
	int j;
	while (i < m) {
		j = hashFuncQuadraticProbing(key.id, m, i);
		if (table[j].status == -1 || table[j].status == -2) {
			table[j].id = key.id;
			strcpy(table[j].name, key.name);
			table[j].status = 0;
			//return j;
			break;
		}
		else i = i + 1;
	}
	//table[i].status = 0;
	//return -1;
}

void printTable(Entry* table, int m) {
	for (int i = 0; i < m; i++) {
		printf("%d: %d %s \n", i, table[i].id, table[i].name);
	}
}

int searchInTable(Entry* table, int key, int m, int* op) {
	int i = 0;
	int j = hashFuncQuadraticProbing(key, m, i);
	(* op)++;
	while (table[j].status != -1  && i < m) {
		if (table[j].id == key)
			return j;
		(*op)++;
		i = i + 1;
		j = hashFuncQuadraticProbing(key, m, i);
	}
	return -1;
}

int searchInTable_name(Entry* table, Entry key, int m, int* op) {
	int i = 0;
	int j = hashFuncQuadraticProbing(key.id, m, i);

	while (table[j].status == 0 && i < m) {
		(* op)++;
		if (strcmp(table[j].name, key.name) == 0)
			return j;
		i = i + 1;
		j = hashFuncQuadraticProbing(key.id, m, i);
	}
	return -1;
}

void deleteFromTable(Entry* table, Entry key, int m, int* op) {
	int i = searchInTable(table, key.id, m, op);
	if (i != -1) {
		table[i].status = -2;
		table[i].id = 0;
		strcpy(table[i].name, "NULL");
	}
}

void deleteFromTable_name(Entry* table, Entry key, int m, int* op) {
	int i = searchInTable_name(table, key, m, op);
	if (i != -1) {
		table[i].status = -2;
		table[i].id = 0;
		strcpy(table[i].name, "NULL");
	}
}

//name e pt demonstrarea corectitudinii operatiunilor de stergere si cautare

void demo_95() {
	int m = 19;
	int op;
	int load_factor = 95;
	int nr_el = (double(load_factor) / 100) * m;
	printf("%d\n", nr_el);
	Entry* table = initTable(m);
	int* aux = (int*)malloc(nr_el * sizeof(int));
	Entry* v = initTable(nr_el);

	FillRandomArray(aux, nr_el, 1, 3000, true, 0);

	for (int i = 0; i < nr_el; i++) {
		v[i].id = aux[i];
		insertInTable(table, v[i], m);
	}
	printTable(table, m);
	Entry el;
	el.id = 12;
	strcpy(el.name, "\n");
	int pos = searchInTable(table, 1000101, m, &op);
	printf("Am gasit elementul pe pozitia: %d", pos);
}

void demo() {
	int m = 7;
	int op;
	Entry* table = initTable(m);
	Entry el1;
	el1.id = 9;
	strcpy(el1.name, "raluca");
	insertInTable(table, el1, m);
	el1.id = 2;
	strcpy(el1.name, "denisa");
	insertInTable(table, el1, m);
	el1.id = 6;
	strcpy(el1.name, "cosmin");
	insertInTable(table, el1, m);
	el1.id = 12;
	strcpy(el1.name, "dorin");
	insertInTable(table, el1, m);

	printTable(table, m);

	Entry key;
	key.id = 2;
	strcpy(key.name, "denisa");
	int r = searchInTable_name(table, key, m, &op);
	printf("Am gasit elementul pe pozitia: %d\n", r);

	deleteFromTable_name(table, key, m, &op);
	printTable(table, m);
}

void printArray(int* v, int n) {
	for (int i = 0; i < n; i++)
		printf("%d ", v[i]);
	printf("\n");
}

void average_case(int load_factor) {
	int N = 10007;
	int op = 0;
	Entry* table = initTable(N);
	int n = (double(load_factor) / 100) * N;
	int* aux = (int*)malloc(N * sizeof(int));
	FillRandomArray(aux, n, 1, 20000, false , 0);
	Entry* v = initTable(N);
	for (int i = 0; i < n; i++) {
		v[i].id = aux[i];
		insertInTable(table, v[i], N);
	}
	int m = 3000;

	int sum = 0;
	int s = 0;
	int max1 = 0;
	int max2 = 0;
	for (int j = 0; j < 5; j++) {
		int* sr_aux = (int*)malloc(m * sizeof(int));
		//int* sr_aux2 = (int*)malloc(m * sizeof(int));
		int* sr = (int*)malloc(m * sizeof(int));
		//FillRandomArray(sr_aux, m / 2, 1, 8000, true, 0);
		for (int i = 0; i < m/2; i++) {
			sr_aux[i] = aux[i*2];
		}
		FillRandomArray(sr_aux + (m / 2), m / 2, 20001, 50000, false, 0);
		//FillRandomArray(sr_aux2, m / 2, 20001, 50000, true, 0);
		for (int i = 0; i < m-1; i += 2) {
			sr[i] = sr_aux[i / 2];
		}
		for (int i = 1; i < m; i += 2) {
			sr[i] = sr_aux[(m - 1) - i / 2];
		}
		op = 0;
		Entry* l = initTable(m);
		for (int i = 0; i < m; i++) {
			op = 0;
			int r = searchInTable(table, sr[i], N, &op);
			if (r == -1) {
				s += op;
				if (op > max1) {
					max1 = op;
				}
			}
			else {
				sum += op;
				if (op > max2)
					max2 = op;
			}
			//printf("%d\n", op);
			//sum = sum + op;
			//op = 0;
		}
	}
	printf("%17f", float(load_factor) / 100);

	float efort_total_gasite = float(sum) / 1500;
	efort_total_gasite = efort_total_gasite / 5;

	float efort_mediu_gasite = efort_total_gasite / n;
	printf("%22f ", efort_mediu_gasite);

	printf("%21d ", max2);

	float efort_total_negasite = float(s) / 1500;
	efort_total_negasite = efort_total_negasite / 5;
	//printf("%23d ", efort_total_negasite);

	float efort_mediu_negasite = float(efort_total_negasite) / n;
	printf("%23f ", efort_mediu_negasite);

	printf("%22d ", max1);

	printf("\n");
}

void average_case_dupa_stergere() {
	int N = 10007;
	int op = 0;
	int load_factor = 99;
	Entry* table = initTable(N);
	int n = (double(load_factor) / 100) * N;
	int* aux = (int*)malloc(N * sizeof(int));
	FillRandomArray(aux, n, 1, 20000, false, 0);
	Entry* v = initTable(N);
	for (int i = 0; i < n; i++) {
		v[i].id = aux[i];
		insertInTable(table, v[i], N);
	}
	int sum = 0;
	int m = 3000;
	int load_factor_2 = 80;
	int n2 = (double(load_factor_2) / 100) * N;
	for (int i = n2; i < n; i++) {
		deleteFromTable(table, v[i], m, &op);
	}
	int* sr_aux = (int*)malloc(m * sizeof(int));
	//int* sr_aux2 = (int*)malloc(m * sizeof(int));
	int* sr = (int*)malloc(m * sizeof(int));
	//FillRandomArray(sr_aux, m / 2, 1, 8000, true, 0);
	for (int i = 0; i < m / 2; i++) {
		sr_aux[i] = aux[i * 2];
	}
	FillRandomArray(sr_aux + (m / 2), m / 2, 20001, 50000, false, 0);
	//FillRandomArray(sr_aux2, m / 2, 20001, 50000, true, 0);
	for (int i = 0; i < m - 1; i += 2) {
		sr[i] = sr_aux[i / 2];
	}
	for (int i = 1; i < m; i += 2) {
		sr[i] = sr_aux[(m - 1) - i / 2];
	}
	for (int i = 0; i < 3000; i++) {
		op = 0;
		int r = searchInTable(table, sr[i], N, &op);
		sum += op;
	}
	int ef_necesar = sum / 3000;
	printf("\n%d", ef_necesar);
}

void perf() {
	printf("Factor de umplere    Efort mediu gasite    Efort maxim gasite    Efort mediu negasite    Efort maxim negasite\n");
	average_case(80);
	average_case(85);
	average_case(90);
	average_case(95);
	average_case(99);
	average_case_dupa_stergere();
}

int main()
{
	//demo_95();
	//demo();
	perf();
	return 0;
}