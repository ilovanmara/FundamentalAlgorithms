/*
Nume Prenume: Ilovan Mara Gabriela
Grupa: 2
Tema nr. 3: Analiza si compararea metodelor avansate de sortatre 
            HeapSort si QuickSort
Complexitate: (in practica QuickSort-ul mai eficient)
	WORST CASE: HeapSort O(nlogn), QuickSort O(n^2) 
	AVERAGE CASE: HeapSort(nlog n), QuickSort O(nlogn)
	BEST CASE:  O(nlogn) 

Comparand graficele putem vedea fapul ca in average case QuickSort-ul este putin mai eficient decat HeapSort.

Pentru QuickSort avem nevoie de un element pivot (elementele din stanga mai mici ca el, elementele din dreapta mai mari ca el).
Astfel se realizaeaza paritionarea sirului, realizata recursiv pana cand sirul este ordonat crescatot.

QuickSelectul returneaza al k lea cel mai mic element din sir. Pentru realizarea lui am fonosit o functie de partitionare 
randomizata.

Dupa compararea SelectionSort-ului recursiv si iterativ am constatat faptul ca in privinta comparatiilor si aributiilor nu sunt
diferente, in schimb diferentele apar la nivelul timpului de executie, metoda recursiva fiind mai ineficienta din acest punct de 
vedere.

*/

#include <stdio.h>
#include <string.h>

#include "Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100

Profiler p("QuickSort vs HeapSort");

int partition(int a[], int left, int right, int n) {
	Operation OpAtr1 = p.createOperation("QuickSort_atr", n);
	Operation OpComp1 = p.createOperation("QuickSort_comp", n);

	OpAtr1.count();
	int x = a[right];
	int i = left - 1;
	int aux;
	for (int j = left; j < right; j++) {
		OpComp1.count();
		if (a[j] < x) {
			i++;
			aux = a[i];
			a[i] = a[j];
			a[j] = aux;
			OpAtr1.count(3);
		}
	}
	aux = a[i + 1];
	a[i + 1] = a[right];
	a[right] = aux;
	OpAtr1.count(3);

	return i + 1;
}

void QuickSort(int a[], int left, int right, int n) {
	if (left < right) {
		int q = partition(a, left, right, n);
		QuickSort(a, left, q - 1, n);
		QuickSort(a, q + 1, right, n);
	}
}

void max_heapify(int a[], int i, int n, Operation* atr, Operation* comp) {

	//Operation OpAttr1 = p.createOperation("BuildHeap_BottomUp_atr", n);
	//Operation OpComp1 = p.createOperation("BuildHeap_BottomUp_comp", n);

	int l = 2 * i + 1;
	int r = 2 * i + 2;
	int largest = i;

	(* comp).count();
	if (l < n && a[l] > a[i])
		largest = l;

	(* comp).count();
	if (r < n && a[r] > a[largest])
		largest = r;

	if (largest != i) {
		int aux = a[i];
		a[i] = a[largest];
		a[largest] = aux;
		(* atr).count(3);

		max_heapify(a, largest, n, atr, comp);
	}
}

void build_max_heap_BottomUP(int a[], int n, Operation* atr, Operation* comp) {
	for (int i = (n / 2) - 1; i >= 0; i--)
		max_heapify(a, i, n, atr, comp);
}

void heap_sort(int a[], int n) {

	Operation OpAtr2 = p.createOperation("HeapSort_atr", n);
	Operation OpComp2= p.createOperation("HeapSort_comp", n);

	build_max_heap_BottomUP(a, n, &OpAtr2, &OpComp2);
	for (int i = n - 1; i > 0; i--) {

		OpAtr2.count();
		int aux = a[0];
		a[0] = a[i];
		a[i] = aux;

		max_heapify(a, 0, i, &OpAtr2, &OpComp2);
	}
}

void print_array(int v[], int n) {
	for (int i = 0; i < n; i++)
		printf("%d ", v[i]);
	printf("\n");
}

void save(int v[], int aux[], int n) {
	for (int i = 0; i < n; i++)
		aux[i] = v[i];
}

void selection_sort(int v[], int n) {
	Operation opComp1 = p.createOperation("selection_sort_comp", n);
	Operation opAtrr1 = p.createOperation("selection_sort_atr", n);
	int pos_min = 0;
	for (int i = 0; i < n; i++) {
		pos_min = i;
		for (int j = i + 1; j < n; j++) {
			opComp1.count();
			if (v[j] < v[pos_min])
				pos_min = j;
		}
		if (i != pos_min) {
			opAtrr1.count(3);
			int aux = v[i];
			v[i] = v[pos_min];
			v[pos_min] = aux;
		}
	}
}

