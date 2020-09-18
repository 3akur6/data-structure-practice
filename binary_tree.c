#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define ELEMTYPE char //二叉树数据类型
#define MAXSIZE 100   //新数组数据域大小

//定义二叉树结构
typedef struct BinTree
{
    ELEMTYPE data;         //结点数据域
    struct BinTree *left;  //左子树
    struct BinTree *right; //右子树
} BinTree;

//定义新数组结构，用于灵活存取结果
typedef struct
{
    int size;               //新数组存储元素数
    ELEMTYPE data[MAXSIZE]; //顺序结构存储数据
} Array;

bool btree_init(BinTree *);        //初始化二叉树
int preorder(BinTree *, Array *);  //二叉树先序遍历
int inorder(BinTree *, Array *);   //二叉树中序遍历
int postorder(BinTree *, Array *); //二叉树后序遍历
int btree_depth(BinTree *);        //返回二叉树深度
int btree_count_leaves(BinTree *); //返回二叉树叶子结点数

bool array_init(Array *);
bool push(Array *, ELEMTYPE); //定义向新数组末尾添加元素操作
ELEMTYPE shift(Array *);      //返回新数组首元素
ELEMTYPE pop(Array *);        //返回新数组尾元素

int parse_btree(BinTree *, char *, int); //自定义格式化用户输入，并抽象成二叉树，返回值为结点个数
void show(char *, Array *);

int main(void)
{
    BinTree b_tree;
    btree_init(&b_tree);

    char input[MAXSIZE];
    Array travel;
    array_init(&travel);

    //指定用户输入二叉树格式
    //^表示结点为空
    //;表示该层结束输入
    printf(
        "Acceptable format:\n"
        "<\e[33m^\e[0m> tells that the node is empty\n"
        "<\e[33m;\e[0m> separates each layer\n\n");

    //提示用户输入二叉树
    printf("Now your input: ");

    //获取用户输入
    scanf("%s", input);
    
    printf("\n");

    //格式化用户输入，并抽象成二叉树
    parse_btree(&b_tree, input, strlen(input));

    //输出先序遍历结果
    preorder(&b_tree, &travel);
    show("Preorder", &travel);

    //输出中序遍历结果
    array_init(&travel);
    inorder(&b_tree, &travel);
    show("Inorder", &travel);

    //输出后序遍历结果
    array_init(&travel);
    postorder(&b_tree, &travel);
    show("Postorder", &travel);

    printf("\n");

    //输出二叉树深度
    printf("BinTree depth: %d\n", btree_depth(&b_tree));

    //输出二叉树叶子节点数
    printf("BinTree leaves: %d\n", btree_count_leaves(&b_tree));

    printf("\n");

    return 0;
}

bool btree_init(BinTree *b_tree)
{
    b_tree->left = NULL;
    b_tree->right = NULL;

    return true;
}

int preorder(BinTree *b_tree, Array *travel)
{
    if (!b_tree)
    {
        return 0;
    }

    push(travel, b_tree->data);
    preorder(b_tree->left, travel);
    preorder(b_tree->right, travel);

    return travel->size;
}

int inorder(BinTree *b_tree, Array *travel)
{
    if (!b_tree)
    {
        return 0;
    }

    inorder(b_tree->left, travel);
    push(travel, b_tree->data);
    inorder(b_tree->right, travel);

    return travel->size;
}

int postorder(BinTree *b_tree, Array *travel)
{
    if (!b_tree)
    {
        return 0;
    }

    postorder(b_tree->left, travel);
    postorder(b_tree->right, travel);
    push(travel, b_tree->data);

    return travel->size;
}

int btree_depth(BinTree *b_tree)
{
    if (!b_tree->left && !b_tree->right)
    {
        return 1;
    }
    else
    {
        int l_depth = b_tree->left ? btree_depth(b_tree->left) : 0;
        int r_depth = b_tree->right ? btree_depth(b_tree->right) : 0;
        int depth = (l_depth > r_depth ? l_depth : r_depth);

        return (depth + 1);
    }
}

int btree_count_leaves(BinTree *b_tree)
{
    if (!b_tree) //二叉树为空则返回0，表示无叶子结点
    {
        return 0;
    }

    if (!b_tree->left && !b_tree->right) //仅当二叉树左右子树为空，表示该结点为叶子节点
    {
        return 1; //返回叶子结点数为1
    }
    else
    {
        //叶子结点总数为左右二叉树叶子结点和
        int left_nodes = b_tree->left ? btree_count_leaves(b_tree->left) : 0;
        int right_nodes = b_tree->right ? btree_count_leaves(b_tree->right) : 0;
        return (left_nodes + right_nodes);
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

ELEMTYPE shift(Array *arr)
{
    ELEMTYPE rslt = (arr->data)[0];

    for (int i = 0; i < arr->size; i++)
    {
        for (int j = i + 1; j < arr->size; j++)
        {
            arr->data[j - 1] = arr->data[j];
        }
    }

    (arr->size)--;

    return rslt;
}

ELEMTYPE pop(Array *arr)
{
    ELEMTYPE rslt = (arr->data)[arr->size - 1];
    (arr->size)--;
    return rslt;
}

int parse_btree(BinTree *btree, char *input, int length)
{
    int size = 1;
    int nodes = 0;
    int position = 0;
    int layers = 0;
    BinTree *cache[MAXSIZE];
    BinTree *before[MAXSIZE];

    if (*input == '^')
    {
        return 0;
    }

    btree->data = *input;
    
    if (length <= 2)
    {
        return 1;
    }

    cache[0] = btree;

    nodes++;
    position++;
    layers = 2;
    btree->left = (BinTree *)malloc(sizeof(BinTree));
    btree->right = (BinTree *)malloc(sizeof(BinTree));

    for (int i = 2; i < length; i++)
    {
        switch (*(input + i))
        {
        case '^':
            position++;
            break;
        case ';':
        {
            layers++;

            //将cache数组暂存在before中
            for (int m = 0; m < size; m++)
            {
                before[m] = cache[m];
            }

            //cache数组扩展
            for (int n = 0; n < 2 * size; n++)
            {
                if (!(n % 2))
                {
                    cache[n] = before[n / 2]->left;
                }
                else
                {
                    cache[n] = before[n / 2]->right;
                }
            }
            size *= 2;

            break;
        }
        default:
        {
            int select;
            nodes++;
            position++;

            select = (position - (int)pow(2.0, (double)(layers - 1))) / 2;

            BinTree *new = (BinTree *)malloc(sizeof(BinTree));
            new->data = *(input + i);

            if (!(position % 2))
            {
                cache[select]->left = new;
            }
            else
            {
                cache[select]->right = new;
            }
        }
        }
    }

    return nodes;
}

void show(char *title, Array *content)
{
    printf("%s: ", title);
    for (int i = 0; i < content->size; i++)
    {
        printf("%c", (content->data)[i]);
    }
    printf("\n");
}
