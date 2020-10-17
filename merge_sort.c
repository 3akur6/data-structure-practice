/*吐槽：学校在线代码审核系统竟然不支持C99标准，数据结构题还要严格控制输出格式→_←*/
#include <stdio.h>
#include <stdbool.h>

#define MAX 50

bool merge_sort(int *, int);
bool merge(int *, int *, int, int);

//自定义比较函数
//返回值为-1（小于），0（等于）, 1（大于）
int compare(int *, int *);

int main(void)
{
  int length;
  int test_list[MAX];

  int i;

  scanf("%d", &length);

  for (i = 0; i < length; i++)
  {
    scanf("%d", test_list + i);
  }

  if (length == 2)
  {
    printf("第1个分裂点为%d\n", test_list[0]);
  }
  else if (length != 1)
  {
    printf("第1个分裂点为%d\n", test_list[length / 2]);
  }

  if (merge_sort(test_list, length))
  {
    printf("排序后的结果为\n");

    for (i = 0; i < length; i++)
    {
      printf("%d ", test_list[i]);
    }
    printf("\n");
  }

  return 0;
}

bool merge_sort(int *list, int length)
{
  int half = length / 2;

  if (length == 1)
  {
    return true;
  }
  else if (length == 2)
  {
    if (compare(list, list + 1) == 1)
    {
      int tmp = *list;
      *list = *(list + 1);
      *(list + 1) = tmp;
    }
  }
  else
  {
    merge_sort(list, half);
    merge_sort(list + half, length - half);
  }

  merge(list, list + half, half, length - half);
  return true;
}

bool merge(int *list1, int *list2, int length1, int length2)
{
  int i = 0;
  int j = 0;
  int k = 0;

  int final[MAX];

  while (i < length1 && j < length2)
  {
    int cmp = compare(&list1[i], &list2[j]);

    if (cmp == -1)
    {
      final[k++] = list1[i++];
    }
    else
    {
      final[k++] = list2[j++];
    }
  }

  while (i < length1)
  {
    final[k++] = list1[i++];
  }

  while (j < length2)
  {
    final[k++] = list2[j++];
  }

  for (i = 0; i < k; i++)
  {
    list1[i] = final[i];
  }

  return true;
}

int compare(int *first, int *last)
{
  return (*first > *last ? 1 : (*first == *last ? 0 : -1));
}