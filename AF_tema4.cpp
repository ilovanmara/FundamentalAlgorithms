/*
Nume: Ilovan Mara Gabriela
Grupa: 2
Tema 4: Interclasarea ecicienta a k siruri ordonate

Complexitatea interclasarii a k liste cu n elemnete este: O(n*logk)
Analizand graficele in care variem n-ul iar k e fix observam faptul ca daca avem mai putine liste
algoritmul e mai eficient.
Din al doilea grafic observam cresterea numarului de operatii o data cu cresterea numarului de liste.

*/

#include <stdio.h>
#include "Profiler.h"

Profiler p("interclasarea a k liste");

typedef struct _node {
	int key;
	struct _node* next;
}NodeT;

typedef struct List {
	NodeT* first;
	NodeT* last;
	int size;
}List;

NodeT* createNode(int key) {
	NodeT* p = (NodeT*)malloc(sizeof(NodeT));
	p->key = key;
	p->next = NULL;
	return p;
}

void listaVida(List* l) {
	l->first = NULL;
	l->last = NULL;
	l->size = 0;
}

void insertFirst(List* l, int Key) {
	NodeT* p = createNode(Key);
	if (l->first == NULL) {
		l->first = p;
		l->last = p;
	}
	else {
		p->next = l->first;
		l->first = p;
	}
}

void insertLast_op(List* l, int Key, Operation* Comp, Operation* Atr) {
	NodeT* p = createNode(Key);
	Comp->count(2);
	if (l->first == NULL && l->last == NULL) {
		Atr->count(2);
		l->last = p;
		l->first = p;
	}
	else {
		Atr->count(2);
		l->last->next = p;
		l->last = p;
	}
}

void insertLast(List* l, int Key) {
	NodeT* p = createNode(Key);
	if (l->first == NULL && l->last == NULL) {
		l->last = p;
		l->first = p;
	}
	else {
		l->last->next = p;
		l->last = p;
	}
}

int delete_first(List* l, Operation* Comp, Operation* Atr)
{
	NodeT* p = (NodeT*)malloc(sizeof(NodeT));
	int key;
	//if (l->first == NULL)
		//return -1;
	Comp->count();
	if (l->first != NULL)
	{
		Atr->count(2);
		p = l->first;
		l->first = l->first->next;
		key = p->key;
		free(p);
		Comp->count();
		if (l->first == NULL) {
			l->last = NULL;
			Atr->count();
		}
	}
	l->size--;
	return key;
}

int randomF(int a, int b) {
	srand(time(0));
	int random = (rand() % (b - a + 1)) + a;
	return random;
}

List** createArrayOfLists(int k) {
	List** a = (List**)malloc((k + 1) * sizeof(List*));
	for (int i = 0; i < k; i++) {
		a[i] = (List*)malloc(sizeof(List));
		a[i]->size = 0;
		a[i]->first = NULL;
		a[i]->last = NULL;
	}
	return a;
}

//liste de dimensiuni diferite dar cu un total de n elemente
List** generateLists(int n, int k) {
	//alocam vectorul de liste
	List** a = createArrayOfLists(k);

	srand(time(0));
	for (int i = 0; i < k; i++) {
		a[i]->size = 1;
	}
	//generez k numere random ce au suma n
	for (int i = k; i < n; i++) {
		int p = rand() % k;
		a[p]->size = a[p]->size + 1;
	}
	//inserez in liste elemente random in ordine crescatoare
	for (int i = 0; i < k; i++) {
		int* aux = (int*)malloc(a[i]->size * sizeof(int));
		FillRandomArray(aux, a[i]->size, 1, 500, false, ASCENDING);
		for (int j = 0; j < a[i]->size; j++)
			insertLast(a[i], aux[j]);
	}
	return a;
}

void printList(List l) {
	printf("\nlist: ");
	while (l.first != NULL) {
		printf("%d ", l.first->key);
		l.first = l.first->next;
	}
}

void min_heapify(List** a, int i, int n, Operation* Comp, Operation* Atr) {

	int l = 2 * i + 1;
	int r = 2 * i + 2;
	int largest = i;

	Comp->count();
	if (l < n && a[l]->first->key < a[i]->first->key)
		largest = l;

	Comp->count();
	if (r < n && a[r]->first->key < a[largest]->first->key)
		largest = r;
	
	if (largest != i) {
		List* aux = a[i];
		a[i] = a[largest];
		a[largest] = aux;
		Atr->count(3);

		min_heapify(a, largest, n, Comp, Atr);
	}
}