int index_min(int v[], int i, int j, Operation* comp, Operation* atr) {
	if (i == j)
		return i;
	int m = index_min(v, i + 1, j, comp, atr);
	comp->count();
	if (v[i] < v[m])
		return i;
	else return m;
}

void selection_sort_recursiv(int a[], int n, int pos_min) {
	Operation opComp2 = p.createOperation("selection_sort_recursiv_comp", n);
	Operation opAtrr2 = p.createOperation("selection_sort_recursiv_atr", n);

	if (pos_min == n)
		return;
	int m = index_min(a, pos_min, n - 1, &opComp2, &opAtrr2);
	if (m != pos_min) {
		opAtrr2.count(3);
		int aux = a[m];
		a[m] = a[pos_min];
		a[pos_min] = aux;
	}
	selection_sort_recursiv(a, n, pos_min + 1);
}

int randomized_partition(int v[], int left, int right, int n) {
	srand(time(0));
	int random = (rand() % (right - left + 1)) + left;
	int aux;
	aux = v[right];
	v[right] = v[random];
	v[random] = aux;

	return partition(v, left, right, n);
}

int QuickSelect(int a[], int left, int right, int i, int n) {
	if (left == right)
		return a[left];
	int q = randomized_partition(a, left, right, n);
	int k = q - left + 1;
	if (i == k)
		return a[q];
	else if (i < k)
		return QuickSelect(a, left, q - 1, i, n);
	else return QuickSelect(a, q + 1, right, i - k, n);
}

void demo() {
	int v[] = { 5, 12, 4, 25, 19, 3, 7, 9 };
	int n = sizeof(v) / sizeof(v[0]);
	print_array(v, n);
	int* aux = (int*)malloc(n * sizeof(int));
	save(v, aux, n);
	int k = 8;
	//selection_sort_recursiv(v, n, 0);
	int i = QuickSelect(aux, 0, n - 1, k, n);
	printf("%d\n", i);
	QuickSort(v, 0, n-1, n);
	print_array(v, n);
}

void average_case() {
	int v[MAX_SIZE];
	int aux[MAX_SIZE];
	
	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		for (int i = 0; i < 5; i++) {
			FillRandomArray(v, n, 10, 50000, false, 0);
			save(v, aux, n);

			QuickSort(v, 0, n - 1, n);
			heap_sort(aux, n);
		}
	}

	p.divideValues("QuickSort_atr", 5);
	p.divideValues("QuickSort_comp", 5);
	p.divideValues("HeapSort_atr", 5);
	p.divideValues("HeapSort_comp", 5);

	p.addSeries("QuickSort_total", "QuickSort_atr", "QuickSort_comp");
	p.addSeries("HeapSort_total", "HeapSort_atr", "HeapSort_comp");

	p.createGroup("Average_case_atr", "QuickSort_atr", "HeapSort_atr");
	p.createGroup("Average_case_comp", "QuickSort_comp", "HeapSort_comp");
	p.createGroup("Average_case_total", "QuickSort_total", "HeapSort_total");
	
	
	p.showReport();
}

void best_case() {
	int v[MAX_SIZE];
	int a[MAX_SIZE];

	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		//FillRandomArray(a, n, 10, 50000, false, DESCENDING);
		//heap_sort(a, n);
		FillRandomArray(v, n, 10, 50000, false, 0);
		QuickSort(v, 0, (n-1)/2, n);
	}

	p.addSeries("QuickSort_total", "QuickSort_atr", "QuickSort_comp");
	//p.addSeries("HeapSort_total", "HeapSort_atr", "HeapSort_comp");

	//p.createGroup("BectCase_atr", "QuickSort_atr", "HeapSort_atr");
	//p.createGroup("BectCase_comp", "QuickSort_comp", "HeapSort_comp");
	//p.createGroup("BectCase_total", "QuickSort_total", "HeapSort_total");

	p.showReport();
}

void worst_case() {
	int v[MAX_SIZE];
	int a[MAX_SIZE];

	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		FillRandomArray(v, n, 10, 50000, false,	ASCENDING);
		//save(v, a, n);
		QuickSort(v, 0, n - 1, n);
		//heap_sort(a, n);
	}

	p.addSeries("QuickSort_total", "QuickSort_atr", "QuickSort_comp");
	//p.addSeries("HeapSort_total", "HeapSort_atr", "HeapSort_comp");
	//p.showReport();
	//p.reset("impreuna");

	//p.createGroup("WorstCase_atr", "QuickSort_atr", "HeapSort_atr");
	//p.createGroup("WorstCase_comp", "QuickSort_comp", "HeapSort_comp");
	//p.createGroup("WorstCase_total", "QuickSort_total", "HeapSort_total");

	p.showReport();
}

void perf() {
	average_case();
	p.reset("best_case");
	best_case();
	p.reset("worst_case");
	worst_case();
	p.showReport();
}

