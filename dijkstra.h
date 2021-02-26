#ifndef __DIJKSTRA_H_
#define __DIJKSTRA_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#define N 10 // 顶点个数
#define M 13 // 有向图边数
#define INF (~(0x1<<31)) 
#define LENGTH(a) (sizeof(a)/sizeof(a[0]))

#define TRUE 1
#define FALSE 0

typedef int BOOL;

//structure----------------------------------------------------------------------------
/* 邻接矩阵 */
typedef struct{
    char vexs[N]; // 顶点集合
    int weight[N][N]; // 邻接矩阵
    int vexnum;
    int edgenum;
    BOOL tag; // tag=1 有向图；tag=0 无向图
}Graph;

/* 边结构体 */
typedef struct{
    char start;
    char end;
    int weight;
}Edge;

/* 顶点结构体 */
typedef struct{
    char vex;
    BOOL tag; // 标识是否是有向图
    BOOL visited; // 是否被访问过
    int group; // kruskal中要的组标识
    int degree; // 结点的度
    int in_degree; // 结点的入度
    int out_degree; // 结点的出度
}Vex;

/* 辅助队列 */
typedef struct {
    int arr[N];
    int head;
    int tail;
}queue;

/* 优先队列结点 */
typedef struct{
    char vex; // 结点符号
    int index; // 结点在图中存储的索引
    int dist; // 存储离参考结点的最短距离
    char pre_vex; // 存储该结点对应的最短路径经过的前一个结点
    int pre_index; // 存储前一个结点对应的图中索引
    BOOL out; // 标记是否已经找到最短路径（即是否已经出队）
}PQnode;

/* 记录路径的结构体 */
typedef struct{
    int start;
    int end;
    char *path_vexs; // 存储路径经过的结点
    int num_vexs; // 路径经过的结点数
    int path_len; // 路径的总长度
}Path;


//function declaration-----------------------------------------------------------------

/**
 * @brief 有向图邻接矩阵实例
*/
Graph *Graph_example_create();

/**
 * @brief 打印邻接矩阵
*/
void Print_Graph(Graph *pG);

/**
 * @brief 深度优先搜索遍历图G
*/
void DFSTraverse(Graph *pG);

/**
 * @brief 广度优先搜索遍历图G
*/
void BFSTraverse(Graph *pG);

/**
 * @brief Prim算法生成无向图最小生成树
 * @param pG 邻接矩阵图指针
 * @param start 从start元素开始搜索
*/
void Prim(Graph *pG, int start);

/**
 * @brief Kruskal算法生成无向图最小生成树
 * @param pG 邻接矩阵图指针
*/
void Kruskal(Graph *pG);

/**
 * @brief Dijkstra算法寻找从start到end的最短路径
 * @param pG 邻接矩阵图指针
 * @param start 起始顶点
 * @param end 终止顶点
 * @param path 用来返回中间经过的顶点数组
 * @return 返回从start到end的最短路径长度
*/
Path *Dijkstra(Graph *pG, int start, int end);

/**
 * @brief 获取邻接图中的边
 * @param pG 邻接矩阵图指针
 * @return 返回存储邻接图的边信息的指针Edge *
*/
Edge *get_edges(Graph *pG);

/**
 * @brief 对边集合edges按照权值大小排序(冒泡排序)
 * @param edges 存储邻接图的边信息的指针
 * @param edgenum 图中的边数
*/
void sorted_edges(Edge *edges, int edgenum);

/**
 * @brief 获取邻接图中的顶点
 * @param pG 邻接矩阵图指针
 * @return 返回存储邻接图的边信息的指针Vex *
*/
Vex *get_vexs(Graph *pG);

#endif