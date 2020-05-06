// feature: 随机生成字样, 指定测试字样长度
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

#define LEN 20 //测试字样长度

typedef struct node
{
    int data;
    struct node *next;
} node;

typedef node *link_list;

bool link_list_init(link_list *);
bool link_list_create(link_list, node *, int);
node * link_list_read(link_list, int);
bool link_list_partition(link_list, link_list, link_list, link_list);

int to_i(char *);
void show_node(node *);
bool pack_node(node *, char);
void show_link_list(link_list);
void terminal_table(char *, link_list);
bool dup(node **, node *, bool);
bool shuffle(char *, char *, int);
bool include(char *, int, int);
bool take(char *, int, char *, int);

int main(void)
{
    node *node_list[LEN];
    node *nodes;
    nodes = (node *) malloc(sizeof(node) * LEN);
    for (int i = 0; i < LEN; ++i)
    {
        node_list[i] = &nodes[i];
    }
    char mixalpha_numeric[62] = {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D',
        'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
        'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
        'U', 'V', 'W', 'X', 'Y', 'Z'
    };
    char a[62];
    char value_list[LEN];
    shuffle(mixalpha_numeric, a, 62);
    take(a, LEN, value_list, 62);
    link_list llist,
              digit_nodes,
              lower_nodes,
              upper_nodes;
    link_list_init(&llist);
    link_list_init(&digit_nodes);
    link_list_init(&lower_nodes);
    link_list_init(&upper_nodes);
    for (int i = 0; i < LEN; ++i)
    {
        pack_node(node_list[i], value_list[i]);
        link_list_create(llist, node_list[i], i + 1);
    }
    link_list_partition(llist, digit_nodes, lower_nodes, upper_nodes);
    terminal_table("Raw List Link", llist);
    terminal_table("Digit Link List", digit_nodes);
    terminal_table("Downcase Link List", lower_nodes);
    terminal_table("Upcase Link List", upper_nodes);
    printf("\n");
    free(nodes);
    return 0;
}

bool pack_node(node *node, char data)
{
    if (isdigit(data))
    {
        char tmp[2] = {data, '\0'};
        node->data = to_i(tmp);
    } else {
        node->data = data;
    }
    return true;
}

bool link_list_init(link_list *llist)
{
    *llist = (node *) malloc(sizeof(node));
    (*llist)->data = 0;
    (*llist)->next = NULL;
    return true;
}

bool link_list_create(link_list llist, node *nd, int location)
{
    if (location <= llist->data + 1 && location >= 1)
    {
        node *current = llist;
        node *ancestor;
        node *successor;
        int i;
        for (i = 0; i < location; ++i)
        {
            if (i == location - 1)
            {
                ancestor = current;
            }
            current = current->next;
        }
        successor = current;
        nd->next = successor;
        ancestor->next = nd;
        llist->data++;
        return true;
    }
    return false;
}

int to_i(char *string)
{
    int num = 0;
    for (int i = 0, j = strlen(string) - 1; i < strlen(string) && isdigit(string[i]); ++i, --j)
    {
        num += (string[i] - 48) * pow(10, j);
    }
    return num;
}

void show_node(node *nd)
{
    if (nd->data >= 0 && nd->data <= 9)
    {
        printf("data: \033[33m%d\033[0m, this loc: \033[32m%p\033[0m, next loc: \033[32m%p\033[0m\n", nd->data, nd, nd->next);
    } else if (nd->data >= 97 && nd->data <= 122)
    {
        printf("data: \033[35m%c\033[0m, this loc: \033[32m%p\033[0m, next loc: \033[32m%p\033[0m\n", nd->data, nd, nd->next);
    } else if (nd->data >= 65 && nd->data <= 90)
    {
        printf("data: \033[36m%c\033[0m, this loc: \033[32m%p\033[0m, next loc: \033[32m%p\033[0m\n", nd->data, nd, nd->next);
    }
}

node * link_list_read(link_list llist, int location)
{
    if (location <= llist->data && location >= 1)
    {
        node *retn;
        retn = llist;
        for (int i = 0; i < location; ++i)
        {
            retn = retn->next;
        }
        return retn;
    }
    return NULL;
}

bool link_list_partition(link_list llist, link_list digit_nodes, link_list lower_nodes, link_list upper_nodes)
{
    int i = 1;
    int j = 1;
    int k = 1;
    for (int t = 0; t < llist->data; ++t)
    {
        node *current = link_list_read(llist, t + 1);
        node *new;
        dup(&new, current, false);
        int data = current->data;
        if (data >= 0 && data <= 9) //0..9
        {
            link_list_create(digit_nodes, new, i);
            i++;
        } else if (data >= 97 && data <= 122) //'a'..'z'
        {
            link_list_create(lower_nodes, new, j);
            j++;
        } else if (data >= 65 && data <= 90) //'A'..'Z'
        {
            link_list_create(upper_nodes, new, k);
            k++;
        }
    }
    return true;
}

void show_link_list(link_list llist)
{
    for (int i = 0; i < llist->data; ++i)
    {
        show_node(link_list_read(llist, i + 1));
    }
}

bool dup(node **retn, node *raw, bool flag) //flag ? retn->next = raw->next: retn->next = NULL
{
    *retn = (node *) malloc(sizeof(node));
    (*retn)->data = raw->data;
    (*retn)->next = flag ? raw->next : NULL;
    return true;
}

void terminal_table(char *title, link_list llist)
{
    printf("\n\033[1;34m%s\033[0m\n", title);
    for (int i = 0; i < strlen(title); ++i)
    {
        printf("-");
    }
    printf("\n");
    show_link_list(llist);
}

bool shuffle(char *arr, char *retn, int arr_len) //48..57 && 97..122 && 65..90
{
    int memo_len = 0;
    srand((unsigned) time(NULL));
    while (memo_len < arr_len)
    {
        int rd = rand() % arr_len;
        if (!include(arr, rd, arr_len))
        {
            retn[memo_len] = arr[rd];
            memo_len++;
        }
    }
    return true;
}

bool include(char *arr, int t, int arr_len)
{
    for (int i = 0; i < arr_len; ++i)
    {
        if (arr[i] == t)
        {
            return true;
        }
    }
    return false;
}

bool take(char *arr, int count, char *retn, int len)
{
    if (count <= len && count >= 1)
    {
        for (int i = 0; i < count; ++i)
        {
            retn[i] = arr[i];
        }
        return true;
    }
    return false;
}