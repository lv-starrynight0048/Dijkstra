
#include "dijkstra.h"

// Static function declaration---------------------------------------------------------

/**
 * @brief 内部函数：寻找有向图G中索引为para的第一个邻接顶点的索引
 * @return 返回para的第一个邻接顶点的索引，失败则返回-1
*/
static int first_vertex(Graph *pG, int para);

/**
 * @brief 内部函数：寻找有向图G中索引为para的邻接顶点pre的索引的下一个邻接顶点的索引
 * @return 返回para的下一个邻接顶点的索引，失败则返回-1
*/
static int next_vertex(Graph *pG, int para, int pre);

/**
 * @brief 深度搜索从para开始遍历图G
*/
static void DFS(Graph *pG, int fst_vex, BOOL *visited);

/**
 * @return 返回ch在邻接图G中的索引值
*/
static int get_position(Graph *pG, char ch);

/**
 * @brief 将优先队列进行排序（从小到大）
*/
static void sorted_PQueue(PQnode *pqueue, int length);

/**
 * @brief 从队列中寻找index这个结点
 * @param queue 优先队列
 * @param qlen 优先队列中的元素个数
 * @param index 需要寻找的的元素初始索引
*/
static PQnode *get_pqueue_point(PQnode *queue, int qlen, int index);

/**
 * @brief 将char列进行反转
 * @param arr char列
 * @param len char中元素个数
*/
static void inverse(char *arr, int len);

//function-----------------------------------------------------------------------------

Graph *Graph_example_create(){
    char vexs[8] = {'a','b','c','d','e','f','g','h'};
    int weight[8][8] = {
        {0,   9,   14,  15,  INF, INF, INF, INF},
        {INF, 0,   INF, INF, 24,  INF, INF, INF},
        {INF, INF, 0,   INF, 18,  30,  INF, INF},
        {INF, INF, INF, 0,   INF, 20,  INF, 44 },
        {INF, INF, INF, INF, 0,   2,   INF, 19 },
        {INF, INF, INF, INF, INF, 0,   11,  16 },
        {INF, INF, INF, INF, INF, INF, 0,   3  },
        {INF, INF, INF, INF, INF, INF, INF, 0  }
    };
    int vexnum = LENGTH(vexs);
    int i, j;

    Graph *pG = (Graph *)malloc(sizeof(Graph));
    if(pG == NULL)
        return NULL;
    memset(pG, 0, sizeof(Graph));

    pG->vexnum = vexnum;
    pG->edgenum = 0;
    pG->tag = TRUE; // 标识为有像图
    for(i=0; i<pG->vexnum; i++)
        pG->vexs[i] = vexs[i];
    for(i=0; i<pG->vexnum; i++){
        for(j=0; j<pG->vexnum; j++){
            pG->weight[i][j] = weight[i][j];
            if(pG->weight[i][j] != INF && i != j)
                pG->edgenum++;
        }   
    }
    return pG;
}

