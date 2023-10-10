/*
Tema nr 8
Nume: Ilovan Mara
Grupa: 2

La aceasta tema am avut de implementat cele trei tipuri de parcurgeri ale unui arbore binar de cautare iterativ si recursiv,
dar si o hibridizare a quicksort-ului.
Din primul set de grafice (pentru parcurgeri) observam faptul ca metodele iterative si recursive sunt la fel de eficiente.
Dupa masurarea timpului pentru quicksort si pentru varianta hibridizata a acestuia observam faptul ca quicksort-ul hibridizat este
mai rapid, iar in ceea ce priveste numarul de operatii efectuate de acestea, diferenta nu este foarte mare, dar varianta hibridizata
si in acest caz este mai eficienta.
Pentru aflarea pragului optim din algoritmul de quicksort hibridizat am variat acest prag in intervalul [0, 50] pentru a vedea unde
se executa cele mai putine operatii. Astfel am obtinut un grafic sub forma unei parabole cu minimul intre 10 si 12.
*/

#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

Profiler pr("Parcurgerea iterativa vs parcurgerea recursiva");

typedef struct _node {
	int key;
	struct _node* left;
	struct _node* right;
}NodeT;

typedef struct _node2 {
	NodeT* n;
	struct _node2* next;
}NodeL;

NodeT* createNode(int key) {
	NodeT* p = (NodeT*)malloc(sizeof(NodeT));
	p->key = key;
	p->left = NULL;
	p->right = NULL;
	return p;
}

NodeT* build_tree(int a, int b, int* array) {
	
	if (a <= b) {
		int key = array[(b + a ) / 2];
		NodeT* root = createNode(key);
		root->left = build_tree(a, (b + a) / 2 - 1, array);
		root->right = build_tree((b + a) / 2 + 1, b, array);
		return root;
	}
	else return NULL;
}

void push(NodeL** stack, NodeT* nod) {
	NodeL* p = (NodeL*)malloc(sizeof(NodeL));
	p->n = nod;
	p->next = NULL;
	if (*stack == NULL)
		*stack = p;
	else {
		p->next = *stack;
		*stack = p;
	}
}

NodeT* pop(NodeL** stack)
{
	NodeT* nod = NULL;
	NodeL* p;
	if (*stack == NULL)
		return NULL;
	if (*stack != NULL) {
		p = *stack;
		nod = p->n;
		*stack = p->next;
		//nod->key = p->key;
		free(p);
	}
	return nod;
}

NodeT* top(NodeL** stack)
{
	NodeT* nod = NULL;
	NodeL* p;
	if (*stack == NULL)
		return NULL;
	if (*stack != NULL) {
		nod = (* stack)->n;
	}
	return nod;
}

void pretty_print(NodeT* root, int space) {
	if (root == NULL)
		return;
	space = space + 5;
	pretty_print(root->right, space);
	//printf("\n");
	for (int i = 5; i < space; i++)
		printf(" ");
	printf("%d\n", root->key);
	pretty_print(root->left, space);
}

void inorder(NodeT* p, int n){

	Operation op = pr.createOperation("inorder_rec", n);

	if (p != NULL) {
		inorder(p->left, n);
		op.count();
		printf("%d ", p->key);
		inorder(p->right, n);
	}
}

void preorder(NodeT* p, int n) {

	Operation op = pr.createOperation("preorder_rec", n);

	if (p != NULL) {
		printf("%d ", p->key);
		op.count();
		preorder(p->left, n);
		preorder(p->right, n);
	}
}

void postorder(NodeT* p, int n) {

	Operation op = pr.createOperation("postorder_rec", n);

	if (p != NULL) {
		postorder(p->left, n);
		postorder(p->right, n);
		printf("%d ", p->key);
		op.count();
	}
}

void stivaVida(NodeL** stack) {
	*stack = NULL;
}

void inorder_it(NodeT* root, int n) {

	Operation op = pr.createOperation("inorder_it", n);

	NodeT* current = root;
	NodeL* stack = NULL;
	int ok = 0;
	
	while(!ok) {
		if (current != NULL){
			push(&stack, current);
			current = current->left;
		}
		else{
			if ((stack) != NULL) {
				current = pop(&stack);
				printf("%d ", current->key);
				op.count();
				current = current->right;
			}
			else ok = 1;
		}
	} 
}

