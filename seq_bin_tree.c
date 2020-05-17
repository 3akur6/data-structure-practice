// features: 随机生成结点关系，并以凹入图呈现
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAXSIZE 1024
#define MAXDEPTH 6
#define SYM "#####"
#define SYM_LEN strlen(SYM)

typedef struct
{
    int value;
} data_t; // 修改数据结构同时也要修改is_equal()函数

typedef struct
{
    int status;
    int r_idx;
    data_t data;
} node_t;

typedef node_t sq_btree_t[MAXSIZE];

node_t * sq_btree_init(sq_btree_t);
bool sq_btree_destroy(sq_btree_t, sq_btree_t);
node_t * sq_btree_find(sq_btree_t, data_t, sq_btree_t);
node_t * sq_btree_rchild(sq_btree_t, sq_btree_t);
node_t * sq_btree_lchild(sq_btree_t, sq_btree_t);
int sq_btree_depth(sq_btree_t, sq_btree_t);
void sq_btree_show(sq_btree_t, sq_btree_t);
int sq_btree_nodes(sq_btree_t, sq_btree_t);
node_t * random_sq_btree(sq_btree_t, sq_btree_t, int *);

bool is_equal(data_t *, data_t *); // 自定义数据类型比较函数
char * format_node(node_t *, int);
char * to_s(int, int);
int * shuffle(int *, int *, int);
bool include(int *, int, int);
int * take(int *, int, int *, int);

int main(void)
{
    sq_btree_t tree;
    sq_btree_init(tree);
    int curr_dpth = 0;
    srand((unsigned) time(NULL));
    random_sq_btree(tree, tree, &curr_dpth);
    int j = 1;
    for (int i = 0; i < MAXSIZE; ++i)
    {
        node_t *node= &tree[i];
        if (node->status)
        {
            node->status = j;
            j++;
        }
    }
    sq_btree_show(tree, tree);
    return 0;
}

node_t * sq_btree_init(sq_btree_t tree)
{
    for (int i = 0; i < MAXSIZE; ++i)
    {
        tree[i].status = 0;
        tree[i].r_idx = i;
    }
    return tree;
}

bool sq_btree_destroy(sq_btree_t tree, sq_btree_t m_tree)
{
    if (tree->status)
    {
        sq_btree_destroy(sq_btree_lchild(tree, m_tree), m_tree);
        sq_btree_destroy(sq_btree_rchild(tree, m_tree), m_tree);
        tree->status = 0;
        free(tree);
    }
    return true;
}

node_t * sq_btree_find(sq_btree_t tree, data_t v, sq_btree_t m_tree)
{
    node_t *lchild;
    node_t *rchild;
    node_t *lf;
    node_t *rf;
    lchild = sq_btree_lchild(tree, m_tree);
    rchild = sq_btree_rchild(tree, m_tree);
    if (is_equal(&tree->data, &v) || !tree->status)
    {
        return tree;
    }
    lf = sq_btree_find(lchild, v, m_tree);
    rf = sq_btree_find(rchild, v, m_tree);
    if (!lf) // 不写成return lf || rf，为了避免不明确指定的warning提示
    {
        return lf;
    } else {
        return rf;
    }
}

node_t * sq_btree_rchild(sq_btree_t tree, sq_btree_t m_tree)
{
    return &m_tree[2 * tree->r_idx + 2];
}

node_t * sq_btree_lchild(sq_btree_t tree, sq_btree_t m_tree)
{
    return &m_tree[2 * tree->r_idx + 1];
}

int sq_btree_depth(sq_btree_t tree, sq_btree_t m_tree)
{
    node_t *rchild;
    node_t *lchild;
    rchild = sq_btree_rchild(tree, m_tree);
    lchild = sq_btree_lchild(tree, m_tree);
    if (!rchild->status && !lchild->status)
    {
        return 1;
    } else {
        int depth;
        int rdepth;
        int ldepth;
        ldepth = sq_btree_depth(lchild, m_tree);
        rdepth = sq_btree_depth(rchild, m_tree);
        depth = ldepth > rdepth ? ldepth : rdepth;
        return depth + 1;
    }
}

void sq_btree_show(sq_btree_t tree, sq_btree_t m_tree)
{
    node_t *lchild;
    node_t *rchild;
    int width;
    lchild = sq_btree_lchild(tree, m_tree);
    rchild = sq_btree_rchild(tree, m_tree);
    width = sq_btree_depth(m_tree, m_tree);
    if (tree->status)
    {
        char show[10] = "%";
        strcat(show, to_s(width * SYM_LEN + 1, 10));
        strcat(show, "s\n");
        printf(show, format_node(tree, width));
    }
    if (lchild->status || rchild->status)
    {
        sq_btree_show(lchild, m_tree);
        sq_btree_show(rchild, m_tree);
    }
}

int sq_btree_nodes(sq_btree_t tree, sq_btree_t m_tree)
{
    node_t *lchild = sq_btree_lchild(tree, m_tree);
    node_t *rchild = sq_btree_rchild(tree, m_tree);
    if (!tree->status)
    {
        return 0;
    }
    if (!lchild->status && !rchild->status)
    {
        return 1;
    }
    int lchild_nodes = lchild->status ? sq_btree_nodes(lchild, m_tree) : 0;
    int rchild_nodes = rchild->status ? sq_btree_nodes(rchild, m_tree) : 0;
    return lchild_nodes + rchild_nodes + 1;
}

char * format_node(node_t *n, int width)
{
    int len = (int) ((log(n->r_idx + 1) / log(2)) + 1);
    char *rslt = (char *) malloc(sizeof(char) * MAXSIZE);
    char *id = to_s(n->status, 10);
    strcat(rslt, id);
    for (int i = 0; i < width - len + 1; ++i)
    {
        strcat(rslt, SYM);
    }
    return rslt;
}

bool is_equal(data_t *d1, data_t *d2)
{
    return d1->value == d2->value;
}

char * to_s(int n, int base)
{
    int i = 0;
    int t = n;
    while (t >= base)
    {
        t /= base;
        i++;
    }
    i++;
    char *retn = (char *) malloc(sizeof(char) * (i + 1));
    for (int j = 0; j < i; ++j)
    {
        retn[i - 1 - j] = n % base + 48;
        n /= base;
    }
    retn[i] = '\0';
    return retn;
}

node_t * random_sq_btree(sq_btree_t tree, sq_btree_t m_tree, int *curr_dpth)
{
    int rd = rand() % 4;
    int count = sq_btree_nodes(m_tree, m_tree);
    if (*curr_dpth < MAXDEPTH)
    {
        tree->status = -1;
        (*curr_dpth)++;
        switch (rd)
        {
            case 0: // 没有子结点
            {
                return tree;
            }
            case 1: // 有左孩子结点
            {
                node_t *lchild = sq_btree_lchild(tree, m_tree);
                lchild->status = -1;
                random_sq_btree(lchild, m_tree, curr_dpth);
                break;
            }
            case 2: // 有右孩子结点
            {
                node_t *rchild = sq_btree_rchild(tree, m_tree);
                rchild->status = -1;
                random_sq_btree(rchild, m_tree, curr_dpth);
                break;
            }
            case 3: // 有左右孩子节点
            {
                node_t *lchild = sq_btree_lchild(tree, m_tree);
                node_t *rchild = sq_btree_rchild(tree, m_tree);
                lchild->status = -1;
                rchild->status = -1;
                random_sq_btree(lchild, m_tree, curr_dpth);
                random_sq_btree(rchild, m_tree, curr_dpth);
                break;
            }
        }
        (*curr_dpth)--;
    }
    return tree;
}
