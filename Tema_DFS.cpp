/*
Tema nr 11: DFS
Nume: Ilovan Mara
Grupa: 2
Complexitate DFS: O(V+E) ->similar cu bfs, dar inlocuim coada cu stiva, in loc sa punem in coada fiecare vecin ai unui nod 
facem un apel recursiv pentru fiecare, d reprezinta timpul de descoperire, iar f timpul de finalizare.
Complexitatea Topological Sort: O(V+E) : pentru realizarea acestui algoritm trebuie verificam ca graful nu are ciculri
Algoritmul lui Tarjan este bazat pe DFS si gasete componentele tare conexe din graf.
*/
#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"
#include <stack>
#include <time.h>  

Profiler p("DFS");
 
using namespace std;

enum{
WHITE = 0,
GRAY,
BLACK
};

int index_glbal = 0;


typedef struct _Node {
    int key;
    int adjSize;
    struct _Node** adj;

    int index;
    int lowLink;
    int comp;
    int onStack;

    int color;
    int d;
    int f;
    struct _Node* parent;
}Node;

typedef struct {
    int nrNodes;
    Node** v;
}Graph;

//int time;

void DFS_VISIT(Graph* g, Node* u, int* time, stack<Node*>* s, int* array, Operation* op) {
    (*time)++;
    u->d = *time;
    u->color = GRAY;
    op->count(2);
    printf("%d ", u->key);
    for (int i = 0; i < u->adjSize; i++) {
        if (u->adj[i]->color == GRAY) {
            array[i] = 1;
        }
        op->count();
        if (u->adj[i]->color == WHITE) {
            u->adj[i]->parent = u;
            op->count();
            DFS_VISIT(g, u->adj[i], time, s, array, op);
        }
    }
    u->color = BLACK;
    (*time)++;
    u->f = *time;
    (* s).push(u);
    op->count(3);
}

void DFS(Graph* g, int* time, stack<Node*>* s, int* array, Operation* op) {
    //stack<Node*> s;
    for (int i = 0; i < g->nrNodes; i++) {
        g->v[i]->color = WHITE;
        g->v[i]->parent = NULL;
        op->count(2);
    }
    *time = 0;
    for (int i = 0; i < g->nrNodes; i++) {
        op->count();
        if (g->v[i]->color == WHITE) {
            DFS_VISIT(g, g->v[i], time, s, array, op);
        }
    }
}

void topological_sort(Graph* g, int* time, Operation* op) {
    stack<Node*> s;
    int* array = (int*)calloc(g->nrNodes , sizeof(int));
    DFS(g, time, &s, array, op);
    printf("\n");
    for (int i = 0; i < g->nrNodes; i++) {
        if (!array[i])
            return;
    }
    while (!s.empty()) {
        Node* aux = s.top();
        printf("%d ", aux->key);
        s.pop();
    }

}

int minim(int a, int b) {
    if (a < b)
        return a;
    return b;
}

void strong_connect(Graph* g, Node* u, int* index, stack<Node*>* s, int* nrComponents){
    u->index = *index;
    u->lowLink = *index;
    *index = *index + 1;
    (* s).push(u);
    u->onStack = 1;

    for (int i = 0; i < u->adjSize; i++) {
        if (u->adj[i]->index == -1) {
            strong_connect(g, u->adj[i], index, s, nrComponents);
            u->lowLink = minim(u->lowLink, u->adj[i]->lowLink);
        }
        else if (u->adj[i]->onStack) {
            u->lowLink = minim(u->lowLink, u->adj[i]->index);
        }
    }
    if (u->lowLink == u->index) {
        printf("strongly connected components: ");
        nrComponents = nrComponents + 1;
        Node* aux;
        do {
            aux = s->top();
            printf("%d ", aux->key);
            aux->onStack = 0;
            aux->comp = *nrComponents;
            s->pop();
        } while (aux->key != u->key);
        printf("\n");
    }

}

