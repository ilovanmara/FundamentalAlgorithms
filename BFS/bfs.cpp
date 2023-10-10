/*
Tema nr 10: BFS
Nume: Ilovan Mara
Grupa: 2
Bfs are complexitatea O(V + E)
In aceasta tema graful este reprezentat sub forma de lista de adiacenta
Pentru realizarea parcurgerii in latime avem nevoie de 3 culori, distanta fata de nodul sursa si parintele. 
Se baga nodul de start in coada si se macheaza cu gri,iar cat timp mai avem elemente in coada extragem din coada
coloram vecinii gri si ii bagam in coada, dupa care nodul se marcheaza cu negru.

*/

#include <stdlib.h>
#include <string.h>
#include "bfs.h"
#include <queue>
#include <time.h>  

using namespace std;

int get_neighbors(const Grid* grid, Point p, Point neighb[])
{
    // TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
    // the point p will have at most 4 neighbors (up, down, left, right)
    // avoid the neighbors that are outside the grid limits or fall into a wall
    // note: the size of the array neighb is guaranteed to be at least 4
 
    int k = 0;
    if (grid->mat[p.row][p.col] == 0) {
        if (p.row > 0 && grid->mat[p.row - 1][p.col] == 0) {
            neighb[k].row = p.row - 1;
            neighb[k].col = p.col;
            k++;
        }
        if (p.col > 0 && grid->mat[p.row][p.col - 1] == 0) {
            neighb[k].row = p.row;
            neighb[k].col = p.col - 1;
            k++;
        }
        if (p.row < grid->rows - 1 && grid->mat[p.row + 1][p.col] == 0) {
            neighb[k].row = p.row + 1;
            neighb[k].col = p.col;
            k++;
        }
        if (p.col < grid->cols - 1 && grid->mat[p.row][p.col + 1] == 0) {
            neighb[k].row = p.row;
            neighb[k].col = p.col + 1;
            k++;
        }
    }
    return k;
}