void build_min_heap_BottomUP(List** a, int n, Operation* Comp, Operation* Atr) {
	for (int i = (n / 2) - 1; i >= 0; i--)
		min_heapify(a, i, n, Comp, Atr);
}

void printArray(int v[], int n) {
	for (int i = 0; i < n; i++)
		printf("%d ", v[i]);
	printf("\n");
}

List* mergeSortedLists(List** a, int k, int n, Operation* OpAttr, Operation* OpComp) {

	List* res = (List*)malloc(sizeof(List));
	listaVida(res);
	int heapsize = k;

	build_min_heap_BottomUP(a, k, OpComp, OpAttr);
	while (heapsize > 0) {
		int key = delete_first(a[0], OpComp, OpAttr);
		insertLast_op(res, key, OpComp, OpAttr);
		OpComp->count();
		if (a[0]->first == NULL) {
			//List* aux= (List*)malloc(sizeof(List));
			//aux = a[0];
			OpAttr->count();
			a[0] = a[heapsize - 1];
			//a[heapsize - 1] = aux;
			heapsize--;
		}
		min_heapify(a, 0, heapsize, OpComp, OpAttr);
	}
	return res;
}

void average_case_n() {
	List** array; 
	
	int k = 5;
	for (int n = 100; n <= 10000; n += 100) {
		Operation OpAttr = p.createOperation("mergeSortedLists_atr1", n);
		Operation OpComp = p.createOperation("mergeSortedLists_comp1", n);	

		Operation OpAttr1 = p.createOperation("mergeSortedLists_atr2", n);
		Operation OpComp1 = p.createOperation("mergeSortedLists_comp2", n);

		Operation OpAttr2 = p.createOperation("mergeSortedLists_atr3", n);
		Operation OpComp2 = p.createOperation("mergeSortedLists_comp3", n);

		
		k = 5;
		array = generateLists(n, k);
		mergeSortedLists(array, k, n, &OpAttr, &OpComp);
		free(array);

		k = 10;
		array = generateLists(n, k);
		mergeSortedLists(array, k, n, &OpAttr1, &OpComp1);
		free(array);
		
		k = 100;
		array = generateLists(n, k);
		mergeSortedLists(array, k, n, &OpAttr2, &OpComp2);
		free(array);
		
	}
	p.addSeries("mergeSortedLists_total1", "mergeSortedLists_atr1", "mergeSortedLists_comp1");
	p.addSeries("mergeSortedLists_total2", "mergeSortedLists_atr2", "mergeSortedLists_comp2");
	p.addSeries("mergeSortedLists_total3", "mergeSortedLists_atr3", "mergeSortedLists_comp3");
	
	p.createGroup("total", "mergeSortedLists_total1", "mergeSortedLists_total2", "mergeSortedLists_total3");
	p.createGroup("atr", "mergeSortedLists_atr1", "mergeSortedLists_atr2", "mergeSortedLists_atr3");
	p.createGroup("comp", "mergeSortedLists_comp1", "mergeSortedLists_comp2", "mergeSortedLists_comp3");
	

	p.showReport();
}

void average_case_k() {
	int n = 10000;
	List** array;

	//Operation OpAttr = p.createOperation("mergeSortedLists_atr", k);
	//Operation OpComp = p.createOperation("mergeSortedLists_comp", k);

	for (int k = 10; k <= 500; k += 10) {
		Operation OpAttr = p.createOperation("mergeSortedLists_atr", k);
		Operation OpComp = p.createOperation("mergeSortedLists_comp", k);

		array = generateLists(n, k);
		mergeSortedLists(array, k, n, &OpAttr, &OpComp);
		free(array);
	}
	p.addSeries("mergeSortedLists_total", "mergeSortedLists_atr", "mergeSortedLists_comp");
	p.showReport();
}

void demo() {
	int k = 3;
	int n = 20;
	List** a = generateLists(n, k);
	for (int i = 0; i < k; i++) {
		printList(*a[i]);
	}
	Operation Atr = p.createOperation("mergeSortedLists_atr3", n);
	Operation Comp = p.createOperation("mergeSortedLists_comp3", n);
	List* res = mergeSortedLists(a, k, n, &Atr, &Comp);
	printList(*res);
}

int main() {
	demo();
	//average_case_k();
	return 0;
}