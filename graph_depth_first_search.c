#include <stdio.h>
#include <stdbool.h>

#define MAXSIZE 100  //邻接矩阵表示的图的最大顶点数
#define SIZE 6       //指定顶点列表的大小
#define ELEMTYPE int //顶点数据类型

//定义以邻接矩阵表示的图
typedef struct
{
    ELEMTYPE vertices[MAXSIZE];         //图的顶点集
    int edges_matrix[MAXSIZE][MAXSIZE]; //图的邻接矩阵
} AdjacentMatrixGraph;

//定义新数组结构，用于灵活存取结果
typedef struct
{
    int size;               //数组当前存储数
    ELEMTYPE data[MAXSIZE]; //以顺序方式存储元素
} Array;

//图的深度优先搜索函数，接受参数为待遍历的图指针、开始遍历顶点和遍历结果数组
bool graph_depth_first_search(AdjacentMatrixGraph *, ELEMTYPE, Array *);

int locate(AdjacentMatrixGraph *, ELEMTYPE); //定位初始遍历顶点的位置
void show_dfs(Array *);                      //自定义函数显示结果
void show_matrix(int (*)[MAXSIZE]);          //自定义函数显示邻接矩阵
bool include(Array *, ELEMTYPE);             //查找数组中是否包含指定元素

bool array_init(Array *);     //新数组结构初始化
bool push(Array *, ELEMTYPE); //定义向新数组末尾添加元素操作

//指定顶点列表
const ELEMTYPE vertices[SIZE] = {1, 2, 3, 4, 5, 6};
//指定邻接矩阵
const int edges_matrix[SIZE][SIZE] = {
    {0, 1, 1, 1, 0, 0},
    {1, 0, 0, 0, 1, 0},
    {1, 0, 0, 0, 1, 0},
    {1, 0, 0, 0, 0, 1},
    {0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0}};

int main(void)
{
    AdjacentMatrixGraph graph;

    //初始化图的顶点数组
    for (int i = 0; i < SIZE; i++)
    {
        graph.vertices[i] = vertices[i];
    }

    //初始化图的邻接矩阵
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            graph.edges_matrix[i][j] = edges_matrix[i][j];
        }
    }

    //指定初始遍历顶点
    ELEMTYPE start = graph.vertices[0];

    Array route;
    array_init(&route);

    printf("Graph Edges Matrix:\n\n");
    show_matrix(graph.edges_matrix);
    printf("\n");

    graph_depth_first_search(&graph, start, &route);

    printf("Graph DFS Result: ");
    show_dfs(&route);

    return 0;
}

bool graph_depth_first_search(AdjacentMatrixGraph *graph, ELEMTYPE start, Array *route)
{
    int loc;

    //如果顶点不存在于图中或已存在于遍历结果中，则直接返回false
    if ((loc = locate(graph, start)) < 0 || include(route, graph->vertices[loc]))
    {
        return false;
    }

    push(route, graph->vertices[loc]);

    bool rtrn = false;

    //遍历start的邻接顶点
    for (int i = 0; i < SIZE; i++)
    {
        //如果存在边，则深入，以新顶点作为初始点，继续遍历
        if (graph->edges_matrix[loc][i])
        {
            //取消已遍历后的边
            graph->edges_matrix[loc][i] = 0;
            graph->edges_matrix[i][loc] = 0;

            //获得从邻接顶点深度优先搜索结果
            rtrn = graph_depth_first_search(graph, graph->vertices[i], route);

            //start的邻接顶点的深度优先搜索失败，则继续寻找下一个邻接顶点，否则返回成功
            if (rtrn)
            {
                return true;
            }
        }
    }

    return false;
}

int locate(AdjacentMatrixGraph *graph, ELEMTYPE start)
{
    for (int i = 0; i < SIZE; i++)
    {
        if (graph->vertices[i] == start)
        {
            return i;
        }
    }
    return -1;
}

void show_dfs(Array *route)
{
    for (int i = 0; i < SIZE; i++)
    {
        printf("%d", route->data[i]);
    }
    printf("\n");
}

void show_matrix(int matrix[MAXSIZE][MAXSIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        printf("  ");
        for (int j = 0; j < SIZE; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

bool array_init(Array *arr)
{
    arr->size = 0;
    return true;
}

bool push(Array *arr, ELEMTYPE d)
{
    if (arr->size + 1 <= MAXSIZE)
    {
        (arr->data)[arr->size] = d;
        (arr->size)++;
        return true;
    }
    return false;
}

bool include(Array *route, ELEMTYPE d)
{
    for (int i = 0; i < route->size; i++)
    {
        if (route->data[i] == d)
        {
            return true;
        }
    }
    return false;
}
