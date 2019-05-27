#include "stdio.h"
#include"stdlib.h"
#include"time.h"
#define R 5000    //物理内存块数
#define V 10000   //虚拟内存块数

/***************FIFO数据结构***************/

typedef struct node1
{
    int num;
    node1* next;
} Node1, *pNode1;

typedef struct queue
{
    int n;
    pNode1 front;
    pNode1 rear;

} Queue, *pQueue;

void initQueue (pQueue q);
void addQueue (pQueue q, int num);
void removeMemory (pQueue q);
void destroy (pQueue q);
bool searchQ (pQueue q, int num);
void FIFO (pQueue q, int num);
void testFIFO();
/****************改进型Clock数据结构********************/

struct LNode    //改进型Clock置换算法用到的数据结构
{
    int data;
    int flag;//访问位
    int modify;//修改位
};
LNode* nodes;

/*************PBA数据结构****************/
struct LNode2
{
    int data;
    int flag;//访问位
    int modify;//修改位
    LNode2* next;
};
struct Link
{
    int num;//当前链表上的结点数
    LNode2* next;
};
LNode2* nodes2;
Link link1;
Link link2;
/*****************************************/

void initMemory();
void generate();//生成访问序列
bool inMemory (int n); //
void optimal (int n); //
void testOptimal();
void LRU (int n);
void testLRU();
void updated_Clock (int n);
void testClock();
void initialPBA();
bool inNodes (int n); //
void add (int data, int type);
void trans1();
void trans2();
void PBA (int n);

int block = 3;
int size = 3;
int access[32]; //访问序列
int* memo;
int lost = 0;//没找到的页面数
int index = 0;//指示当前下标
int p;//工作集的起始位置


int main (int argc, char* argv[])
{
    generate();
    testOptimal();
    testFIFO();
    testLRU();
    testClock();
    initialPBA();
    int i = 0;

    for (; i < 32; i++)
    {
        printf ("%d, ", access[i]);
    }
    return 0;
}

void generate()
{
    srand ( (unsigned) time (NULL)); //用时间做种，每次产生随机数不一样
    int p = rand() % 64;
    int m = 8, e = 8;
    int i, j;
    double t;
    t = rand() % 10 / 10.0;

    for (i = 0; i < 4; i++)
    {
        for (j = i * m; j < (i + 1) *m; j++)
        {
            access[j] = (p + rand() % e) % 64;
        }

        double r = (rand() % 10) / 10.0;

        if (r < t)
        {
            p = rand() % 64;
        }

        else
        {
            p = (p + 1) % 64;
        }
    }
}

void initQueue (pQueue q)   //初始化队列
{
    q->rear = (pNode1) malloc (sizeof (Node1));

    if (q->rear == NULL)
    {
        printf ("failed\n");
    }

    else
    {
        q->front = q->rear;
        q->rear->next = NULL;
        q->front->next = NULL;
        q->n = 0;
    }
}

void addQueue (pQueue q, int num)   //队列中加入新的页面结点
{
    pNode1 p = (pNode1) malloc (sizeof (Node1));

    if (p == NULL)
    {
        printf ("failed");
    }

    else
    {
        p->next = NULL;
        p->num = num;

        if (q->front == q->rear)
        {
            q->front->next = p;
            q->rear = p;
        }

        else
        {
            q->rear->next = p;
            q->rear = p;
        }

        q->n++;
    }
}

void removeMemory (pQueue q) //将页面移出内存
{
    pNode1 p;

    if (q->front != q->rear)
    {
        p = q->front->next;
        q->front->next = p->next;

        if (p == q->rear)
        {
            q->front = q->rear;
        }

        q->n--;
        free (p);
    }
}

void destroy (pQueue q)     //销毁队列
{
    while (q->front != q->rear)
    {
        removeMemory (q);
    }
}


bool searchQ (pQueue q, int num)       //查找页面是否已经调入内存
{
    pNode1 p;

    if (q->front != q->rear)
    {
        p = q->front->next;

        while (p)
        {
            if (p->num == num)
            {
                return true;
            }

            else
            {
                p = p->next;
            }
        }
    }

    return false;
}

void testFIFO()
{
    Queue q;
    pNode1 p;
    initQueue (&q);
    int i = 0;
    printf ("先进先出置换算法\n");

    for (; i < 32; i++)
    {
        FIFO (&q, access[i]);
        p = q.front->next;

        while (p)
        {
            printf ("%d ", p->num);
            p = p->next;
        }

        printf ("\n");
    }
    printf("***************************************************\n");
    printf ("先进先出算法\n");
    printf("页面置换次数为%d，缺页率：%f\n", lost,lost / 32.0);
    printf("***************************************************\n");
    destroy (&q);
    lost = 0;
    index = 0;
}

void FIFO (pQueue q, int num)
{
    if (searchQ (q, num))
    {
        printf ("已装入内存\n");
    }

    else
    {
        if (q->n == size)
        {
            removeMemory (q);
            addQueue (q, num);
            lost++;
        }

        else
        {
            addQueue (q, num);
        }
    }
}

