/*
Tema nr 9: kruskal
Nume: Ilovan Mara
Grupa: 2

La aceasta tema am implementat algoritmul lui Kruskal cu multimi disjuncte si operatiile pe multimi disjuncte:
- make-set: creaza multimea
- find-set: returneaza radacina multimii din care apartine elementul
- union: uneste doua multimi

Pentru algoritmul lui Kruskal pe un graf am avut nevoie de o structura de muchie, care contine nodurile si ponderea dintre ele.
Complexitate O(e * log e), unde e este numarul de muchii. Dupa aplicarea aloritmului obtinem un minimum spanning tree.
*/
#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

typedef struct node_{
	int rank;
	int key;
	struct node_* parent;
}Node;

typedef struct edge_ {
	Node* node1;
	Node* node2;
	int pondere;
}edge;

Profiler p("Kruskal");

Node* createNode(int key) {
	Node* x = (Node*)malloc(sizeof(Node));
	x->key = key;
	return x;
}

void make_set(Node* x, Operation* op) {
	op->count(2);
	x->parent = x;
	x->rank = 0;
}

void link(Node* x, Node* y, Operation* op) {
	op->count();
	if (x->rank > y->rank) {
		y->parent = x;
		op->count();
	}
	else {
		op->count();
		x->parent = y;
		op->count();
		if (x->rank == y->rank) {
			y->rank = y->rank + 1;
			op->count();
		}
	}
}

Node* find_set(Node* x, Operation* op) {
	op->count();
	if (x != x->parent) {
		x->parent = find_set(x->parent, op);
		op->count();
	}
	return x->parent;
}

void Union(Node* x, Node* y, Operation* op) {
	link(find_set(x, op), find_set(y, op), op);
}

int partition(edge* a[], int left, int right, int n) {
	int x = a[right]->pondere;
	int i = left - 1;
	edge* aux = (edge*)malloc(sizeof(edge));
	for (int j = left; j < right; j++) {
		if (a[j]->pondere < x) {
			i++;
			aux = a[i];
			a[i] = a[j];
			a[j] = aux;
		}
	}
	aux = a[i + 1];
	a[i + 1] = a[right];
	a[right] = aux;

	return i + 1;
}

void QuickSort(edge* a[], int left, int right, int n) {
	if (left < right) {
		int q = partition(a, left, right, n);
		QuickSort(a, left, q - 1, n);
		QuickSort(a, q + 1, right, n);
	}
}

edge** Kruskal(edge** muchii, Node** noduri, int nr_noduri, int nr_muchii, int* nr_muchii_res) {

	Operation op = p.createOperation("MultimiDisjuncteKruskal", nr_noduri);

	edge** result = (edge**)malloc(nr_muchii * sizeof(edge*));
	for (int i = 0; i < nr_noduri; i++) {
		//createNode(i);
		make_set(noduri[i], &op);
	}
	int cost = 0;
	QuickSort(muchii, 0, nr_muchii - 1, nr_muchii);
	
	int j = 0;
	int i = 0;
	//for (int i = 0; i < nr_muchii, j < (nr_muchii - 1) ; i++) {
	while(j < (nr_noduri - 1)){
		edge* next_edge = muchii[i];
		if (find_set(muchii[i]->node1, &op) != find_set(muchii[i]->node2, &op)) {
			cost += muchii[i]->pondere;
			result[j] = next_edge;
			j++;
			Union(muchii[i]->node1, muchii[i]->node2, &op);
		}
		i++;
	}
	*nr_muchii_res = j;
	return result;
}

