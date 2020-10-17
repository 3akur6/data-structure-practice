#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAXSIZE 100
#define SIZE 6
#define ELEMTYPE int

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

//dijkstra算法计算最短路径,接受参数为图指针、起始点、路径保存的数组指针
bool dijkstra_shortest_path(AdjacentMatrixGraph *, ELEMTYPE, Array *[SIZE]);

bool array_init(Array *);        //新数组结构初始化
bool push(Array *, ELEMTYPE);    //定义向新数组末尾添加元素操作
bool shift(Array *, ELEMTYPE *); //返回新数组首元素

int locate(AdjacentMatrixGraph *, ELEMTYPE);               //定位顶点在顶点集的位置,返回数组下标
int path_value(AdjacentMatrixGraph *, Array *[SIZE], int); //计算带权路径长度，接收参数为图指针、路径数组、目的顶点的数组下标
void show_path(AdjacentMatrixGraph *, Array *[SIZE]);      //自定义函数显示路径
void show_matrix(int[MAXSIZE][MAXSIZE]);

//指定顶点列表
const ELEMTYPE vertices[SIZE] = {0, 1, 2, 3, 4, 5};
//指定邻接矩阵
const int edges_matrix[SIZE][SIZE] = {
    {0, 0, 10, 0, 30, 100},
    {0, 0, 5, 0, 0, 0},
    {0, 0, 0, 50, 0, 0},
    {0, 0, 0, 0, 0, 10},
    {0, 0, 0, 20, 0, 60},
    {0, 0, 0, 0, 0, 0}};

int main(void)
{
    AdjacentMatrixGraph graph;

    //定义路径数组的指针数组，存放各顶点的路径数组指针
    Array *path_list[SIZE];

    //路径指针数组初始化
    //路径数组顺序存放依次经过的顶点，不包含目的顶点自身
    for (int i = 0; i < SIZE; i++)
    {
        path_list[i] = (Array *)malloc(sizeof(Array));
        array_init(path_list[i]);
    }

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

    printf("Graph Edges Matrix:\n\n");

    show_matrix(graph.edges_matrix);

    printf("\n");

    //指定起始点
    ELEMTYPE start = graph.vertices[0];

    dijkstra_shortest_path(&graph, start, path_list);

    show_path(&graph, path_list);

    return 0;
}

bool dijkstra_shortest_path(AdjacentMatrixGraph *graph, ELEMTYPE start, Array *path_list[SIZE])
{
    //定义start的邻接数组并初始化
    Array adjacent;
    array_init(&adjacent);

    int loc = locate(graph, start);

    //start的带权路径和
    int pre_path_value = path_value(graph, path_list, loc);

    for (int i = 0; i < SIZE; i++)
    {
        int weight = graph->edges_matrix[loc][i];

        if (weight)
        {
            //保存当前顶点的邻接顶点，成为该顶点的邻接数组
            push(&adjacent, graph->vertices[i]);

            //vertices[i]的带权路径和
            int succ_path_value = path_value(graph, path_list, i);

            //如果新路径的带权长度小于当前带权值，则更新路径数组
            if (!succ_path_value || (pre_path_value + weight) < succ_path_value)
            {
                //重新初始化路径数组
                array_init(path_list[i]);

                //将新路径的顶点信息保存到vertices[i]的路径数组中
                //就是将其前驱顶点的当前路径数组保存，再保存该前驱顶点
                for (int j = 0; j < path_list[loc]->size; j++)
                {
                    push(path_list[i], path_list[loc]->data[j]);
                }
                push(path_list[i], start);
            }
        }
    }

    //对start顶点的邻接数组依次用dijkstra算法计算，直到遍历所有可达顶点
    for (int i = 0; i < adjacent.size; i++)
    {
        ELEMTYPE vertex;

        if (shift(&adjacent, &vertex))
        {
            dijkstra_shortest_path(graph, vertex, path_list);
        }
    }

    return true;
}

int path_value(AdjacentMatrixGraph *graph, Array *path_list[SIZE], int v)
{
    //定义带权路径和
    int count = 0;

    for (int i = 0; i < path_list[v]->size; i++)
    {
        //标记是否为路径数组中最后一个顶点
        //由于路径数组不包含目的顶点自身，需要单独计算最后一段带权路径
        bool flag = (i == path_list[v]->size - 1);
        ELEMTYPE pre = path_list[v]->data[i];
        //如果当前遍历对象为路径数组最后一个顶点，则将后继顶点设为目的顶点
        ELEMTYPE succ = (flag ? graph->vertices[v] : path_list[v]->data[i + 1]);

        int pre_loc = locate(graph, pre);
        //如果当前遍历对象为路径数组最后一个顶点，直接返回目的顶点在图顶点数组的位置
        int succ_loc = (flag ? v : locate(graph, succ));

        //更新带权路径和
        count += graph->edges_matrix[pre_loc][succ_loc];
    }

    return count;
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

bool shift(Array *arr, ELEMTYPE *d)
{
    if (arr->size)
    {
        *d = (arr->data)[0];

        for (int i = 0; i < arr->size; i++)
        {
            for (int j = i + 1; j < arr->size; j++)
            {
                arr->data[j - 1] = arr->data[j];
            }
        }

        (arr->size)--;

        return true;
    }
    return false;
}

void show_path(AdjacentMatrixGraph *graph, Array *path_list[SIZE])
{
    printf("Notes: -1 means vertex unreachable\n\n");
    for (int i = 0; i < SIZE; i++)
    {
        Array *current = path_list[i];

        printf("shortest path to %d: ", graph->vertices[i]);

        for (int j = 0; j < current->size; j++)
        {
            printf("%d  ", current->data[j]);
        }
        //-1表示不可达
        printf("%d", (current->size ? graph->vertices[i] : -1));
        printf("\n");
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
            printf("%-3d ", matrix[i][j]);
        }
        printf("\n");
    }
}