void initMemory()
{
    memo = (int*) malloc (block * sizeof (int));
    int i = 0;

    for (; i < block; i++)
    {
        memo[i] = -1;
    }

    return;
}

void testOptimal()
{
    initMemory();
    int i = 0;
    printf ("最佳置换算法：\n");

    for (; i < 32; i++)
    {
        optimal (i);
        printf ("%d %d %d\n", memo[0], memo[1], memo[2]);
    }
    printf("***************************************************\n");
    printf ("最佳置换算法:\n");
    printf("页面置换次数：%d ,缺页率：%2f \n", lost,lost / 32.0);
    printf("***************************************************\n");
    lost = 0;
    free (memo);
    index = 0;
}

bool  inMemory (int n)
{
    int i = 0;

    for (; i < block; i++)
    {
        if (access[n] == memo[i])
        {
            return true;
        }
    }

    return false;
}

//最佳适应算法
void optimal (int n)
{
    int i = 0, j = 0;

    if (inMemory (n))
    {
        printf ("页面已被调入\n");
    }

    else
        if (index == block)
        {
            lost++;
            int max = 0, pos, tag;

            for (i = 0; i < block; i++)
            {
                tag = -1;

                for (j = n + 1; j < 32; j++)
                {
                    if (access[j] == memo[i])
                    {
                        tag = j;
                        break;
                    }
                }

                if (tag == -1)
                {
                    max = 32;
                    pos = i;
                    break;
                }

                else
                {
                    if (max < tag)
                    {
                        max = tag;
                        pos = i;
                    }
                }
            }

            memo[pos] = access[n];
        }

        else
        {
            memo[index] = access[n];
            index++;
        }
}


////////LRU算法
void LRU (int n)
{
    int i, j;

    if (inMemory (n))
    {
        printf ("已经装入内存\n");
    }

    else
        if (index == block)
        {
            int max = n, pos = -1, tag;

            for (i = 0; i < block; i++)
            {
                for (j = n - 1; j >= 0; j--)
                {
                    if (access[j] == memo[i])
                    {
                        tag = j;
                        break;
                    }
                }

                if (tag < max)
                {
                    max = tag;
                    pos = i;

                    if (max == 0)
                    {
                        break;
                    }
                }
            }

            memo[pos] = access[n];
            lost++;
        }

        else
        {
            memo[index] = access[n];
            index++;
        }
}

void testLRU()
{
    int i;
    initMemory();
    printf ("最近最久未使用算法\n");

    for (i = 0; i < 32; i++)
    {
        LRU (i);
        printf ("%d %d %d\n", memo[0], memo[1], memo[2]);
    }
    printf("***************************************************\n");
    printf ("最近最久未使用算法:\n");
    printf("置换页面次数为: %d, 缺页率： %2f \n", lost, lost / 32.0);
    printf("***************************************************\n");
    lost = 0;
    index = 0;
}

bool isInNodes (int n)
{
    int i;

    for (i = 0; i < block; i++)
    {
        if (nodes[i].data == access[n])
        {
            return true;
        }
    }

    return false;
}

void updated_Clock (int n)
{
    if (isInNodes (n))
    {
        printf ("已经装入内存\n");
    }

    else
        if (index == block)
        {
            lost++;
            int i = 0, tag = -1;

            while (true)
            {
                if ( (i / block) % 2 == 0)
                {
                    if (nodes[i % block].flag == 0 && nodes[i % block].modify == 0)
                    {
                        tag = i % block;
                        break;
                    }
                }

                if ( (i / block) % 2 == 1)
                {
                    if (nodes[i % block].flag == 0 && nodes[i % block].modify == 1)
                    {
                        tag = i % block;
                        break;
                    }

                    else
                    {
                        nodes[i % block].flag = 0;
                    }
                }

                i++;
            }

            nodes[tag].data = access[n];
            nodes[tag].flag = 1;

            if (rand() % 10 < 4)
            {
                nodes[tag].modify = 1;
            }

            else
            {
                nodes[tag].modify = 0;
            }
        }

        else
        {
            nodes[index].data = access[n];
            nodes[index].flag = 1;

            if (rand() % 10 < 4)
            {
                nodes[index].modify = 1;
            }

            else
            {
                nodes[index].modify = 0;
            }

            index++;
        }
}
void testClock()
{
    int i = 0, j = 0;
    printf ("改进型Clock置换算法\n");
    nodes = (LNode*) malloc (block * sizeof (LNode));

    for (i = 0; i < block; i++)
    {
        nodes[i].data = -1;
        nodes[i].flag = -1;
        nodes[i].modify = -1;
    }

    for (i = 0; i < 32; i++)
    {
        updated_Clock (i);

        for (j = 0; j < block; j++)
        {
            printf ("%d ", nodes[j].data);
        }

        printf ("\n");
    }
    printf("***************************************************\n");
    printf ("改进型Clock置换算法: \n");
    printf("置换页面次数为: %d , 缺页率： %2f  \n",lost ,lost / 32.0);
    printf("***************************************************\n");
    lost = 0;
    index = 0;
}