void tarjan(Graph* g, int* index, stack<Node*>* s, int* nrComponents) {
    for (int i = 0; i < g->nrNodes; i++) {
        g->v[i]->index = -1;
        g->v[i]->lowLink = -1;
        g->v[i]->onStack = 0;
        g->v[i]->comp = 0;
    }
    for (int i = 0; i < g->nrNodes; i++) {
        if (g->v[i]->index == -1) {
            strong_connect(g, g->v[i], index, s, nrComponents);
        }
    }
}

Graph* create_graph(int nrNodes) {
    Graph* g = (Graph*)malloc(sizeof(Graph));
    g->nrNodes = nrNodes;
    g->v = (Node**)malloc(nrNodes * sizeof(Node*));
    for (int i = 0; i < nrNodes; i++) {
        g->v[i] = (Node*)malloc(sizeof(Node));
        g->v[i]->key = i;
        g->v[i]->adjSize = 0;
        g->v[i]->adj = (Node**)malloc(nrNodes * sizeof(Node*));
    }
    return g;
}

void add_edge(Graph* g, int x, int y) {
    g->v[x]->adj[g->v[x]->adjSize] = (Node*)malloc(sizeof(Node));
    g->v[x]->adj[g->v[x]->adjSize++] = g->v[y];
}

void printGraph(Graph* g) {
    for (int i = 0; i < g->nrNodes; i++) {
        printf("%d: ", g->v[i]->key);
        for (int j = 0; j < g->v[i]->adjSize; j++)
            printf("%d ", g->v[i]->adj[j]->key);
        printf("\n");
    }
}

void demo_DFS() {
    Operation op = p.createOperation("op", 5);
    int time = 0;
    stack<Node*> s;
    Graph* g = create_graph(8);
    int* array = (int*)malloc(g->nrNodes * sizeof(int));
    add_edge(g, 1, 0);
    add_edge(g, 0, 2);
    add_edge(g, 2, 3);
    add_edge(g, 2, 1);
    add_edge(g, 1, 3);
    add_edge(g, 2, 4);
    add_edge(g, 3, 5);
    add_edge(g, 5, 3);
    add_edge(g, 4, 5);
    add_edge(g, 6, 4);
    add_edge(g, 6, 7);
    add_edge(g, 5, 7);
    add_edge(g, 4, 6);
    printGraph(g);
    DFS(g, &time, &s, array, &op);
}

void demo_topologicalSort() {
    Operation op = p.createOperation("op", 5);
    int time = 0;
    stack<Node*> s;
    Graph* g = create_graph(5);
    add_edge(g, 0, 3);
    add_edge(g, 0, 1);
    add_edge(g, 2, 4);
    add_edge(g, 1, 2);
    add_edge(g, 0, 2);
    topological_sort(g, &time, &op);
}

void demo_tarjan() {
    int index = 0;
    stack<Node*> s;
    int nrComponents = 0;

    Graph* g = create_graph(7);
    /*
    add_edge(g, 1, 0);
    add_edge(g, 0, 2);
    add_edge(g, 2, 3);
    add_edge(g, 2, 1);
    add_edge(g, 1, 3);
    add_edge(g, 2, 4);
    add_edge(g, 3, 5);
    add_edge(g, 5, 3);
    add_edge(g, 4, 5);
    add_edge(g, 6, 4);
    add_edge(g, 6, 7);
    add_edge(g, 5, 7);
    add_edge(g, 4, 6);
    */
    /*
    add_edge(g, 0, 1);
    add_edge(g, 1, 3);
    add_edge(g, 1, 6);
    add_edge(g, 2, 8);
    add_edge(g, 2, 7);
    add_edge(g, 3, 4);
    add_edge(g, 3, 8);
    add_edge(g, 4, 2);
    add_edge(g, 5, 9);
    add_edge(g, 6, 5);
    add_edge(g, 7, 3);
    add_edge(g, 7, 8);
    add_edge(g, 8, 5);
    add_edge(g, 9, 8);
    */
   
    add_edge(g, 0, 1);
    add_edge(g, 1, 3);
    add_edge(g, 2, 0);
    add_edge(g, 2, 6);
    add_edge(g, 3, 2);
    add_edge(g, 6, 5);
    add_edge(g, 5, 1);
    add_edge(g, 3, 5);
    add_edge(g, 5, 4);
 

    tarjan(g, &index, &s, &nrComponents);
}

