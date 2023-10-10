/*
Nume Prenume: Ilovan Mara Gabriela
Grupa: 2
Tema 1 AF: Metode de sortare directe

In aceasta tema am implementat 3 variante de sortare: 
Bubble Sort, Insetion Sort si Selction Sort

Toti cei trei algoritmi au fost testati pe vectorul:
v = { 5, 2, 3, 7, 1, 12, 4 } si functioneaza

Bubble Sort - stabil
Insertion Sort - stabil
Selection Sort - nu e stabil

Dupa ce am analizat fiecare sortare in cazurile worst, best si average am observat ca:
Best case: O(n) pt bubble sort si insetion sort si O(n^2) pt selection sort
bubble sort face n-1 comparatii si 0 asignari
insetion sort face 2*(n-1) atribuiri si n-1 comparatii
iar seletion sort face 0 atribuiri si n*(n-1)/2 comparatii
Average case: O(n^2) pt toate
cele mai putine atribuiri sunt facute in selection sort, 
cele mai putine comparatii sunt facure in insertion sort
Worst Case: O(n^2) pt toate
cele mai putine comparatii si stribuiri sunt facute in selection sort
*/


#include <stdio.h>
#include <string.h>
#include "Profiler.h"

Profiler p1("sortari");

#define MAX_SIZE 9000
#define STEP_SIZE 100


void bubble_sort(int v[], int n) {
	Operation  opComp1 = p1.createOperation("bubble_sort_comp", n);
	Operation opAtrr1 = p1.createOperation("bubble_sort_atrr", n);
	int sortat = 0;
	int i = 0;
	while(sortat == 0){
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

void insetion_sort(int v[], int n) {
	Operation opComp2 = p1.createOperation("insetion_sort_comp", n);
	Operation opAtrr2 = p1.createOperation("insetion_sort_atrr", n);
	for (int i = 1; i < n; i++) {
		opAtrr2.count();
		int sel = v[i];
		int j = i - 1;
		while (j > -1 && v[j] > sel) {
			opComp2.count();
			opAtrr2.count();
			v[j + 1] = v[j];
			j--;
		}
		opComp2.count();
		opAtrr2.count();
		v[j + 1] = sel;
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

void save(int v[], int aux[], int n) {
	for (int i = 0; i < n; i++)
		aux[i] = v[i];
}

void average_case() {
	int v[MAX_SIZE];
	int aux[MAX_SIZE];
	int n;
	for (int n = STEP_SIZE; n <= MAX_SIZE; n+=STEP_SIZE) {
		for (int i = 0; i < 5; i++) {
			FillRandomArray(v, n, 10, 50000, false, 0);
			save(v, aux, n);
			bubble_sort(aux, n);
			save(v, aux, n);
			insetion_sort(aux, n);
			selection_sort(v, n);
		}
	}
	p1.divideValues("bubble_sort_comp", 5);
	p1.divideValues("bubble_sort_atrr", 5);
	p1.divideValues("insetion_sort_atrr", 5);
	p1.divideValues("insetion_sort_comp", 5);
	p1.divideValues("selection_sort_comp", 5);
	p1.divideValues("selection_sort_atrr", 5);

	p1.addSeries("bubbleSort", "bubble_sort_comp", "bubble_sort_atrr");
	p1.addSeries("insetionSort", "insetion_sort_comp", "insetion_sort_atrr");
	p1.addSeries("selectionSort", "selection_sort_comp", "selection_sort_atrr");
	
	p1.createGroup("Average_case_comp", "bubble_sort_comp", "insetion_sort_comp", "selection_sort_comp");
	p1.createGroup("Average_case_atrr", "bubble_sort_atrr", "insetion_sort_atrr");
	//am pus nr de atribuiri de la selection sort separat pentru a se vedea cresterea
	p1.createGroup("Average_case_total", "bubbleSort", "insetionSort", "selectionSort");

	p1.showReport();
}

// best case: sirul este sortat in ordine crescatoare
// pentru ca graficul nu arata bine cresterea  comparatilor la bubblesort si insertionsort le-am afisat separat
void best_case() {
	int v[MAX_SIZE];
	int aux[MAX_SIZE];
	int n;
	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		
		FillRandomArray(v, n, 10, 50000, false, ASCENDING);
		save(v, aux, n);
		bubble_sort(aux, n);
		save(v, aux, n);
		insetion_sort(aux, n);
		selection_sort(v, n);
	}
	
	p1.addSeries("bubbleSort", "bubble_sort_comp", "bubble_sort_atrr");
	p1.addSeries("insetionSort", "insetion_sort_comp", "insetion_sort_atrr");
	p1.addSeries("selectionSort", "selection_sort_comp", "selection_sort_atrr");

	//p1.createGroup("Best_case_comp", "bubble_sort_comp", "insetion_sort_comp", "selection_sort_comp");
	p1.createGroup("Best_case_atrr", "bubble_sort_atrr", "insetion_sort_atrr", "selection_sort_atrr");
	//p1.createGroup("Best_case_total", "bubbleSort", "insetionSort", "selectionSort");

	p1.showReport();
}

// worst case sirul este in ordine descrescatoare
void worst_case() {
	int v[MAX_SIZE];
	int aux[MAX_SIZE];
	int n;
	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {

		FillRandomArray(v, n, 10, 50000, false, DESCENDING);
		save(v, aux, n);
		bubble_sort(aux, n);
		save(v, aux, n);
		insetion_sort(aux, n);
		selection_sort(v, n);

	}

	p1.addSeries("bubbleSort", "bubble_sort_comp", "bubble_sort_atrr");
	p1.addSeries("insetionSort", "insetion_sort_comp", "insetion_sort_atrr");
	p1.addSeries("selectionSort", "selection_sort_comp", "selection_sort_atrr");

	p1.createGroup("Worst_case_comp", "bubble_sort_comp", "insetion_sort_comp", "selection_sort_comp");
	p1.createGroup("Worst_case_atrr", "bubble_sort_atrr", "insetion_sort_atrr");
	p1.createGroup("Worst_case_total", "bubbleSort", "insetionSort", "selectionSort");

	p1.showReport();
}


void perf_all() {
	best_case();
	p1.reset("average");
	average_case();
	p1.reset("worst");
	worst_case();
	p1.showReport();
 }

void demo1()
{
	int v[] = { 5, 2, 3, 7, 1, 12, 4 };
	int n = sizeof(v) / sizeof(v[0]);

	bubble_sort(v, n);
	//selection_sort(v, n);
	//insetion_sort(v, n);

	printf("Sirul sortat cu bubble_sort: ");
	for (int i = 0; i < n; i++)
		printf("%d ", v[i]);
	printf("\n");
}

void demo2()
{
	int v[] = { 5, 2, 3, 7, 1, 12, 4 };
	int n = sizeof(v) / sizeof(v[0]);

	//bubble_sort(v, n);
	selection_sort(v, n);
	//insetion_sort(v, n);

	printf("Sirul sortat selection_sort: ");
	for (int i = 0; i < n; i++)
		printf("%d ", v[i]);
	printf("\n");
}

void demo3()
{
	int v[] = { 5, 2, 3, 7, 1, 12, 4 };
	int n = sizeof(v) / sizeof(v[0]);

	//bubble_sort(v, n);
	//selection_sort(v, n);
	insetion_sort(v, n);

	printf("Sirul sortat insetion_sort: ");
	for (int i = 0; i < n; i++)
		printf("%d ", v[i]);
	printf("\n");
}


int main()
{
	demo1();
	demo2();
	demo3();
	/*
	average_case();
	best_case();
	worst_case();
	*/
	//perf_all ();

	return 0;
}