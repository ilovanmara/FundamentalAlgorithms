/*
Nume: Ilovan Mara
Grupa: 2
Tema 7: Satistici dinamice de ordine

Pentru aceasta tema am implementata algoritmul de construire a unui arbore binar echilibrat ce are un size in structura,
algoritmul ce selecteaza elementul al k-lea cel mai mic (OS_select), algoritmul de stergere a unui nod din arbore.
Size-ul se calculeaza dupa formula: root.size = root.left.size + root.right.size + 1
Am adaugat un pointer la parintele nodului in structura arborelui pentru a reface size-urile parintilor in cazul stergeri unui nod.
*/
#include <stdio.h>
#include "Profiler.h"

Profiler p("Statistici dinamice de ordine");

typedef struct _node {
	int key;
	struct _node* left;
	struct _node* right;
	int size;
	struct _node* parent;
}NodeT;

NodeT* createNode(int key)
{
	NodeT* p = (NodeT*)malloc(sizeof(NodeT));
	p->key = key;
	p->left = NULL;
	p->right = NULL;
	p->size = 0;
	p->parent = NULL;
	return p;
}

NodeT* build_tree(int a, int b) {

	if (a <= b) {	
		//NodeT* root = (NodeT*)malloc(sizeof(NodeT));
		int key = (b + a + 1) / 2  ;
		//NodeT* p = createNode(key);
		NodeT* root = createNode(key);
		root->size = 1;
		root->left = build_tree(a, root->key - 1);
		root->right = build_tree(root->key + 1, b);
		
		if (root->left != NULL) {
			root->left->parent = root;
			root->size += root->left->size;
		} if (root->right != NULL) {
			root->right->parent = root;
			root->size += root->right->size;
		} 
	
		return root;
	}
	else return NULL;
}

NodeT* OS_select(NodeT* x, int k, int n) {

	Operation op = p.createOperation("Select", n);

	int rang;
	op.count();
	if (x->left == NULL)
		rang = 1;
	else rang = x->left->size + 1;
	op.count();
	if (k == rang)
		return x;
	else {
		op.count();
		if (k < rang)
			return OS_select(x->left, k, n);
		else return OS_select(x->right, k - rang, n);
	}
}

void inorder(NodeT* p)
{
	if (p != NULL) {
		inorder(p->left);
		printf("%d ", p->key);
		inorder(p->right);
	}
}

void preorder(NodeT* p) {
	if (p != NULL) {
		printf("%d ", p->key);
		preorder(p->left);
		preorder(p->right);
	}
}

void preorder_parents(NodeT* p) {
	if (p != NULL) {
		printf("Nod %d ", p->key);
		if (p->parent != NULL)
			printf("parent: %d\n", p->parent->key);
		else printf("radacina\n");
		preorder_parents(p->left);
		preorder_parents(p->right);
	}
}

void preorder_verif_size(NodeT* p) {
	if (p != NULL) {
		printf("Nod: %d - size: %d\n", p->key, p->size);
		preorder_verif_size(p->left);
		preorder_verif_size(p->right);
	}
}

NodeT* findMin(NodeT* root)
{
	if (root == NULL)
		return NULL;
	if (root->left == NULL)
		return root;
	return findMin(root->left);
}

NodeT* findMax(NodeT* root)
{
	if (root == NULL)
		return NULL;
	if (root->right == NULL)
		return root;
	return findMax(root->right);
}


NodeT* OS_Delete(NodeT* root, int key, int n) {

	Operation op_d = p.createOperation("Delete", n);

	NodeT* p;
	op_d.count();
	if (root == NULL)
		return root;
	op_d.count();
	if (key < root->key) {
		op_d.count();
		root->left = OS_Delete(root->left, key, n);
		op_d.count();
		if (root->left != NULL) {
			root->left->parent = root;
			//root->size--;
		}
	}
	else {
		op_d.count();
		if (key > root->key) {
			op_d.count();
			root->right = OS_Delete(root->right, key, n);
			op_d.count();
			if (root->right != NULL) {
				root->right->parent = root;
				//root->size--;
			}
		}
		else {
			//root->parent->size--;
			op_d.count();
			if (root->left == NULL && root->right == NULL) {
				return NULL;
			}
			else {
				op_d.count();
				if (root->left == NULL) {
					op_d.count();
					p = root->right;
					free(root);
					return p;
				}
				else {
					op_d.count();
					if (root->right == NULL) {
						op_d.count();
						p = root->left;
						free(root);
						return p;
					}
				}
			}
			//root->size--;
			op_d.count(3);
			p = findMin(root->right);
			root->key = p->key;
			root->right = OS_Delete(root->right, p->key, n);
		}
	}
	return root;
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

void demo() {
	int n = 11;
	
	NodeT* root = build_tree(1, n);
	preorder(root);
	printf("\n%d", root->size);

	int k = 6;
	NodeT* x = OS_select(root, k, n);
	printf("\nElementul selectat: %d\n", x->key);
	preorder_verif_size(root);
	printf("\n");
	pretty_print(root, 0);
	
	OS_Delete(root, x->key, n);
	preorder(root);
	printf("\n");
	pretty_print(root, 0);
	preorder_verif_size(root);

	OS_Delete(root, 10, n);
	preorder(root);
	printf("\n");
	pretty_print(root, 0);
	preorder_verif_size(root);

	OS_Delete(root, 2, n);
	preorder(root);
	printf("\n");
	pretty_print(root, 0);
	preorder_verif_size(root);
	//verific parintii
	//printf("\n");
	//preorder_parents(root);
	//preorder_verif_size(root);
}

void average_case() {
	int n;
	
	for (n = 100; n < 10000; n += 100) {
		for (int i = 0; i < 5; i++) {
			NodeT* root = build_tree(1, n);
			NodeT* root_aux = build_tree(1, n);
			int* aux = (int*)malloc(n * sizeof(int));
			FillRandomArray(aux, n, 1, n, true, 0);
			for (int j = 0; j < n; j++) {
				NodeT* x = OS_select(root_aux, aux[i], n);
				root = OS_Delete(root, x->key, n);
			}
		}
	}
	
	p.divideValues("Select", 5);
	p.divideValues("Delete", 5);

	//p.createGroup("Evaluarea operatiilor", "Select", "Delete");

	p.showReport();
}

int main() {
	demo();
	//average_case();
	return 0;
}