#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void unix_error(char *msg);
void runtime_error(char *msg);

void *Malloc(size_t size);
void *Realloc(void *ptr, size_t size);
void *Calloc(size_t nmemb, size_t size);
void Free(void *ptr);

#ifndef ElementType
#define ElementType int
#endif

void unix_error(char *msg)
{
    fprintf(stderr, "%s %s\n", msg, strerror(errno));
    exit(0);
}
void runtime_error(char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(0);
}

void *Malloc(size_t size)
{
    void *p;
    if ((p = malloc(size)) == NULL)
        unix_error("Malloc error");
    return p;
}

void *Realloc(void *ptr, size_t size)
{
    void *p;
    if ((p = realloc(ptr, size)) == NULL)
        unix_error("Realloc error");
    return p;
}

void *Calloc(size_t nmemb, size_t size)
{
    void *p;
    if ((p = calloc(nmemb, size)) == NULL)
        unix_error("Calloc error");
    return p;
}
void Free(void *ptr)
{
    free(ptr);
}

struct Node;
typedef struct Node *PtrToNode;
struct ListRecord;
typedef struct ListRecord *List;
typedef PtrToNode Position;

static void DeleteNode(Position P);
static Position NewNode(ElementType X, Position Up, Position Down, Position Left, Position Right, int Row, int Col);
void DeleteList(List L);
List Init(int Rt, int Ct);
void MakeEmpty(List L);
int IsEmpty(List L);
void InsertLast(ElementType X, int Row, int Col, List L);
ElementType Retrieve(Position P);

void Swap(Position *a, Position *b);
List Reverse(List L);
List Add(List La, List Lb);
List Mul(List La, List Lb);

struct Node
{
    ElementType Element;
    PtrToNode Up, Down, Left, Right;
    int Row, Col;
};

struct ListRecord
{
    PtrToNode Row, Col;
    int Rt, Ct;
    int Size;
};

static void DeleteNode(Position P)
{
    Free(P);
}

static Position NewNode(ElementType X, Position Up, Position Down, Position Left, Position Right, int Row, int Col)
{
    Position P;

    P = (Position)Malloc(sizeof(struct Node));
    P->Element = X;
    P->Up = Up;
    P->Down = Down;
    P->Left = Left;
    P->Right = Right;
    P->Row = Row;
    P->Col = Col;
    return P;
}

void DeleteList(List L)
{
    MakeEmpty(L);
    Free(L);
}

List Init(int Rt, int Ct)
{
    List L;
    L = (List)Malloc(sizeof(struct ListRecord));

    L->Row = (Position)Malloc(sizeof(struct Node) * (Rt + 1));
    L->Col = (Position)Malloc(sizeof(struct Node) * (Ct + 1));
    L->Rt = Rt;
    L->Ct = Ct;
    L->Size = 0;
    for (int i = 1; i <= Rt; i++)
    {
        Position P = &(L->Row[i]);
        P->Down = P->Up = NULL;
        P->Left = P->Right = P;
    }
    for (int i = 1; i <= Ct; i++)
    {
        Position P = &(L->Col[i]);
        P->Left = P->Right = NULL;
        P->Up = P->Down = P;
    }
    return L;
}

void MakeEmpty(List L)
{
    for (int i = 1; i <= L->Rt; i++)
    {
        Position St = &(L->Row[i]);
        Position P, PN = St->Right;
        while ((P = PN) != St)
        {
            PN = P->Right;
            DeleteNode(P);
        }
    }

    for (int i = 1; i <= L->Rt; i++)
    {
        Position P = &(L->Row[i]);
        P->Down = P->Up = NULL;
        P->Left = P->Right = P;
    }
    for (int i = 1; i <= L->Ct; i++)
    {
        Position P = &(L->Col[i]);
        P->Left = P->Right = NULL;
        P->Up = P->Down = P;
    }
    L->Size = 0;
}

int IsEmpty(List L)
{
    return L->Size == 0;
}

void InsertLast(ElementType X, int Row, int Col, List L)
{
    Position P;
    Position Down = &(L->Col[Col]), Up = Down->Up, Right = &(L->Row[Row]), Left = Right->Left;
    P = NewNode(X, Up, Down, Left, Right, Row, Col);
    Up->Down = P;
    Down->Up = P;
    Left->Right = P;
    Right->Left = P;
}

ElementType Retrieve(Position P)
{
    return P->Element;
}

void Swap(Position *a, Position *b)
{
    Position P;

    P = *a;
    *a = *b;
    *b = P;
}

