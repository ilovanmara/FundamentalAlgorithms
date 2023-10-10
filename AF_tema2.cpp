/*
Nume: Ilovan Mara Gabriela
Tema nr 2: Analiza si combararea Bottom-up si Top-down

Complexitate: Bottom-up O(n)
			  Top-down O(n*logn)
Heapsort O(n*logn)
Pentru crearea heap-ului in varianta bottom up am utilizat functia heapify care compara copiii
unui nod cu el si interschimba daca e cazul
Pentru crearea heap-ului in varianta top down am utilizat inserarea unei valori in heap
Dupa analizarea graficelor observam fapul ca implementarea Bottom-up este mai eficienta atat
in worst case cat si in average case.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Profiler.h"

//daca incepe de la 1
// parent i/2
//left 2i
//right 2i+1


Profiler p("Bottom Up vs Top Down");

#define MAX_SIZE 10000
#define STEP_SIZE 100

void max_heapify(int a[], int i, int n) {

	Operation OpAttr1 = p.createOperation("BuildHeap_BottomUp_atr", n);
	Operation OpComp1 = p.createOperation("BuildHeap_BottomUp_comp", n);

	int l = 2 * i + 1;
	int r = 2 * i + 2;
	int largest = i;
	
	OpComp1.count();
	if (l < n && a[l] > a[i])
		largest = l;
	//else largest = i;

	OpComp1.count();
	if (r < n && a[r] > a[largest])
		largest = r;
	//OpComp1.count();
	if (largest != i) {
		int aux = a[i];
		a[i] = a[largest];
		a[largest] = aux;
		OpAttr1.count(3);

		max_heapify(a, largest, n);
	}
}

void build_max_heap_BottomUP(int a[], int n) {
	for (int i = (n / 2) - 1; i >= 0; i--)
		max_heapify(a, i, n);
}

void heap_sort(int a[], int n) {
	build_max_heap_BottomUP(a, n);
	for (int i = n - 1; i > 0; i--) {
		int aux = a[0];
		a[0] = a[i];
		a[i] = aux;

		max_heapify(a, 0, i);
	}
}

void print_array(int v[], int n) {
	for (int i = 0; i < n; i++)
		printf("%d ", v[i]);
	printf("\n");
}

int heap_extract_max(int a[], int n) {
	if (n < 1)
		return -1;
	int max = a[0];
	n = n - 1;
	max_heapify(a, 1, n);
	return max;
}

void heap_increase_key(int a[], int i, int key, int n) {
	Operation OpAttr2 = p.createOperation("BuildHeap_TopDown_atr", n);
	Operation OpComp2 = p.createOperation("BuildHeap_TopDown_comp", n);

	OpComp2.count();
	if (key < a[i])
		return;
	OpAttr2.count();
	a[i] = key;
	while (i > 0 && a[(i - 1) / 2] < a[i]) {
		OpComp2.count();
		OpAttr2.count(3);
		int aux = a[i];
		a[i] = a[(i - 1) / 2];
		a[(i - 1) / 2] = aux;
		i = (i - 1) / 2;
	}
	OpComp2.count();
}

void max_heap_insert(int a[], int array[], int key, int* heap_size, int n) {
	
	(* heap_size)++;
	array[*heap_size] = -1;
	heap_increase_key(array, *heap_size, key, n);
}

void build_max_heap_TopDown(int a[],int array[], int n) {
	int heap_size = -1;

	for (int i = 0; i < n; i++)
		max_heap_insert(a, array, a[i], &heap_size, n);
	//printf("%d", n);
}

void save(int v[], int aux[], int n) {
	for (int i = 0; i < n; i++)
		aux[i] = v[i];
}

void average_case() {
	int v[MAX_SIZE];
	int aux[MAX_SIZE];
	int array[MAX_SIZE];
	
	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		for (int i = 0; i < 5; i++) {
			FillRandomArray(v, n, 10, 50000, false, 0);
			save(v, aux, n);
			build_max_heap_BottomUP(aux, n);
			build_max_heap_TopDown(v, array, n);
		}
	}

	p.divideValues("BuildHeap_BottomUp_atr", 5);
	p.divideValues("BuildHeap_BottomUp_comp", 5);
	p.divideValues("BuildHeap_TopDown_atr", 5);
	p.divideValues("BuildHeap_Topdown_atr", 5);

	p.addSeries("BottomUp_total", "BuildHeap_BottomUp_atr", "BuildHeap_BottomUp_comp");
	p.addSeries("TopDown_total", "BuildHeap_TopDown_atr", "BuildHeap_TopDown_comp");

	p.createGroup("Average_case_comp", "BuildHeap_BottomUp_comp", "BuildHeap_TopDown_comp");
	p.createGroup("Average_case_atr", "BuildHeap_BottomUp_atr", "BuildHeap_TopDown_atr");
	p.createGroup("Average_case_total", "BottomUp_total", "TopDown_total");

	p.showReport();
}

void worst_case() {
	int v[MAX_SIZE];
	int aux[MAX_SIZE];
	int array[MAX_SIZE];

	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		FillRandomArray(v, n, 10, 50000, false, 1);
		save(v, aux, n);
		build_max_heap_BottomUP(aux, n);
		build_max_heap_TopDown(v, array, n);
	}

	p.addSeries("BottomUp_total", "BuildHeap_BottomUp_atr", "BuildHeap_BottomUp_comp");
	p.addSeries("TopDown_total", "BuildHeap_TopDown_atr", "BuildHeap_TopDown_comp");

	p.createGroup("Worst_case_comp", "BuildHeap_BottomUp_comp", "BuildHeap_TopDown_comp");
	p.createGroup("Worst_case_atr", "BuildHeap_BottomUp_atr", "BuildHeap_TopDown_atr");
	p.createGroup("Worst_case_total", "BottomUp_total", "TopDown_total");

	p.showReport();
}

void bubble_sort(int v[], int n) {
	Operation  opComp1 = p1.createOperation("bubble_sort_comp", n);
	Operation opAtrr1 = p1.createOperation("bubble_sort_atrr", n);
	int sortat = 0;
	int i = 0;
	while (sortat == 0) {
		sortat = 1;
		for (int j = 0; j < n - 1; j++) {
			opComp1.count();
			if (v[j] > v[j + 1]) {
				opAtrr1.count(3);
				int aux = v[j];
				v[j] = v[j + 1];
				v[j + 1] = aux;
				sortat = 0;
			}
		}
	}
}

void selection_sort(int v[], int n) {
	Operation opComp3 = p1.createOperation("selection_sort_comp", n);
	Operation opAtrr3 = p1.createOperation("selection_sort_atrr", n);
	int pos_min = 0;
	for (int i = 0; i < n; i++) {
		pos_min = i;
		for (int j = i + 1; j < n; j++) {
			opComp3.count();
			if (v[j] < v[pos_min])
				pos_min = j;
		}
		if (i != pos_min) {
			opAtrr3.count(3);
			int aux = v[i];
			v[i] = v[pos_min];
			v[pos_min] = aux;
		}
	}
}

int index_min(int v[], int i, int j) {
	if (i == j)
		return i;
	int m = index_min(v, i + 1, j);
	if (a[i] < a[k])
		return i;
	else return k;
}

void selection_sort_recrusiv(int v[], int n, int pos_min) {
	//int pos_min = 0;
	if (pos_min == n)
		return;
	int m = index_min(v, pos_min, n - 1);
	if (m != pos_min) {
		int aux = a[m];
		a[m] = a[pos_min];
		a[pos_min] = aux;
	}
	selection_sort_recursiv(v, n, pos_min + 1);
}

void perf() {
	worst_case();
	p.reset("average");
	average_case();
	p.showReport();
}

void demo() {
	int a[] = { 4, 1, 3, 2, 16, 9, 10, 14, 8, 7 };
	int a1[] = { 1, 2, 3, 4, 7, 8, 9, 10, 14, 16};
	int n = sizeof(a) / sizeof(a[0]);
	int* aux =(int*) malloc(n * sizeof(int));
	int* array = (int*)malloc(n * sizeof(int));
	save(a, aux, n);
	build_max_heap_BottomUP(a, n);
	print_array(a, n);
	heap_sort(a, n);
	printf("vectorul sortat: ");
	print_array(a, n);
	int heap_size = n;
	build_max_heap_TopDown(a1,array, n);
	print_array(array, n);
	//printf("%d", n);
}

int main()
{
	demo();
	//perf();
	return 0;
}