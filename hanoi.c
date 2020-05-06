// features: 解法为hanoi的通解，可指定任意盘子的个数
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ncurses.h>
#include <unistd.h>
#include <ctype.h>

#define DISKS 7
#define MAXSIZE 255
#define WIDTH (DISKS * 2 + 2)

typedef struct
{
    int size; // 盘子的尺寸
    int peg; // 所处的楔子
    int peg_loc; // 所在楔子的位置
} disk;

typedef struct
{
    int num; // 楔子的编号
    int disks; // 楔子上的盘子数
    disk remains[DISKS];
} peg;

typedef struct // 操作
{
    bool status;
    int from; // 移动的起始位置
    int to; // 移动目的位置
    peg pegs[3];
    int obj; // 操作对象
} step;

void hanoi_solve(int, peg *, peg *, peg *, step *, int);
bool hanoi_push(disk, peg *);
disk hanoi_pop(peg *);
bool hanoi_move(peg *, peg *);

bool append(step *, step, int);
disk top(peg *);
char * to_s(int, int);
int to_i(char *string);
void show_text(step *);
void show_animation(step *);
void show_peg(peg *);
void show_step(step *);
char * center_print(char *, int, int);
char * draw_disk(disk *);
char * draw_peg(peg *);
void show_menu();

int main(void)
{
    disk *disk_list;
    peg *peg_list;
    step *steps;
    int count = pow(2, DISKS) - 1;
    disk_list = (disk *) malloc(sizeof(disk) * DISKS);
    peg_list = (peg *) malloc(sizeof(peg) * 3);
    for (int i = 0; i < 3; ++i)
    {
        peg *p = &peg_list[i];
        p->num = i + 1;
        if (i == 0)
        {
            p->disks = DISKS;
        } else {
            p->disks = 0;
        }
    }
    for (int i = 0; i < DISKS; ++i)
    {
        disk *d = &disk_list[i];
        d->size = i + 1;
        d->peg = peg_list[i].num;
        d->peg_loc = i + 1;
        peg_list[0].remains[DISKS - 1 - i] = *d;
    }
    steps = (step *) malloc(sizeof(step) * count);
    hanoi_solve(DISKS, &peg_list[0], &peg_list[1], &peg_list[2], steps, count);

    show_menu();
    char opt[2];
    fgets(opt, 2, stdin);
    switch (to_i(opt))
    {
        case 1:
        {
            show_animation(steps);
            break;
        }
        case 2:
        default:
        show_text(steps);
    }
    return 0;
}

void hanoi_solve(int disk_num, peg *from, peg *to, peg *tra, step *steps, int count)
{
    step s;
    s.status = true;
    s.from = from->num;
    s.to = to->num;
    if (disk_num == 1)
    {
        s.obj = top(from).size;
        hanoi_move(from, to);
        s.pegs[from->num - 1] = *from;
        s.pegs[to->num - 1] = *to;
        s.pegs[tra->num - 1] = *tra;
        append(steps, s, count);
    } else {
        hanoi_solve(disk_num - 1, from, tra, to, steps, count);
        s.obj = top(from).size;
        hanoi_move(from, to);
        s.pegs[from->num - 1] = *from;
        s.pegs[to->num - 1] = *to;
        s.pegs[tra->num - 1] = *tra;
        append(steps, s, count);
        hanoi_solve(disk_num - 1, tra, to, from, steps, count);
    }
}

bool hanoi_push(disk d, peg *to)
{
    if (d.size < top(to).size || !to->disks)
    {
        d.peg = to->num;
        d.peg_loc = 1;
        for (int i = 0; i < to->disks; ++i)
        {
            to->remains[i].peg_loc++;
        }
        to->remains[to->disks] = d;
        to->disks++;
        return true;
    } else {
        return false;
    }
}

disk hanoi_pop(peg *from)
{
    if (from->disks > 0)
    {
        disk retn = from->remains[from->disks - 1];
        from->disks--;
        return retn;
    }
}

bool hanoi_move(peg *from, peg *to)
{
    disk d = hanoi_pop(from);
    if (!hanoi_push(d, to))
    {
        hanoi_push(d, from);
    }
    return true;
}