List Reverse(List L)
{
    List LN = Init(L->Ct, L->Rt);
    for (int i = 1; i <= L->Rt; i++)
    {
        Position St = &(L->Row[i]);
        Position P, PN = St->Right;
        while ((P = PN) != St)
        {
            PN = P->Right;
            InsertLast(P->Element, P->Col, P->Row, LN);
        }
    }
    return LN;
    /*
    for (int i = 1; i <= L->Rt; i++)
    {
        Position St = &(L->Row[i]);
        Position P, PN = St->Right;
        while ((P = PN) != St)
        {
            PN = P->Right;
            Swap(&P->Up, &P->Left);
            Swap(&P->Right, &P->Down);
        }
    }
    for (int i = 1; i <= L->Rt; i++)
    {
        Position P = &(L->Row[i]);
        Swap(&P->Up, &P->Left);
        Swap(&P->Right, &P->Down);
    }
    for (int i = 1; i <= L->Ct; i++)
    {
        Position P = &(L->Col[i]);
        Swap(&P->Up, &P->Left);
        Swap(&P->Right, &P->Down);
    }
    Swap(&L->Col, &L->Row);

    int Temp;
    Temp = L->Rt;
    L->Rt = L->Ct;
    L->Ct = Temp;

    return L;
    */
}
List Add(List La, List Lb)
{
    if (La->Ct != Lb->Ct || La->Rt != La->Rt)
        runtime_error("Invalid Martix Add");
    List L = Init(La->Rt, La->Ct);
    for (int i = 1; i <= La->Rt; i++)
    {
        Position Pas = &(La->Row[i]), Pbs = &(Lb->Row[i]);
        Position Pa = Pas->Right, Pb = Pbs->Right;
        while (Pa != Pas && Pb != Pbs)
        {
            if (Pa->Col == Pb->Col)
            {
                int Result = Pa->Element + Pb->Element;
                if (Result != 0)
                    InsertLast(Result, i, Pa->Col, L);
                Pa = Pa->Right;
                Pb = Pb->Right;
            }
            else if (Pa->Col < Pb->Col)
            {
                InsertLast(Pa->Element, i, Pa->Col, L);
                Pa = Pa->Right;
            }
            else
            {
                InsertLast(Pb->Element, i, Pb->Col, L);
                Pb = Pb->Right;
            }
        }
        while (Pa != Pas)
        {
            InsertLast(Pa->Element, i, Pa->Col, L);
            Pa = Pa->Right;
        }
        while (Pb != Pbs)
        {
            InsertLast(Pb->Element, i, Pb->Col, L);
            Pb = Pb->Right;
        }
    }
    return L;
}

List Mul(List La, List Lb)
{
    if (La->Ct != Lb->Rt)
        runtime_error("Invalid Martix Mul");
    List L = Init(La->Rt, Lb->Ct);
    for (int i = 1; i <= La->Rt; i++)
    {
        for (int j = 1; j <= Lb->Ct; j++)
        {
            int Result = 0;
            Position Pas = &(La->Row[i]), Pbs = &(Lb->Col[j]);
            Position Pa = Pas->Right, Pb = Pbs->Down;
            while (Pa != Pas && Pb != Pbs)
            {
                if (Pa->Col == Pb->Row)
                {
                    Result += Pa->Element * Pb->Element;
                    Pa = Pa->Right;
                    Pb = Pb->Down;
                }
                else if (Pa->Col < Pb->Row)
                {
                    Pa = Pa->Right;
                }
                else
                {
                    Pb = Pb->Down;
                }
            }
            if (Result != 0)
                InsertLast(Result, i, j, L);
        }
    }
    return L;
}
int main()
{
    int n, m;
    scanf("%d%d", &n, &m);
    int a, b, c;
    List La = Init(n, m);
    while (scanf("%d %d %d", &a, &b, &c) != EOF)
    {
        if (a == 0 && b == 0 && c == 0)
            break;
        InsertLast(c, a, b, La);
    }
    scanf("%d%d", &n, &m);
    List Lb = Init(n, m);
    while (scanf("%d %d %d", &a, &b, &c) != EOF)
    {
        if (a == 0 && b == 0 && c == 0)
            break;
        InsertLast(c, a, b, Lb);
    }
    List L = Mul(La, Lb);
    for (int i = 1; i <= L->Rt; i++)
    {
        Position St = &(L->Row[i]);
        Position P, PN = St->Right;
        while ((P = PN) != St)
        {
            PN = P->Right;
            printf("%d %d %d\n", P->Row, P->Col, P->Element);
        }
    }
    DeleteList(L);
    DeleteList(La);
    DeleteList(Lb);
    return 0;
}