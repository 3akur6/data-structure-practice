#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <readline/readline.h>

#define ID_LENGTH 10
#define NAME_SIZE 11
#define MAXSIZE 200
#define CMD 8
#define MAXINPUT 20

typedef struct
{
    char id[ID_LENGTH];  //学号
    char name[NAME_SIZE];//姓名
    int  score;          //分数
} student;

typedef struct
{
    student *item;
    int length;
} student_table; //以学号为主键

//以下int表示逻辑顺序，而非存储顺序
bool init_table(student_table *);
bool table_insert(student_table *, int, student);
bool table_delete(student_table *, int);
bool table_read(student_table *, char *, student *);
bool table_update(student_table *, char *, int);

void show_menu();
void show_student(student *);
void show_table(student_table *);
void terminal_table(char *, student **, int);
void command_exec(char *, student_table *);
char * input(char *);
int to_i(char *);
int * ngets(char *);
void show_banner();

const char *commands[CMD] = {"help", "create", "delete", "update", "read", "table", "quit", "exit"};

int main(void)
{
    student_table table;
    if (!init_table(&table))
    {
        return -1;
    }
    show_banner();
    while (true)
    {
        char *cmd;
        cmd = readline("\033[1;4mcmd\033[0m > ");
        command_exec(cmd, &table);
    }
    return 0;
}

bool init_table(student_table *table)
{
    table->item = (student *) malloc(sizeof(student) * MAXSIZE);
    if (!table->item)
    {
        return false;
    }
    table->length = 0;
    return true;
}

bool table_insert(student_table *table, int location, student item)
{
    if (location >= 1 && location <= MAXSIZE && table->length + 1 <= MAXSIZE)
    {
        int i;
        for (i = table->length - 1; i >= location - 1; --i)
        {
            *(table->item + i + 1) = *(table->item + i);
        }
        *(table->item + location - 1) = item;
        table->length++;
        return true;
    }
    return false;
}

bool table_delete(student_table *table, int location)
{
    if (location >= 1 && location <= table->length)
    {
        for (int i = location; i <= table->length - 1; ++i)
        {
            *(table->item + i - 1) = *(table->item + i);
        }
        table->length--;
        return true;
    }
    return false;
}

bool table_read(student_table *table, char *id, student *retn)
{
    for (int i = 0; i <= table->length - 1; ++i)
    {
        student stud = *(table->item + i);
        if (!strcmp(stud.id, id))
        {
            *retn = stud;
            return true;
        }
    }
    return false;
}

bool table_update(student_table *table, char *id, int scr)
{
    for (int i = 0; i <= table->length - 1; ++i)
    {
        student *stud = table->item + i;
        if (!strcmp(stud->id, id))
        {
            stud->score = scr;
            return true;
        }
    }
    return false;
}

void show_menu()
{
    printf(
        "\n\033[1;34mMenu\033[0m\n"
        "----\n"
        "help\t\tShow this help info\n"
        "create\t\tCreate an item in table\n"
        "delete\t\tDelete an item specified by id\n"
        "read\t\tGet detailed info item contains\n"
        "update\t\tUpdate scores of students\n"
        "table\t\tDisplay all items in table\n"
        "exit|quit\tExit the program\n\n"
    );
}

void show_student(student *stud)
{
    student *list[1] = {stud};
    terminal_table("Student", list, 1);
}

void show_table(student_table *table)
{
    student *list[table->length];
    for (int i = 0; i < table->length; ++i)
    {
        list[i] = table->item + i;
    }
    terminal_table("Student Table", list, table->length);
}

void terminal_table(char *title, student **list, int count)
{
    printf("\n\033[1;34m%s\033[0m\n", title);
    for (int i = 0; i < strlen(title); ++i)
    {
        printf("-");
    }
    printf("\n");
    printf("%s\t\t%s\t%s\n", "Id", "Name", "Score");
    for (int i = 0; i < count; ++i)
    {
        student stud = *list[i];
        printf("\033[32m%s\033[0m\t\033[32m%s\033[0m\t\033[32m%d\033[0m\n", stud.id, stud.name, stud.score);
    }
    printf("\n");
}

void command_exec(char *cmd, student_table *table)
{
    int i;
    for (i = 0; i < CMD; ++i)
    {
        if (!strcmp(commands[i], cmd))
        {
            break;
        }
    }
    switch (i)
    {
        case 1:
        {
            int location = to_i(readline("\n\033[7minsert location\033[0m: "));
            student stud;
            strcpy(stud.id, readline("\033[7mid\033[0m: "));
            strcpy(stud.name, readline("\033[7mname\033[0m: "));
            stud.score = to_i(readline("\033[7mscore\033[0m: "));
            printf("\n");
            table_insert(table, location, stud);
            break;
        }
        case 2:
        {
            int location = to_i(readline("\n\033[7mdelete location\033[0m: "));
            printf("\n");
            table_delete(table, location);
            break;
        }
        case 3:
        {
            char *id = readline("\n\033[7mid\033[0m: ");
            int scr = to_i(readline("\033[7mnew score\033[0m: "));
            printf("\n");
            table_update(table, id, scr);
            break;
        }
        case 4:
        {
            char *id = readline("\n\033[7mid\033[0m: ");
            printf("\n");
            student stud;
            table_read(table, id, &stud);
            show_student(&stud);
            break;
        }
        case 5:
        show_table(table);
        break;
        case 6:
        case 7:
        free(cmd);
        exit(0);
        default:
        show_menu();
    }
}

// char * input(char *prompt)
// {
//     static char get[MAXINPUT];
//     printf("%s", prompt);
//     ngets(get);
//     return get;
// }

int to_i(char *string)
{
    int num = 0;
    for (int i = 0, j = strlen(string) - 1; i < strlen(string) && isdigit(string[i]); ++i, --j)
    {
        num += (string[i] - 48) * pow(10, j);
    }
    return num;
}

// int * ngets(char *get)
// {
//     fgets(get, MAXINPUT, stdin);
//     char *find;
//     while (find = strchr(get, '\n'))
//     {
//         *find = '\0';
//     }
//     return 0;
// }

void show_banner()
{
    const char *banner = 
    "   _____ _             _            _     _______    _     _      ""\n"
    "  / ____| |           | |          | |   |__   __|  | |   | |     ""\n"
    " | (___ | |_ _   _  __| | ___ _ __ | |_     | | __ _| |__ | | ___ ""\n"
    "  \\___ \\| __| | | |/ _` |/ _ \\ '_ \\| __|    | |/ _` | '_ \\| |/ _ \\""\n"
    "  ____) | |_| |_| | (_| |  __/ | | | |_     | | (_| | |_) | |  __/""\n"
    " |_____/ \\__|\\__,_|\\__,_|\\___|_| |_|\\__|    |_|\\__,_|_.__/|_|\\___|""\n"
    "                                                                  ""\n"
    "                                                                  ""\n";
    printf("%s", banner);
}