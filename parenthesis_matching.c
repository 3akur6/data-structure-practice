#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define ELEMTYPE char //栈内存储元素类型
#define MAXSIZE 256   //栈内最大元素数量
#define LEN 6         //PARENTHESES长度

typedef struct
{
    int top;                //栈顶，表示逻辑位置，0表示栈空
    ELEMTYPE data[MAXSIZE]; //顺序栈，以数组结构存储数据
} Stack;

int match(const char *, int);      //主功能函数，返回值有0、-1、i三种情况
                                   //0表示括号匹配
                                   //-1表示完全匹配后栈不为空，即左括号未完全匹配
                                   //i表示匹配失败位置，右括号未完全匹配
int find(const char *, char, int); //辅助函数，查找并返回字符在字符串中的下标
int stack_init(Stack *);           //初始化栈结构
bool push(Stack *, ELEMTYPE);      //压栈操作
ELEMTYPE pop(Stack *);             //栈顶元素出栈操作

const char PARENTHESES[LEN + 1] = "()[]{}"; //可匹配的括号类型

int main(int argc, char const *argv[])
{
    const char *expression = argv[1]; //从命令行参数获取待判断字符串
    int length = strlen(expression);
    int m = match(expression, length);

    if (!m)
    {
        printf("Balanced.\n");
    }
    else
    {
        printf("Not balanced.\n");
    }
    return 0;
}

int match(const char *exp, int length)
{
    Stack cache;

    stack_init(&cache);

    for (int i = 0; i < length; i++)
    {
        char current = *(exp + i);
        int loc = find(PARENTHESES, current, LEN);

        if (loc)
        {
            if (loc % 2)
            {
                push(&cache, current);
            }
            else
            {
                ELEMTYPE top = pop(&cache);
                if (top != PARENTHESES[loc - 2])
                {
                    return i + 1;
                }
            }
        }
    }
    if (cache.top)
    {
        return -1;
    }
    return 0;
}

int find(const char *s, char c, int length)
{
    for (int i = 0; i < length; i++)
    {
        if (c == *(s + i))
        {
            return (i + 1);
        }
    }
    return 0;
}

int stack_init(Stack *stack)
{
    stack->top = 0;
}

bool push(Stack *stack, ELEMTYPE item)
{
    if (stack->top < MAXSIZE)
    {
        (stack->data)[stack->top] = item;
        (stack->top)++;
        return true;
    }
    return false;
}

ELEMTYPE pop(Stack *stack)
{
    if (stack->top)
    {
        ELEMTYPE rtrn = (stack->data)[stack->top - 1];
        (stack->top)--;
        return rtrn;
    }
}
