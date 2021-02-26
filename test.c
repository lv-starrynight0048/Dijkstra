#include "dijkstra.h"

//test code----------------------------------------------------------------------------

int main(){
    Graph *pG = Graph_example_create(); printf("\n");
    Print_Graph(pG); printf("\n");
    DFSTraverse(pG); printf("\n");
    BFSTraverse(pG); printf("\n");
    Prim(pG, 0); printf("\n");
    Kruskal(pG); printf("\n");

    Path *result = Dijkstra(pG, 1, 6);
    printf("Dijkstra:\n");
    printf("path_len = %d, num_vexs = %d\n", result->path_len, result->num_vexs);
    for(int i=0; i<result->num_vexs; i++){
        printf("%c ", result->path_vexs[i]);
    }
    printf("\n");
    return 0;
}