void initialPBA()
{
    int i = 0, j = 0;
    generate();
    printf ("页面缓冲置换算法PBA\n");
    link1.num = 0;
    link1.next = NULL;
    link2.num = 0;
    link2.next = NULL;
    nodes2 = (LNode2*) malloc (size * sizeof (LNode2));

    for (i = 0; i < size; i++)
    {
        nodes2[i].data = -1;
        nodes2[i].flag = 0;
        nodes2[i].modify = 0;
        nodes2[i].next = NULL;
    }

    for (i = 0; i < 32; i++)
    {
        PBA (i);

        for (j = 0; j < size; j++)
        {
            printf ("%d ", nodes2[j].data);
        }

        printf ("\n");
    }
    printf("***************************************************\n");
    printf("页面缓冲置换算法PBA:\n");
    printf ("置换页面次数为：%d , 缺页率：%f \n",  lost, lost / 32.0);
    printf("***************************************************\n");
    lost = 0;
    index = 0;
}


bool inNodes (int n)
{
    int i;

    for (i = 0; i < 3; i++)
    {
        if (nodes2[i].data == access[n])
        {
            return true;
        }
    }

    return false;
}




LNode2* isinLinks (int n)
{
    LNode2*p, *q;
    p = link1.next;
    q = NULL;

    while (p)
    {
        if (p->data == access[n])
        {
            if (q != NULL)
            {
                q->next = p->next;
                p->next = NULL;
                link1.num--;
                break;
            }

            else
            {
                link1.next = NULL;
            }
        }

        q = p;
        p = p->next;
    }

    if (p == NULL)
    {
        p = link2.next;

        while (p != NULL)
        {
            if (p->data == access[n])
            {
                if (p == link2.next)
                { link2.next = p->next; }

                else
                {
                    q->next = p->next;
                    p->next = NULL;
                    link2.num--;
                }

                if (link2.num == 0)
                { link2.next = NULL; }

                break;
            }

            q = p;
            p = p->next;
        }
    }

    return p;
}


void PBA (int n)
{
    if (inNodes (n))
    {
        printf ("已装入内存\n");
    }

    else
        if (index == size)
        {
            LNode2 *p;

            if ( (p = isinLinks (n)) != NULL)
            {
                nodes2 = (LNode2*) realloc (nodes2, (size + 1) * sizeof (LNode2));
                nodes2[size] .data = p->data;
                nodes2[size].flag = p->flag;
                nodes2[size].modify = p->modify;
                nodes2[size].next = p->next;
                free (p);
                size++;
                index++;
            }

            else
            {
                lost++;//缺页

                if (nodes2[n % 3].modify == 1)
                {
                    add (nodes2[n % 3].data, 1);
                }

                else
                {
                    add (nodes2[n % 3].data, 0);
                }

                nodes2[n % 3].data = access[n];
                nodes2[n % 3].flag = 1;
                nodes2[n % 3].next = NULL;

                if (rand() % 10 < 4)
                {
                    nodes2[n % 3].modify = 0;
                }

                else
                {
                    nodes2[n % 3].modify = 1;
                }
            }
        }

        else
        {
            nodes2[index].data = access[n];
            nodes2[index].flag = 1;
            nodes2[index].next = NULL;

            if (rand() % 10 < 4)
            {
                nodes2[index].modify = 1;
            }

            else
            {
                nodes2[index].modify = 0;
            }

            index++;
        }
}

void add (int data, int type) //页面添加到已修改页面链表和空闲链表上
{
    LNode2* p;
    LNode2* q;
    q = (LNode2*) malloc (sizeof (LNode2));
    q->data = data;
    q->flag = 1;

    if (type == 1)
    {
        q->modify = 1;
        p = link2.next;
    }

    else
    {
        q->modify = 0;
        p = link1.next;
    }

    q->next = NULL;

    if (p == NULL)
    {
        if (type == 0)
        {
            link1.next = q;
        }

        else
        {
            link2.next = q;
        }
    }

    else
    {
        while (p)
        {
            if (p->next == NULL)
            {
                p->next = q;
                break;
            }

            else
            {
                p = p->next;
            }
        }
    }

    if (type == 0)
    {
        link1.num += 1;

        if (link1.num == 10)
        {
            trans1();
        }
    }

    else
    {
        link2.num += 1;

        if (link2.num == 10)
        {
            trans2();
        }
    }
}
void trans1()   //将空闲链表上的所有页面送出内存
{
    LNode2* p;
    p = link1.next;

    while (p)
    {
        link1.next = p->next;
        free (p);
        p = link1.next;
    }

    link1.num = 0;
}
void trans2()    //将已修改页面链表上所有的链表送出内存
{
    LNode2* p;
    p = link2.next;

    while (p)
    {
        link2.next = p->next;
        free (p);
        p = link2.next;
    }

    link2.num = 0;
}

