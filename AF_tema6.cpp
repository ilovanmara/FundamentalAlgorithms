/*
Nume: Ilovan Mara Gabbriela
Grupa: 2
Tema nr 6: Arbori Multicai

La aceasta tema am implementat arborii multicai flosind cele 3 reprezentari.
R1: vector de parinti
R2: are o structura ce contine cheia unui nod, un vector de pointeri la nodurile copii si dimensiunea acestui vector
R3: are o structura ce contine cheia, pointer la copilul din stanga(primul) si pointer la fratele din dreapta (next)
Prima data am transformat din reprezentarea cu vectori de parinti in reprezentarea 2,
iar dupa am transformat in reprezentarea 3, binara.
Cele doua transformari au fost facute in O(n)
T1: este O(n) intrucat avem 4 for-uri care fac n pasi, astfel avem o complexitate de 4 * n , 4-constanta => O(n)
T2: este O(n) (Master Theorem)
Am folosit memorie aditionala pentru formarea legaturiolor intre copii si parinti.
Dupa ce am realizat cele doua transformari am printat prietenos arborele pentru fiecare dintre cele 3 reprezentari.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _node2{
	int key;
	int nr_copii;
	struct _node2** children;
}NodeT;

typedef struct _node3 {
	int key;
	struct _node3* first_child;
	struct _node3* right_brother;
}NodeT3;

void R1pretty_print(int* v, int space, int root_ind, int tree_size) {
	for (int i = 0; i < tree_size; i++) {
		if (v[i] == root_ind) {
			for(int j = 0; j < space; j++)
				printf(" ");
			printf("%d\n", i + 1);
			R1pretty_print(v, space + 2, i + 1, tree_size);
		}
	}
}

//functia de alocare si initializare pentru fiecare nod
NodeT* createNode2(int key) {
	NodeT* p = (NodeT*)malloc(sizeof(NodeT));
	p->key = key;
	p->nr_copii = 0;
	//p.children =(NodeT*) malloc(sizeof(NodeT));
	p->children = NULL;
	return p;
}

//transformarea T1
NodeT* r1_to_r2(int* v, int size) {
	NodeT**  tree = (NodeT**)malloc(size  * sizeof(NodeT*));
	NodeT* root = (NodeT*)malloc(sizeof(NodeT));
	NodeT* parent_aux = (NodeT*)malloc(sizeof(NodeT));
	NodeT* child_aux = (NodeT*)malloc(sizeof(NodeT));
	for (int i = 0; i < size; i++) {
		tree[i] = createNode2(i + 1);
	}
	for (int i = 0; i < size; i++) {
		if (v[i] != -1) {
			tree[v[i] - 1]->nr_copii++;
		}
		else root = tree[i];		
	}
	for (int i = 0; i < size; i++) {
		if (tree[i]->nr_copii > 0) {
			tree[i]->children =(NodeT**) malloc(tree[i]->nr_copii * sizeof(NodeT*));
			tree[i]->nr_copii = 0;
		}
	}
	for (int i = 0; i < size; i++) {
		if (v[i] != -1) {
			parent_aux = tree[v[i] - 1];
			child_aux = tree[i];
			parent_aux->children[parent_aux->nr_copii] = child_aux;
			parent_aux->nr_copii++;
		}
	}
	/*
	printf("\nAfisare vect noduri r2:\n");
	for (int i = 0; i < size; i++) {
		if (tree[i]->nr_copii != 0) {
			printf("Nodul %d are copiii: ", tree[i]->key);
			for (int j = 0; j < tree[i]->nr_copii; j++) {
				printf("%d ", tree[i]->children[j]->key);
			}
			printf("\n");
		}
		else printf("Nodul %d e frunza\n", tree[i]->key);
	}
	*/
	return root;
}

void R2pretty_print(NodeT* root, int space) {
	if (root != NULL) {
		for (int i = 0; i < space; i++) {
			printf(" ");
		}
		printf("%d\n", root->key);
		for (int i = 0; i < root->nr_copii; i++) {
			R2pretty_print(root->children[i], space + 2);
		}
	}
	else return;
}

void print_verifR2(NodeT* root) {
	printf("%d\n", root->key);
	for (int i = 0; i < root->nr_copii; i++) {
		print_verifR2(root->children[i]);
	}
}

//aloc + initializez un nod
NodeT3* createNode3(int key) {
	NodeT3* root = (NodeT3*)malloc(sizeof(NodeT3));
	root->key = key;
	root->first_child = NULL;
	root->right_brother = NULL;
	return root;
}

//transformarea T2
NodeT3* r2_to_r3(NodeT* root2, NodeT3* left_brother) {
	NodeT3* root = createNode3(root2->key);
	if (left_brother != NULL) {
		left_brother->right_brother = root;
	}
	NodeT3* c = (NodeT3*)malloc(sizeof(NodeT3));
	for (int i = 0; i < root2->nr_copii; i++) {
		c = r2_to_r3(root2->children[i], c);
		if (i == 0)
			root->first_child = c;
	}
	return root;
}

void print_verifR3(NodeT3* root) {
	printf("%d ", root->key);
	while (root->first_child != NULL) {
		//printf("Copii");
		print_verifR3(root->first_child);
		root->first_child = root->first_child->right_brother;
	}
}

void R3pretty_print(NodeT3* root, int space) {
	if (root != NULL) {
		for (int i = 0; i < space; i++) {
			printf(" ");
		}
		printf("%d\n", root->key);
		while (root->first_child != NULL) {
			R3pretty_print(root->first_child, space + 2);
			root->first_child = root->first_child->right_brother;
		}
	}
	else return;
}

void demo_r1() {
	int v[9] = { 2, 7, 5, 2, 7, 7, -1, 5, 2 };
	printf("Pretty print reprezentarea R1:\n");
	R1pretty_print(v, 0, -1, 9);
}

void demo_r1_to_r2_to_r3() {
	int v[9] = { 2, 7, 5, 2, 7, 7, -1, 5, 2 };
	int size = sizeof(v) / sizeof(int);
	//NodeT**  tree = (NodeT**)malloc(size  * sizeof(NodeT*));
	
	NodeT* root = r1_to_r2(v, size);
	//r1_to_r2(v, size);
	//printf("Radacina arborelui este: %d\n", root->key);
	//for (int i = 0; i < root->nr_copii; i++) {
		//printf("%d ", root->children[i]->key);
	//}
	//print_verifR2(root);
	
	printf("Pretty print reprezentare R2:\n");
	R2pretty_print(root, 0);
	
	NodeT3* root3 = r2_to_r3(root, NULL);
	
	//printf("\nRadacina arborelui este: %d ", root3->key);

	//print_verifR3(root3);
	printf("\nPretty print reprezentare R3:\n");
	R3pretty_print(root3, 0);
	
}

int main() {
	demo_r1();
	demo_r1_to_r2_to_r3();
	return 0;
}