void grid_to_graph(const Grid* grid, Graph* graph)
{
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node* nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[4];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for (i = 0; i < grid->rows; ++i) {
        for (j = 0; j < grid->cols; ++j) {
            if (grid->mat[i][j] == 0) {
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            }
            else {
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for (i = 0; i < grid->rows; ++i) {
        for (j = 0; j < grid->cols; ++j) {
            if (nodes[i][j] != NULL) {
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for (i = 0; i < graph->nrNodes; ++i) {
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if (graph->v[i]->adjSize != 0) {
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for (j = 0; j < graph->v[i]->adjSize; ++j) {
                if (neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0) {
                    graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if (k < graph->v[i]->adjSize) {
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}


void free_graph(Graph* graph)
{
    if (graph->v != NULL) {
        for (int i = 0; i < graph->nrNodes; ++i) {
            if (graph->v[i] != NULL) {
                if (graph->v[i]->adj != NULL) {
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

void bfs(Graph* graph, Node* s, Operation* op)
{
    // TOOD: implement the BFS algorithm on the graph, starting from the node s
    // at the end of the algorithm, every node reachable from s should have the color BLACK
    // for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
    // for counting the number of operations, the optional op parameter is received
    // since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
    // if(op != NULL) op->count();
    for (int i = 0; i < graph->nrNodes, graph->v[i] != s; i++) {
        if (op != NULL)
            op->count();
        graph->v[i]->color = COLOR_WHITE;
        graph->v[i]->dist = INT32_MAX;
        graph->v[i]->parent = NULL;
        if (op != NULL)
            op->count(3);

    }

    s->color = COLOR_GRAY;
    s->dist = 0;
    s->parent = NULL;
    if (op != NULL)
        op->count(3);

    queue<Node*> Q;
    Q.push(s);

    int i = 0;
    while (!Q.empty()) {
        Node* u = Q.front();
        Q.pop();
        if (op != NULL)
            op->count();

        for (int j = 0; j < u->adjSize; j++) {
            if (op != NULL)
                op->count();
            if (u->adj[j]->color == COLOR_WHITE) {
                u->adj[j]->color = COLOR_GRAY;
                u->adj[j]->dist = u->dist + 1;
                u->adj[j]->parent = u;
               // printf("%d ", i);
                Q.push(u->adj[j]);
                if (op != NULL) {
                    op->count(4);
                }
            }
        }
        if (op != NULL)
            op->count();
        u->color = COLOR_BLACK;
       // i++;
    }
}

void pretty_print(int* v, int space, int root_ind, int tree_size, Point* repr) {
  
    for (int i = 0; i < tree_size; i++) {
        if (v[i] == root_ind) {
            for (int j = 0; j < space; j++)
                printf(" ");
            printf("(%d, %d)\n", repr[i].row, repr[i].col);
            pretty_print(v, space + 6, i, tree_size, repr);
        }
    }
}

void print_bfs_tree(Graph* graph)
{
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int* p = NULL; //the parent array
    Point* repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int* transf = (int*)malloc(graph->nrNodes * sizeof(int));
    for (int i = 0; i < graph->nrNodes; ++i) {
        if (graph->v[i]->color == COLOR_BLACK) {
            transf[i] = n;
            ++n;
        }
        else {
            transf[i] = -1;
        }
    }
    if (n == 0) {
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Node));
    for (int i = 0; i < graph->nrNodes && !err; ++i) {
        if (graph->v[i]->color == COLOR_BLACK) {
            if (transf[i] < 0 || transf[i] >= n) {
                err = 1;
            }
            else {
                repr[transf[i]] = graph->v[i]->position;
                if (graph->v[i]->parent == NULL) {
                    p[transf[i]] = -1;
                }
                else {
                    err = 1;
                    for (int j = 0; j < graph->nrNodes; ++j) {
                        if (graph->v[i]->parent == graph->v[j]) {
                            if (transf[j] >= 0 && transf[j] < n) {
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if (!err) {
        // TODO: pretty print the BFS tree
        // the parrent array is p (p[k] is the parent for node k or -1 if k is the root)
        // when printing the node k, print repr[k] (it contains the row and column for that point)
        // you can adapt the code for transforming and printing multi-way trees from the previous labs
      
        pretty_print(p, 0, -1, n , repr);
    }

    if (p != NULL) {
        free(p);
        p = NULL;
    }
    if (repr != NULL) {
        free(repr);
        repr = NULL;
    }
}

int shortest_path(Graph* graph, Node* start, Node* end, Node* path[])
{
    // TODO: compute the shortest path between the nodes start and end in the given graph
    // the nodes from the path, should be filled, in order, in the array path
    // the number of nodes filled in the path array should be returned
    // if end is not reachable from start, return -1
    // note: the size of the array path is guaranteed to be at least 1000
    int i = 0;
    bfs(graph, start);

    Node** aux = (Node**)malloc(1000 * sizeof(Node*));
    for (int j = 0; j < 1000; j++) {
        aux[j] = (Node*)malloc(sizeof(Node));
    }
    while (end != start) {
        aux[i++] = end;
        end = end->parent;
    }
    for (int j = 0; j < i; j++) {
        path[j] = aux[i - j - 1];
    }
    return i;
}


void performance()
{
    int n, i;
    Profiler p("bfs");

    // vary the number of edges

    for (n = 1000; n <= 3500; n += 100) {
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
            graph.v[i]->adjSize = 0;
            //for(int j = 0; j < graph.nrNodes; j++)
            graph.v[i]->adj = (Node**)malloc(graph.nrNodes * sizeof(Node*));

        }
        // TODO: generate n random edges
        // make sure the generated graph is connected

        //am 100 de noduri, leg toate de nodul 0  = > n - 99 random
        for (int i = 1; i < graph.nrNodes - 1; i++) {
            graph.v[0]->adj[graph.v[0]->adjSize] = (Node*)malloc(sizeof(Node));
            graph.v[i]->adj[graph.v[0]->adjSize] = (Node*)malloc(sizeof(Node));
            graph.v[0]->adj[graph.v[0]->adjSize++] = graph.v[i];
            graph.v[i]->adj[graph.v[i]->adjSize++] = graph.v[0];
        }

        int* aux = (int*)malloc((n - 99) * sizeof(int));
        int* aux2 = (int*)malloc((n - 99) * sizeof(int));
        FillRandomArray(aux, n - 99, 1, graph.nrNodes / 2, false, 0);
        FillRandomArray(aux2, n - 99, graph.nrNodes / 2 + 1, graph.nrNodes - 1, false, 0);
       
        //srand(time(0));
        for (int i = 0; i < n - 99; i++) {
            int ok = 0;
            for (int j = 0; j < graph.v[aux[i]]->adjSize; j++) {
                if (graph.v[aux[i]]->adj[j] == graph.v[aux2[i]]) {
                    ok = 1;
                    //aux[i] = rand() % (graph.nrNodes / 2) + 1;
                   // i--;
                   // continue;
                }
            }
            if (ok == 0) {
                graph.v[aux[i]]->adj[graph.v[aux[i]]->adjSize] = (Node*)malloc(sizeof(Node));
                graph.v[aux2[i]]->adj[graph.v[aux2[i]]->adjSize] = (Node*)malloc(sizeof(Node));
                graph.v[aux[i]]->adj[graph.v[aux[i]]->adjSize++] = graph.v[aux2[i]];
                graph.v[aux2[i]]->adj[graph.v[aux2[i]]->adjSize++] = graph.v[aux[i]];
            }
        }
        /*
        int k = 0;
        srand(time(0));

        while (k < n - 99) {
            int ok = 0;
            int a = (rand() % (graph.nrNodes - 2));
            int b = (rand() % (graph.nrNodes - 2));
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
                    graph.v[b]->adj[graph.v[b]->adjSize] = (Node*)malloc(sizeof(Node));
                    graph.v[a]->adj[graph.v[a]->adjSize++] = graph.v[b];
                    graph.v[b]->adj[graph.v[b]->adjSize++] = graph.v[a];
                }
            }
        }
        */

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for (n = 100; n <= 200; n += 10) {
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
            graph.v[i]->adj = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected
        
        for (int i = 1; i < graph.nrNodes; i++) {
            graph.v[0]->adj[graph.v[0]->adjSize] = (Node*)malloc(sizeof(Node));
            graph.v[i]->adj[graph.v[0]->adjSize] = (Node*)malloc(sizeof(Node));
            graph.v[0]->adj[graph.v[0]->adjSize++] = graph.v[i];
            graph.v[i]->adj[graph.v[i]->adjSize++] = graph.v[0];
        }

        int* aux = (int*)malloc((4500 - 99) * sizeof(int));
        int* aux2 = (int*)malloc((4500 - 99) * sizeof(int));
        FillRandomArray(aux, 4500 - 99, 1, graph.nrNodes / 2, false, 0);
        FillRandomArray(aux2, 4500 - 99, graph.nrNodes / 2 + 1, graph.nrNodes - 1, false, 0);

        int k = 0;
        for (int i = 0; i < 4500 - 99; i++) {
            int ok = 0;
            for (int j = 0; j < graph.v[aux[i]]->adjSize; j++) {
                if (graph.v[aux[i]]->adj[j] == graph.v[aux2[i]]) {
                    ok = 1;
                   // break;
                }
            }
            if (!ok) {
                graph.v[aux[i]]->adj[graph.v[aux[i]]->adjSize] = (Node*)malloc(sizeof(Node));
                graph.v[aux2[i]]->adj[graph.v[aux2[i]]->adjSize] = (Node*)malloc(sizeof(Node));
                graph.v[aux[i]]->adj[graph.v[aux[i]]->adjSize++] = graph.v[aux2[i]];
                graph.v[aux2[i]]->adj[graph.v[aux2[i]]->adjSize++] = graph.v[aux[i]];
            }
           
        }
        
        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}