void Print_Graph(Graph *pG){
    int i, j;
    if(pG->tag == TRUE){
        printf("G为有向图\n");
    }else{
        printf("G为无向图\n");
    }
    printf("顶点数 = %d     边数 = %d\n", pG->vexnum, pG->edgenum);
    printf("The Graph matrix:\n");
    printf("          ");
    for(i=0; i<pG->vexnum; i++)
        printf("%10c", pG->vexs[i]);
    printf("\n");
    for(i=0; i<pG->vexnum; i++){
        printf("%10c", pG->vexs[i]);
        for(j=0; j<pG->vexnum; j++){
            if(pG->weight[i][j]== INF)
                printf("       INF");
            else   
                printf("%10d", pG->weight[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void DFSTraverse(Graph *pG){
    int i;
    BOOL visited[pG->vexnum];
    memset(visited, 0, sizeof(BOOL)*pG->vexnum);

    printf("DFS:\n");
    for(i=0; i<pG->vexnum; i++){
        if(visited[i] == FALSE)
            DFS(pG, i, visited);
    }
    printf("\nDFS finished!\n");
}

void BFSTraverse(Graph *pG){
    queue q;
    q.head = 0;
    q.tail = 0;
    memset(q.arr, 0, sizeof(int)*N);
    
    BOOL visited[pG->vexnum];
    memset(visited, FALSE, sizeof(BOOL)*pG->vexnum);

    int i, j, k;

    printf("BFS:\n");
    for(i=0; i<pG->vexnum; i++){
        if(visited[i] == FALSE){
            visited[i] = TRUE;
            printf("%c ", pG->vexs[i]);
            q.arr[q.tail++] = i; // 入队列
        }
        while(q.head != q.tail){
            j = q.arr[q.head++]; // 出队列
            for(k=first_vertex(pG, j); k>=0; k = next_vertex(pG, j, k)){
                if(visited[k] == FALSE){
                    visited[k] = TRUE;
                    printf("%c ", pG->vexs[k]);
                    q.arr[q.tail++] = k; // 入队列
                }
            }
        }
    }
    printf("\nBFS finished!\n");
}

void Prim(Graph *pG, int start){
    if(pG->tag == TRUE)
        printf("Warning: 图G为有向图，prim算法一般仅适合无向图求最小生成树\n");
    
    int index = 0;
    char prims[pG->vexnum]; // prim最小树顶点集合
    int weights[pG->vexnum]; // 顶点间边的权值

    int i, j, 
        add, 
        min, 
        sum = 0; // 记录prims最小生成树的权重

    // 将start添加到prims集合中
    prims[index++] = pG->vexs[start];
    for(i=0; i<pG->vexnum; i++)
        weights[i] = pG->weight[start][i];

    for(i=0; i<pG->vexnum; i++){
        if(i == start)
            continue;
        j = 0;
        add = 0;
        min = INF;
        
        // 筛选权重表中最小结点（离prims顶点结合最近的点）
        while(j < pG->vexnum){
            if(weights[j] != 0 && weights[j] < min){
                min = weights[j];
                add = j;
                sum = sum + min;
            }
            j++;
        }
        // 将筛选出来的点放入prims集合，并修改权值
        prims[index++] = pG->vexs[add];
        weights[add] = 0;
        for(j=0; j<pG->vexnum; j++){
            if(weights[j] != 0 && pG->weight[add][j] < weights[j])
                weights[j] = pG->weight[add][j];
        }
    }

    // 打印最小生成树
    printf("PRIM(%c) = %d: \n", pG->vexs[start], sum);
    printf("添加点的顺序如下：\n");
    for(i=0; i<index; i++)
        printf("%c ", prims[i]);
    printf("\n");
}

void Kruskal(Graph *pG){
    if(pG->tag == TRUE)
        printf("Warning: 图G为有向图，Krustal算法一般仅适合无向图求最小生成树\n");
    int i, j,
        v1, v2,
        group,
        index = 0,
        sum = 0;
    Edge *all_edges = get_edges(pG);
    sorted_edges(all_edges, pG->edgenum);
    Vex *vexs = get_vexs(pG);

    Edge kru_edges[pG->vexnum-1];

    for(i=0; i<pG->edgenum; i++){
        v1 = get_position(pG, all_edges[i].start);
        v2 = get_position(pG, all_edges[i].end);
        if(vexs[v1].visited == FALSE && vexs[v2].visited == FALSE){
            vexs[v1].visited = TRUE;
            vexs[v2].visited = TRUE;
            vexs[v2].group = vexs[v1].group;
            kru_edges[index++] = all_edges[i];
        }else if(vexs[v1].visited == TRUE && vexs[v2].visited == FALSE){
            vexs[v2].visited = TRUE;
            vexs[v2].group = vexs[v1].group;
            kru_edges[index++] = all_edges[i];
        }else if(vexs[v1].visited == FALSE && vexs[v2].visited == TRUE){
            vexs[v1].visited = TRUE;
            vexs[v1].group = vexs[v2].group;
            kru_edges[index++] = all_edges[i];
        }else if(vexs[v1].visited == TRUE && vexs[v2].visited == TRUE){
            if(vexs[v1].group != vexs[v2].group){
                group = vexs[v2].group;
                for(j=0; j<pG->edgenum; j++){
                    if(vexs[j].group == group)
                        vexs[j].group = vexs[v1].group;
                }
                kru_edges[index++] = all_edges[i];
            }
        }
    }
    free(all_edges);
    free(vexs);

    for(i=0; i<index; i++){
        sum = sum + kru_edges[i].weight;
    }

    printf("Kruskal = %d :\n", sum);
    printf("添加边的顺序如下：\n");
    for(i=0; i<index; i++)
        printf("%c——%c\n", kru_edges[i].start, kru_edges[i].end);
    printf("\n");
}

Path *Dijkstra(Graph *pG, int start, int end){
    // 初始化输出向量
    Path *result = (Path *)malloc(sizeof(Path));
    result->start = start;
    result->end = end;
    result->path_len = 0;
    result->num_vexs = 0;
    result->path_vexs = NULL;

    int i, j, 
        len1, len2
        // debug
        ;
    PQnode *pt, *tmp;

    // 初始化优先队列
    PQnode pqueue[pG->vexnum];
    for(i=0; i<pG->vexnum; i++){
        if(i == start){
            pqueue[i].out = TRUE;
            pqueue[i].dist = 0;
        }else{
            pqueue[i].out = FALSE;
            pqueue[i].dist = pG->weight[start][i];
        }
        pqueue[i].index = i;
        pqueue[i].vex = pG->vexs[pqueue[i].index];
        pqueue[i].pre_index = start;
        pqueue[i].pre_vex = pG->vexs[pqueue[i].pre_index];
    }
    sorted_PQueue(pqueue, pG->vexnum);

    pt = pqueue + 1; // 优先队列队头指针
    len1 = 1; // 最小路径已经求出的数列的长度 
    len2 = pG->vexnum - 1; // 优先队列的长度
    // sorted_PQueue(pt, pG->vexnum-1);
    for(i=1; i<pG->vexnum; i++){
        // 将优先队列进行排序
        sorted_PQueue(pt, len2);
        
        pt->out = TRUE;
        tmp = pt;
        // debug = tmp->dist;
        pt = pt + 1;
        len1++;
        len2--;
        // 如果end的最小值已经算出，则输出path列，并返回最短路径值
        if(tmp->index == end){
            result->path_len = tmp->dist;
            // 回溯法(back tracking)
            result->path_vexs = (char *)malloc(sizeof(char)*len1);
            for(j=0; tmp->index != start; tmp=get_pqueue_point(pqueue, len1, tmp->pre_index), j++){
                result->path_vexs[j] = tmp->vex;
            }
            result->path_vexs[j] = pG->vexs[start];
            result->num_vexs = j+1;
            // 将存储的路径结点列进行反转
            inverse(result->path_vexs, result->num_vexs);
            return result;
        }
        // 修改优先队列中的每个结点到start的距离
        for(j=0; j<len2; j++){
            if(pG->weight[tmp->index][pt[j].index]+tmp->dist < pt[j].dist &&
                pG->weight[tmp->index][pt[j].index] != INF){
                pt[j].dist = pG->weight[tmp->index][pt[j].index]+tmp->dist;
                pt[j].pre_index = tmp->index;
                pt[j].pre_vex = tmp->vex;
            }
        }
    }
    return NULL;
}

Edge *get_edges(Graph *pG){
    int i, j,
        index = 0;
    Edge *edges;
    edges = (Edge *)malloc(pG->edgenum*sizeof(Edge));
    for(i=0; i<pG->vexnum; i++){
        // 有向图和无向图区别处理
        if(pG->tag == TRUE)
            j = 0;
        else
            j = i + 1;

        for(; j<pG->vexnum; j++){
            if(pG->weight[i][j] != INF && pG->weight[i][j] != 0){
                edges[index].start = pG->vexs[i];
                edges[index].end = pG->vexs[j];
                edges[index].weight = pG->weight[i][j];
                index++;
            }
        }
    }
    return edges;
}

void sorted_edges(Edge *edges, int edgenum){
    int i, j;
    Edge tmp;
    for(i=0; i<edgenum; i++){
        for(j=i+1; j<edgenum; j++){
            if(edges[i].weight > edges[j].weight){
                tmp = edges[i];
                edges[i] = edges[j];
                edges[j] = tmp;
            }
        }
    }
}

Vex *get_vexs(Graph *pG){
    int i, j,
        deg, in_deg, out_deg;
    Vex *vexs = (Vex *)malloc(pG->vexnum*sizeof(Vex));
    for(i=0; i<pG->vexnum; i++){
        if(pG->tag == TRUE){
            vexs[i].tag = TRUE;
            vexs[i].visited = FALSE;
            vexs[i].vex = pG->vexs[i];
            vexs[i].group = i;
            out_deg = 0;
            in_deg = 0;
            for(j=0; j<pG->vexnum; j++){
                if(pG->weight[i][j] != INF && i != j)
                    out_deg++;
                if(pG->weight[j][i] != INF && i != j)
                    in_deg++;
            }
            deg = in_deg + out_deg;
            vexs[i].degree = deg;
            vexs[i].in_degree = in_deg;
            vexs[i].out_degree = out_deg;
        }else{
            vexs[i].tag = FALSE;
            vexs[i].visited = FALSE;
            vexs[i].vex = pG->vexs[i];
            vexs[i].group = i;
            vexs[i].in_degree = 0;
            vexs[i].out_degree = 0;
            deg = 0;
            for(j=0; j<pG->vexnum; j++){
                if(pG->weight[i][j] != INF && i != j)
                    deg++;
            }
            vexs[i].degree = deg;
        }
    }
    return vexs;
}

// Static function---------------------------------------------------------------------

static int first_vertex(Graph *pG, int para){
    int i;
    // 检查index是否是合法输入
    if(para < 0 || para > (pG->vexnum-1) )
        return -1;
    
    for(i=0; i<pG->vexnum; i++){
        if(pG->weight[para][i] != 0 && pG->weight[para][i] != INF)
            return i;
    }
    return -1;
}

static int next_vertex(Graph *pG, int para, int pre){
    int i;
    // 检查index, pre是否是合法输入
    if(para < 0 || para > (pG->vexnum-1) || pre < 0 || pre > (pG->vexnum-1))
        return -1;
    
    for(i=pre+1; i<pG->vexnum; i++){
        if(pG->weight[para][i] != 0 && pG->weight[para][i] != INF)
            return i;
    }
    return -1;
}

static void DFS(Graph *pG, int fst_vex, BOOL *visited){
    int vex;
    visited[fst_vex] = TRUE;
    printf("%c ", pG->vexs[fst_vex]);
    for(vex = first_vertex(pG, fst_vex); vex>=0; vex = next_vertex(pG, fst_vex, vex)){
        if(visited[vex] == FALSE)
            DFS(pG, vex, visited);
    }
}

static int get_position(Graph *pG, char ch){
    int i;
    for(i=0; i<pG->vexnum; i++)
        if(pG->vexs[i] == ch)
            return i;
    return -1;
}

static void sorted_PQueue(PQnode *pqueue, int length){
    int i;
    PQnode tmp;
    for(i=1; i<length; i++){
        if(pqueue[0].dist > pqueue[i].dist){
            tmp =  pqueue[0];
            pqueue[0] = pqueue[i];
            pqueue[i] = tmp;
        }
    }
}

static PQnode *get_pqueue_point(PQnode *queue, int qlen, int index){
    PQnode *find = queue;
    int i;
    for(i=0; i<qlen; i++){
        if(queue[i].index == index){
            find = &queue[i];
            return find;
        }    
    }
    return NULL;
}

static void inverse(char *arr, int len){
    int i, j;
    char tmp;
    for(i=0,j=len-1; i<j; i++,j--){
        tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}