void preorder_it(NodeT* root, int n) {

	Operation op = pr.createOperation("preorder_it", n);

	if (root == NULL)
		return;
	NodeL* stack = NULL;
	NodeT* current = root;

	while (stack != NULL || current != NULL) {
		if (current != NULL) {
			printf("%d ", current->key);
			op.count();
			if(current->right != NULL)
				push(&stack, current->right);
			current = current->left;
		}
		else {
			current = pop(&stack);
		}
	}
}

void postorder_it(NodeT* root, int n) {

	Operation op = pr.createOperation("postorder_it", n);

	NodeL* stack = NULL;
	NodeL * right_child_stack = NULL;
	NodeT* current = root;

	while (stack != NULL || current != NULL) {
		if (current != NULL) {
			if (current->right != NULL)
				push(&right_child_stack, current->right);
			push(&stack, current);
			current = current->left;
		}
		else {
			current = top(&stack);
			if (right_child_stack != NULL && current->right == top(&right_child_stack))
				current = pop(&right_child_stack);
			else {
				printf("%d ", current->key);
				op.count();
				pop(&stack);
				current = NULL;
			}
		}
	}
}

void insetion_sort(int v[], int n, Operation* op) {
	
	for (int i = 1; i < n; i++) {
		op->count();
		int sel = v[i];
		int j = i - 1;
		while (j > -1 && v[j] > sel) {
			op->count();
			op->count();
			v[j + 1] = v[j];
			j--;
		}
		op->count();
		op->count();
		v[j + 1] = sel;
	}
}

int partition(int a[], int left, int right, int n, Operation* op) {

	op->count();
	int x = a[right];
	int i = left - 1;
	int aux;
	for (int j = left; j < right; j++) {
		op->count();
		if (a[j] < x) {
			i++;
			aux = a[i];
			a[i] = a[j];
			a[j] = aux;
			op->count(3);
		}
	}
	aux = a[i + 1];
	a[i + 1] = a[right];
	a[right] = aux;
	op->count(3);

	return i + 1;
}

void QuickSort(int a[], int left, int right, int n) {
	Operation op = pr.createOperation("QuickSort_op", n);
	if (left < right) {
		int q = partition(a, left, right, n, &op);
		QuickSort(a, left, q - 1, n);
		QuickSort(a, q + 1, right, n);
	}
}

void QuickSort_hibridizat(int a[], int left, int right, int n, int opt, Operation* op1) {

	if (left < right) {
		if (right - left + 1 < opt) {
			insetion_sort(a + left, right - left + 1, op1);
			//QuickSort_hibridizat()
		}
		else {
			int q = partition(a, left, right, n, op1);
			QuickSort_hibridizat(a, left, q - 1, n, opt, op1);
			QuickSort_hibridizat(a, q + 1, right, n, opt, op1);
		}
	}
}

void demo() {
	int n = 7;
	int array[7] = { 1, 2, 3, 4, 5, 6, 7 };
	NodeT* root = build_tree(0, 6, array);

	pretty_print(root, 0);
	printf("\n");

	printf("Parcurgerile recursive ale arborelui sunt:\n");
	printf("1. Parcurferea in inordine: ");
	inorder(root, n);
	printf("\n2. Parcurgerea in preordine: ");
	preorder(root, n);
	printf("\n3. Parcurgerea in postordine: ");
	postorder(root, n);

	printf("\nParcurgerile iterative ale arborelui sunt:\n");
	printf("1. Parcurferea in inordine: ");
	inorder_it(root, n);
	printf("\n2. Parcurferea in preordine: ");
	preorder_it(root, n);
	printf("\n3. Parcurferea in postordine: ");
	postorder_it(root, n);
}

void it_vs_rec() {
	for (int n = 100; n <= 10000; n += 100) {
		int* array = (int*)malloc(n * sizeof(int));
		FillRandomArray(array, n, 1, n, true, 1);
		NodeT* root = build_tree(0, n - 1, array);

		preorder(root, n);
		printf("\n");
		preorder_it(root, n);
		printf("\n");

		inorder(root, n);
		inorder_it(root, n);

		postorder(root, n);
		postorder_it(root, n);
	}
	pr.createGroup("preorder_ItVsRec", "preorder_rec", "preorder_it");
	pr.createGroup("inorder_ItVsRec", "inorder_rec", "inorder_it");
	pr.createGroup("postorder_ItVsRec", "postorder_rec", "postorder_it");

	pr.showReport();
}

