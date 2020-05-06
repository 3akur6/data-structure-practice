#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>

#define MAXSIZE 255

typedef struct
{
    int top;
    char elems[MAXSIZE];
} stack;

bool palin_justify(char *);
bool stack_init(stack *);
bool stack_push(stack *, char);
char stack_pop(stack *);

int main(void)
{
    char *get;
    printf("\e[33mPalindrome justify\e[0m\n\n");
    while (
        strcmp(get = readline("Please give a word ('\e[33mq\e[0m' to exit): "), "q")
    )
    {
        printf("\e[32m%s\e[0m palindrome? ", get);
        if (palin_justify(get))
        {
            printf("True\n\n");
        } else {
            printf("False\n\n");
        }
    }
    return 0;
}

bool palin_justify(char *raw)
{
    int half = (strlen(raw) + 1) / 2;
    stack stck;
    stack_init(&stck);
    for (int i = 0; i < half; ++i)
    {
        stack_push(&stck, raw[i]);
    }
    for (int i = !(strlen(raw) % 2) ? half : half - 1; i < strlen(raw); ++i)
    {
        char pp = stack_pop(&stck);
        if (pp != raw[i])
        {
            return false;
        }
    }
    return true;
}

bool stack_init(stack *rtrn)
{
    rtrn->top = -1;
    rtrn = (stack *) malloc(sizeof(stack));
    return true;
}

bool stack_push(stack *stck, char chr)
{
    stck->top++;
    stck->elems[stck->top] = chr;
    return true;
}

char stack_pop(stack *stck)
{
    char rtrn = stck->elems[stck->top];
    stck->top--;
    return rtrn;
}