void demo(){

	int n = 12;
	Node** multime = (Node**)malloc(n * sizeof(Node));

	Operation op = p.createOperation("MultimiDisjuncteKruskal", n);

	for (int i = 0; i < n; i++) {
		multime[i] = createNode(i);
		make_set(multime[i], &op);
		printf("Nodul %d il are ca parinte pe nodul %d\n", multime[i]->key, find_set(multime[i], &op)->key);
	}

	Union(multime[0], multime[1], &op);
	Union(multime[2], multime[3], &op);
	Union(multime[4], multime[5], &op);
	Union(multime[6], multime[7], &op);
	Union(multime[8], multime[9], &op);
	Union(multime[0], multime[10], &op);
	Union(multime[1], multime[11], &op);

	printf("\n");

	for (int i = 0; i < n; i++) {
		printf("Nodul % d il are ca parinte pe nodul % d\n", multime[i]->key, find_set(multime[i], &op)->key);
	}
}

edge* createEdge(Node* node1, Node* node2, int pondere) {
	edge* e = (edge*)malloc(sizeof(edge));
	e->node1 = node1;
	e->node2 = node2;
	e->pondere = pondere;
	return e;
}

void print(edge** muchii, int n) {
	for (int i = 0; i < n; i++) {
		printf("muchia cu ponderea %d intre nodurile %d si %d\n", muchii[i]->pondere, muchii[i]->node1->key, muchii[i]->node2->key);
	}
}

void demo_kruskal() {
	edge** res;

	int nr_noduri = 7;
	int nr_muchii = 8;

	Node** noduri = NULL;
	noduri = (Node**)malloc(nr_noduri * sizeof(Node*));
	edge** muchii = (edge**)malloc(nr_muchii * sizeof(edge));
	for (int i = 0; i < nr_muchii; i++) {
		muchii[i] = (edge*)malloc(sizeof(edge));
	}
	
	for (int i = 0; i < nr_noduri; i++) {
		noduri[i] = createNode(i);
		//make_set(noduri[i]);
	}
	
	muchii[0] = createEdge(noduri[0], noduri[1], 80);
	muchii[1] = createEdge(noduri[1], noduri[2], 78);
	muchii[2] = createEdge(noduri[2], noduri[3], 23);
	muchii[3] = createEdge(noduri[3], noduri[4], 103);
	muchii[4] = createEdge(noduri[5], noduri[6], 65);
	muchii[5] = createEdge(noduri[0], noduri[6], 70);
	muchii[6] = createEdge(noduri[1], noduri[6], 8);
	muchii[7] = createEdge(noduri[2], noduri[4], 13);

	print(muchii, nr_muchii);

	int n;

	res = Kruskal(muchii, noduri, nr_noduri, nr_muchii, &n);

	printf("%d", n);

	printf("\nMuchiile sortate:\n");
	print(muchii, nr_muchii);
	printf("\nArborele rezultat:\n");
	print(res, n);
}

void evaluare() {
	//int n = 100;
	for (int n = 100; n <= 10000; n += 100) {
		//graf aleatoriu cu n elemente si n * 4 muchii
		Node** noduri = (Node**)malloc(n * sizeof(Node*));
		edge** muchii = (edge**)malloc((n * 4) * sizeof(edge*));
		int* ponderi = (int*)malloc((n * 4) * sizeof(int));

		for (int i = 0; i < n; i++) {
			noduri[i] = createNode(i);
		}

		FillRandomArray(ponderi, n * 4, 1, 200, false, 0);
		
		int j = 1;
		int k = 1;
		for (int i = 0; i < n - 1; i++) {
			muchii[i] = createEdge(noduri[0], noduri[i + 1], ponderi[i]);
		}
		for (int i = n - 1; i < n * 4; i++) {
			if (j == k) {
				k += 3;
			}
			if (k >= n)
				k = 2;
			muchii[i] = createEdge(noduri[j], noduri[k], ponderi[i]);
			j += 2;
			k += 3;
			if (k >= n)
				k = 2;
			if (j >= n)
				j = n/2;
		}
		//print(muchii, n * 4);
		int nr_muchii_res;
		edge** res = Kruskal(muchii, noduri, n, n * 4, &nr_muchii_res);
	}
	p.showReport();
}


int main() {
	//demo();
	demo_kruskal();
	//evaluare();
	return 0;
}