void print_array(int* array, int n) {
	for (int i = 0; i < n; i++)
		printf("%d ", array[i]);
	printf("\n");
}

void demo_hibridizare_quicksort() {
	int n = 200;
	Operation op1 = pr.createOperation("QuickSort_hibridizat_op", n);
	int* array = (int*)malloc(n * sizeof(int));
	FillRandomArray(array, n, 1, n, true, 0);
	//int array[7] = { 8, 2, 1, 4, 9, 6, 7 };
	print_array(array, n);
	QuickSort_hibridizat(array, 0, n-1, n, 10, &op1);
	print_array(array, n);
}

void save(int* array, int* aux, int n) {
	for (int i = 0; i < n; i++) {
		aux[i] = array[i];
	}
}

void QS_timer() {
	int v[10000];
	int aux[10000];

	for (int n = 100; n <= 10000; n += 100) {
		pr.startTimer("QuickSort", n);
		for (int i = 0; i < 100; i++) {
			FillRandomArray(v, n, 1, n, true, 0);
			QuickSort(v, 0, n - 1, n);
		}
		pr.stopTimer("QuickSort", n);
	}
	//pr.showReport();
}

void QS_hibridizat_timer() {
	int v[10000];
	int aux[10000];

	for (int n = 100; n <= 10000; n += 100) {
		Operation op1 = pr.createOperation("QuickSort_hibridizat_op", n);
		pr.startTimer("QuickSort_hibridizat", n);
		for (int i = 0; i < 100; i++) {
			FillRandomArray(v, n, 1, n, true, 0);
			QuickSort_hibridizat(v, 0, n - 1, n, 10, &op1);
		}
		pr.stopTimer("QuickSort_hibridizat", n);
	}
	//pr.showReport();
}

void QS_hibridizat_vs_QS() {
	QS_timer();
	QS_hibridizat_timer();
	pr.createGroup("QS_hibridizat_vs_QS", "QuickSort", "QuickSort_hibridizat");
	//pr.showReport();
}

void QS_hibridizat_vs_QS_op() {
	int v[10000];
	int aux[10000];

	for (int n = 100; n < 10000; n += 100) {
		Operation op1 = pr.createOperation("QuickSort_hibridizat_op", n);
		for (int i = 0; i < 5; i++) {
			FillRandomArray(v, n, 1, n, true, 0);
			save(v, aux, n);
			QuickSort(v, 0, n - 1, n);
			QuickSort_hibridizat(aux, 0, n - 1, n, 10, &op1);
		}
	}
	pr.createGroup("QS_hibridizat_vs_QS_op", "QuickSort_op", "QuickSort_hibridizat_op");
	pr.showReport();
}

void prag_optim() {
	int v[10000];
	int aux[10000];
	int n = 10000;

	FillRandomArray(v, n, 1, n, true, 0);
	for (int opt = 0; opt <= 50; opt += 2) {
		Operation op1 = pr.createOperation("QuickSort_hibridizat_op", opt);
		save(v, aux, n);
		QuickSort_hibridizat(aux, 0, n - 1, n, opt, &op1);
	}
}

void prag_optim_timer() {
	int v[10000];
	int aux[10000];
	int n = 10000;

	FillRandomArray(v, n, 1, n, true, 0);
	for (int opt = 0; opt <= 50; opt += 2) {
		Operation op1 = pr.createOperation("QuickSort_hibridizat_op", opt);
		pr.startTimer("QuickSort_hibridizat", opt);
		for (int i = 0; i < 100; i++) {
		save(v, aux, n);
		FillRandomArray(v, n, 1, n, true, 0);
		QuickSort_hibridizat(aux, 0, n - 1, n, opt, &op1);
		}
		pr.stopTimer("QuickSort_hibridizat", opt);
	}
}

void perf() {
	prag_optim_timer();
	prag_optim();
	pr.showReport();
}

int main() {
	//demo();
	//it_vs_rec();
	//demo_hibridizare_quicksort();
	//QS_hibridizat_vs_QS();
	//QS_hibridizat_vs_QS_op();
	//prag_optim();
	perf();
	return 0;
}