bool append(step *steps, step s, int count)
{
    for (int i = 0; i < count; ++i)
    {
        if (!steps[i].status)
        {
            steps[i] = s;
            return true;
        }
    }
    return false;
}

// step * concat(step *ss1, step *ss2)
// {
//     int c1 = length(ss1);
//     int c2 = length(ss2);
//     step *ss3 = (step *) malloc(sizeof(step) * (c1 + c2));
//     for (int i = 0; i < c1; ++i)
//     {
//         append(ss3, ss1[i], c1);
//     }
//     for (int i = c1; i < c1 + c2; ++i)
//     {
//         append(ss3, ss1[i], c1 + c2);
//     }
//     return ss3;
// }

// int length(step *steps)
// {
//     int i = 0;
//     while (!steps[i].status)
//     {
//         i++;
//     }
//     return i;
// }

disk top(peg *p)
{
    return p->remains[p->disks - 1];
}

void show_text(step *steps)
{
    for (int i = 0; i < pow(2, DISKS) - 1; ++i)
    {
        step s = steps[i];
        printf("move %d from %d to %d\n", s.obj, s.from, s.to);
    }
}

void show_animation(step *steps)
{
    initscr();
    noecho();
    curs_set(FALSE);

    WINDOW *wins[3];
    for (int i = 0; i < 3; ++i)
    {
        wins[i] = newwin(DISKS + 1, DISKS * 2 + 2, 0, (DISKS * 2 + 2) * i);
    }
    for (int i = 0; i < pow(2, DISKS) - 1; ++i)
    {
        step s = steps[i];
        for (int j = 0; j < 3; ++j)
        {
            mvwprintw(wins[j], 0, 0, draw_peg(&s.pegs[j]));
        }
        for (int k = 0; k < 3; ++k)
        {
            wrefresh(wins[k]);
        }
        sleep(1);
    }
    endwin();
}

void show_peg(peg *p)
{
    printf("num: %d, disks amount: %d, disks: ", p->num, p->disks);
    for (int i = 0; i < p->disks; ++i)
    {
        disk d = p->remains[p->disks - 1 - i];
        printf("%d ", d.size);
    }
    printf("\n");
}

void show_step(step *s)
{
    printf("status: %s, move: %d, from: %d, to: %d\n", s->status ? "true" : "false", s->obj, s->from, s->to);
}

char * draw_disk(disk *d)
{
    if (d->size)
    {
        char *retn;
        retn = (char *) malloc(sizeof(char) * (d->size * 2 + 3));
        retn[0] = '<';
        for (int i = 1; i <= d->size * 2; ++i)
        {
            retn[i] = '#';
        }
        retn[d->size * 2 + 1] = '>';
        retn[d->size * 2 + 2] = '\0';
        return retn;
    }
}

char * draw_peg(peg *p)
{
    char *retn = (char *) malloc(sizeof(char) * MAXSIZE);
    int count;
    if (count = DISKS + 1 - p->disks)
    {
        for (int i = 0; i < count; ++i)
        {
            strcat(retn, center_print("||", 2, WIDTH));
        }
    }
    for (int i = 0; i < p->disks; ++i)
    {
        disk d = p->remains[p->disks - 1 - i];
        strcat(retn, center_print(draw_disk(&d), d.size * 2 + 2, WIDTH));
    }
    strcat(retn, center_print(to_s(p->num, 10), 1, WIDTH));
    return retn;
}

char * center_print(char *sym, int sym_len, int width)
{
    char *retn;
    int half = (width - sym_len) / 2;
    retn = (char *) malloc(sizeof(char) * MAXSIZE);
    for (int i = 0; i < half; ++i)
    {
        retn[i] = ' ';
    }
    retn[half] = '\0';
    strcat(retn, sym);
    strcat(retn, "\n");
    return retn;
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

int to_i(char *string)
{
    int num = 0;
    for (int i = 0, j = strlen(string) - 1; i < strlen(string) && isdigit(string[i]); ++i, --j)
    {
        num += (string[i] - 48) * pow(10, j);
    }
    return num;
}

void show_menu()
{
    printf(
        "Procedures output form\n"
        "1) animation\n"
        "2) text\n"
        "default choice 2)\n"
        "choice: "
    );
}