void performance()
{
    int n, i;
    int time = 0;
    stack<Node*> s;
    for (n = 1000; n <= 4500; n += 100) {
        Operation op = p.createOperation("dfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        int* array = (int*)calloc(100 , sizeof(int));
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
            graph.v[i]->adjSize = 0;
            graph.v[i]->adj = (Node**)malloc(graph.nrNodes * sizeof(Node*));

        }
     
        for (int i = 0; i < graph.nrNodes - 1; i++) {
            add_edge(&graph, i, i + 1);
        }
        
        int* aux = (int*)malloc((n - 99) * sizeof(int));
        int* aux2 = (int*)malloc((n - 99) * sizeof(int));
        FillRandomArray(aux, n - 99, 1, graph.nrNodes - 1, false, 0);
        FillRandomArray(aux2, n - 99, 1, graph.nrNodes - 1, false, 0);
        //FillRandomArray(aux2, n - 99, graph.nrNodes / 2 + 1, graph.nrNodes - 1, false, 0);

        for (int i = 0; i < n - 99; i++) {
            int ok = 0;
            for (int j = 0; j < graph.v[aux[i]]->adjSize; j++) {
               if (graph.v[aux[i]]->adj[j] == graph.v[aux2[i]]) {
                    //ok = 1;
                    // aux[i] = rand() % (graph.nrNodes / 2) + 1;
                     //i--;
                     //continue;
                    break;
                }
            }
            if (ok == 0) {
                add_edge(&graph, aux[i], aux2[i]);
            }
        }
        
        int k = 0;
       // srand(time(0));
        /*
        while (k < n - 99) {
            int ok = 0;
            int a = (rand() % (graph.nrNodes - 2) + 1);
            int b = (rand() % (graph.nrNodes - 2) + 1);
            if (a == b) {
                int b = rand() % (graph.nrNodes - 1) + 1;
            }

            for (int j = 0; j < graph.v[a]->adjSize; j++) {
                if (graph.v[a]->adj[j] == graph.v[b]) {
                    ok = 1;
                }
                if (ok != 1) {
                    k++;
                    graph.v[a]->adj[graph.v[a]->adjSize] = (Node*)malloc(sizeof(Node));
                    graph.v[a]->adj[graph.v[a]->adjSize++] = graph.v[b];
                }
            }
        }
        */

        DFS(&graph, &time, &s, array, &op);
    }
    time = 0;
    for (n = 100; n <= 200; n += 10) {
        Operation op = p.createOperation("dfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        int* array = (int*)calloc(n , sizeof(int));
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
            graph.v[i]->adjSize = 0;
            graph.v[i]->adj = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        }

        for (int i = 0; i < graph.nrNodes - 1; i++) {
           add_edge(&graph, i, i + 1);
        }

        int* aux = (int*)malloc((4500 - n + 1) * sizeof(int));
        int* aux2 = (int*)malloc((4500 - n + 1) * sizeof(int));
        FillRandomArray(aux, 4500 - n + 1, 1, graph.nrNodes - 1, false, 0);
        FillRandomArray(aux2, 4500 - n + 1, 1, graph.nrNodes - 1, false, 0);
      
        int k = 0;
        for (int i = 0; i < 4500 - n + 1; i++) {
            int ok = 0;
            for (int j = 0; j < graph.v[aux[i]]->adjSize; j++) {
                if (graph.v[aux[i]]->adj[j] == graph.v[aux2[i]]) {
                    ok = 1;
                     break;
                }
            }
            if (!ok) {
                add_edge(&graph, aux[i], aux2[i]);
            }
            else add_edge(&graph, aux2[i], aux[i]);
        }
        DFS(&graph, &time, &s, array, &op);
    }

    p.showReport();
}


int main() {
    printf("DFS:\n");
    demo_DFS();
    printf("\nTopological Sort:\n");
    demo_topologicalSort();
    printf("\nTarjan: \n");
    demo_tarjan();
    //performance();
    return 0;
}