void average_case_selectionSort() {
	int v[MAX_SIZE];
	int aux[MAX_SIZE];

	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		//p.startTimer()
		for (int i = 0; i < 10; i++) {
			FillRandomArray(v, n, 10, 50000, false, 0);
			save(v, aux, n);

			selection_sort(v, n);
			selection_sort_recursiv(aux, n, 0);
		}
	}

	p.divideValues("selection_sort_atr", 10);
	p.divideValues("selection_sort_comp", 10);
	p.divideValues("selection_sort_recursiv_atr", 10);
	p.divideValues("selection_sort_recursiv_comp", 10);

	p.addSeries("selection_sort_total", "selection_sort_atr", "selection_sort_comp");
	p.addSeries("selection_sort_recursiv_total", "selection_sort_recursiv_atr", "selection_sort_recursiv_comp");

	p.createGroup("Average_case_sel_atr", "selection_sort_atr", "selection_sort_recursiv_atr");
	p.createGroup("Average_case_sel_comp", "selection_sort_comp", "selection_sort_recursiv_comp");
	p.createGroup("Average_case_sel_total", "selection_sort_total", "selection_sort_recursiv_total");

	p.showReport();
}

void average_case_timer_iterativ() {
	int v[MAX_SIZE];
	int aux[MAX_SIZE];

	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		p.startTimer("selection_sort", n);
		for (int i = 0; i < 100; i++) {
			FillRandomArray(v, n, 10, 50000, false, 0);
			selection_sort(v, n);
		}
		p.stopTimer("selection_sort", n);
	}

	//p.addSeries("selection_sort_total", "selection_sort_atr", "selection_sort_comp");

	p.showReport();
}
void average_case_timer_iterativ1() {
	int v[MAX_SIZE];
	int aux[MAX_SIZE];

	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		//p.startTimer("selection_sort", n);
		for (int i = 0; i < 100; i++) {
			FillRandomArray(v, n, 10, 50000, false, 0);
			selection_sort(v, n);
		}
		//p.stopTimer("selection_sort", n);
	}
	p.divideValues("selection_sort_atr", 100);
	p.divideValues("selection_sort_comp", 100);

	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		p.startTimer("selection_sort", n);
		for (int i = 0; i < 100; i++) {
			FillRandomArray(v, n, 10, 50000, false, 0);
			selection_sort(v, n);
		}
		p.stopTimer("selection_sort", n);
	}

	p.showReport();
}

void average_case_timer_recursiv() {
	int v[MAX_SIZE];
	int aux[MAX_SIZE];

	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		p.startTimer("selection_sort_recursiv", n);
		for (int i = 0; i < 100; i++) {
			FillRandomArray(v, n, 10, 50000, false, 0);
			selection_sort_recursiv(v, n, 0);
		}
		p.stopTimer("selection_sort_recursiv", n);
	}

	p.showReport();
}

void average_case_timer_recursiv1() {
	int v[MAX_SIZE];
	int aux[MAX_SIZE];

	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		//p.startTimer("selection_sort_recursiv", n);
		for (int i = 0; i < 100; i++) {
			FillRandomArray(v, n, 10, 50000, false, 0);
			save(v, aux, n);
			selection_sort(aux, n);
			selection_sort_recursiv(v, n, 0);
		}
		//p.stopTimer("selection_sort_recursiv", n);
	}
	p.divideValues("selection_sort_recursiv_atr", 100);
	p.divideValues("selection_sort_recursiv_comp", 100);
	p.divideValues("selection_sort_atr", 100);
	p.divideValues("selection_sort_comp", 100);

	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		p.startTimer("selection_sort_recursiv", n);
		for (int i = 0; i < 100; i++) {
			FillRandomArray(v, n, 10, 50000, false, 0);
			selection_sort_recursiv(v, n, 0);
		}
		p.stopTimer("selection_sort_recursiv", n);
	}

	p.addSeries("selection_sort_total", "selection_sort_atr", "selection_sort_comp");
	p.addSeries("selection_sort_recursiv_total", "selection_sort_recursiv_atr", "selection_sort_recursiv_comp");

	p.createGroup("Average_case_sel_atr", "selection_sort_atr", "selection_sort_recursiv_atr");
	p.createGroup("Average_case_sel_comp", "selection_sort_comp", "selection_sort_recursiv_comp");
	p.createGroup("Average_case_sel_total", "selection_sort_total", "selection_sort_recursiv_total");

	p.showReport();
}

void perf2() {
	average_case_timer_iterativ1();
	p.reset("recursiv");
	average_case_timer_recursiv1();
	p.showReport();
}

int main()
{
	demo();
	//perf();
	//average_case_selectionSort();
	//average_case_timer_recursiv1();
	//perf2();
	return 0;
}