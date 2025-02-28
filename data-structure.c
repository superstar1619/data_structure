#include "data-structure.h"

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

#ifdef _List_H

void DeleteNode(Position P)
{
    Free(P);
}

Position NewNode(ElementType X, Position PNext)
{
    Position P = (Position)Malloc(sizeof(struct Node));
    P->Element = X;
    P->Next = PNext;
    return P;
}

List Init()
{
    Position P = NewNode(0, NULL);
    return P;
}

List MakeEmpty(List L)
{
    Position P, Tmp;
    P = L->Next;
    while (P != NULL)
    {
        Tmp = P->Next;
        DeleteNode(P);
        P = Tmp;
    }
    return L;
}

int IsEmpty(List L)
{
    return L->Next == NULL;
}

int IsLast(Position P)
{
    return P->Next == NULL;
}

Position Find(ElementType X, List L)
{
    Position P;
    P = L->Next;
    while (P != NULL && P->Element != X)
        P = P->Next;
    return P;
}

void Delete(ElementType X, List L)
{
    Position P, TmpCell;
    P = FindPrevious(X, L);
    if (!IsLast(P))
    {
        TmpCell = P->Next;
        P->Next = TmpCell->Next;
        DeleteNode(TmpCell);
    }
}

Position FindPrevious(ElementType X, List L)
{
    Position P;
    P = L;
    while (P->Next != NULL && P->Next->Element != X)
        P = P->Next;
    return P;
}

void Insert(ElementType X, List L, Position P)
{
    Position TmpCell;
    TmpCell = NewNode(X, P->Next);
    P->Next = TmpCell;
}

void DeleteList(List L)
{
    Position P, Tmp;
    P = L->Next;
    DeleteNode(L);
    while (P != NULL)
    {
        Tmp = P->Next;
        DeleteNode(P);
        P = Tmp;
    }
}

Position Header(List L)
{
    return L;
}

Position First(List L)
{
    return L->Next;
}

Position Advance(Position P)
{
    return P->Next;
}

ElementType Retrieve(Position P)
{
    return P->Element;
}

#endif
#ifdef _Cursor_H

void InitializeCursorSpace(void)
{
    int i;
    for (i = 0; i < SpaceSize; i++)
        CursorSpace[i].Next = i + 1;
    CursorSpace[SpaceSize].Next = 0;
    return;
}

static Position CursorAlloc(void)
{
    Position P;
    P = CursorSpace[0].Next;
    CursorSpace[0].Next = CursorSpace[P].Next;
    if (P == 0)
        runtime_error("Out of space!!!");
    return P;
}

static void CursorFree(Position P)
{
    CursorSpace[P].Next = CursorSpace[0].Next;
    CursorSpace[0].Next = P;
}

void DeleteNode(Position P)
{
    CursorFree(P);
}

Position NewNode(ElementType X, Position PNext)
{
    Position P = CursorAlloc();
    CursorSpace[P].Element = X;
    CursorSpace[P].Next = PNext;
    return P;
}

List MakeEmpty(List L)
{
    Position P, TmpCell;
    P = CursorSpace[L].Next;
    while (P != 0)
    {
        TmpCell = CursorSpace[P].Next;
        DeleteNode(P);
        P = TmpCell;
    }
    return L;
}

int IsEmpty(List L)
{
    return CursorSpace[L].Next == 0;
}

int IsLast(Position P, List L)
{
    return CursorSpace[P].Next == 0;
}

Position Find(ElementType X, List L)
{
    Position P;

    P = CursorSpace[L].Next;
    while (P && CursorSpace[P].Element != X)
        P = CursorSpace[P].Next;
    return P;
}

void Delete(ElementType X, List L)
{
    Position P, TmpCell;
    P = FindPrevious(X, L);
    if (!IsLast(P, L))
    {
        TmpCell = CursorSpace[P].Next;
        CursorSpace[P].Next = CursorSpace[TmpCell].Next;
        DeleteNode(TmpCell);
    }
}

Position FindPrevious(ElementType X, const List L)
{
    Position P;
    P = L;
    while (CursorSpace[P].Next != 0 && CursorSpace[P].Element != X)
        P = CursorSpace[P].Next;
    return P;
}

void Insert(ElementType X, List L, Position P)
{
    Position TmpCell;
    TmpCell = NewNode(X, CursorSpace[P].Next);
    CursorSpace[P].Next = TmpCell;
}

void DeleteList(List L)
{
    Position P, TmpCell;
    P = CursorSpace[L].Next;
    DeleteNode(L);
    while (P != 0)
    {
        TmpCell = CursorSpace[P].Next;
        DeleteNode(P);
        P = TmpCell;
    }
}

Position Header(const List L)
{
    return L;
}

Position First(const List L)
{
    return CursorSpace[L].Next;
}

Position Advance(const Position P)
{
    return CursorSpace[P].Next;
}

ElementType Retrieve(const Position P)
{
    return CursorSpace[P].Element;
}

#endif

#ifdef _Stack_h

PtrToNode NewNode(ElementType X, PtrToNode PNext)
{
    PtrToNode P = (PtrToNode)Malloc(sizeof(struct Node));
    P->Element = X;
    P->Next = PNext;
    return P;
}

void DeleteNode(PtrToNode P)
{
    Free(P);
}

int IsEmpty(Stack S)
{
    return S->Next == NULL;
}

Stack CreateStack(void)
{
    Stack S;
    S = NewNode(0, NULL);
    MakeEmpty(S);
    return S;
}

void DisposeStack(Stack S)
{
    MakeEmpty(S);
    DeleteNode(S);
}

void MakeEmpty(Stack S)
{
    if (S == NULL)
        runtime_error("Must use CreateStack first");
    else
        while (!IsEmpty(S))
            Pop(S);
}

void Push(ElementType X, Stack S)
{
    PtrToNode TmpCell;
    TmpCell = NewNode(X, S->Next);
    S->Next = TmpCell;
    return;
}

ElementType Top(Stack S)
{
    if (!IsEmpty(S))
        return S->Next->Element;
    runtime_error("Empty Stack");
    return 0;
}

void Pop(Stack S)
{
    PtrToNode TmpCell = S->Next;
    if (TmpCell == NULL)
        runtime_error("Empty Stack");
    S->Next = TmpCell->Next;
    DeleteNode(TmpCell);
}

#endif

#ifdef _Stack_H

int IsEmpty(Stack S)
{
    return S->TopOfStack == EmptyTOS;
}

int IsFull(Stack S)
{
    return S->TopOfStack >= S->Capacity - 1;
}

Stack CreateStack(int MaxElements)
{
    Stack S;
    if (MaxElements < MinStackSize)
        runtime_error("Stack size if too small");
    S = Malloc(sizeof(struct StackRecord));
    S->Array = (ElementType *)Malloc(sizeof(ElementType) * MaxElements);
    S->Capacity = MaxElements;
    MakeEmpty(S);
    return S;
}

void DisposetStack(Stack S)
{
    if (S != NULL)
    {
        free(S->Array);
        free(S);
    }
}

void MakeEmpty(Stack S)
{
    S->TopOfStack = EmptyTOS;
}

void Push(ElementType X, Stack S)
{
    if (IsFull(S))
        runtime_error("Full Stack");
    else
        S->Array[++S->TopOfStack] = X;
}

ElementType Top(Stack S)
{
    if (!IsEmpty(S))
        return S->Array[S->TopOfStack];
    runtime_error("Empty Stack");
    return 0;
}

void Pop(Stack S)
{
    if (IsEmpty(S))
        runtime_error("Empty Stack");
    else
        S->TopOfStack--;
}

ElementType TopAndPop(Stack S)
{
    if (!IsEmpty(S))
        return S->Array[S->TopOfStack--];
    runtime_error("Empty Stack");
    return 0;
}

#endif

#ifdef _Queue_h

int IsEmpty(Queue Q)
{
    return Q->Capacity == 0;
}

int IsFull(Queue Q)
{
    return Q->Capacity == Q->Size;
}

Queue CreateQueue(int MaxElements)
{
    Queue Q;
    if (MaxElements < MinQueueSize)
        runtime_error("Queue size if too small");
    Q = (Queue)Malloc(sizeof(struct QueueRecord));
    Q->Array = (ElementType *)Malloc(sizeof(ElementType) * MaxElements);
    Q->Capacity = MaxElements;
    MakeEmpty(Q);
    return Q;
}

void DisposeQueue(Queue Q)
{
    if (Q != NULL)
    {
        Free(Q->Array);
        Free(Q);
    }
}

void MakeEmpty(Queue Q)
{
    Q->Front = 0;
    Q->Rear = -1;
    Q->Size = 0;
    return;
}

static int Succ(int Value, Queue Q)
{
    if (++Value == Q->Capacity)
        Value = 0;
    return Value;
}

void Enqueue(ElementType X, Queue Q)
{
    if (IsFull(Q))
        runtime_error("Full queue");
    else
    {
        Q->Size++;
        Q->Rear = Succ(Q->Rear, Q);
        Q->Array[Q->Rear] = X;
    }
}

ElementType Front(Queue Q)
{
    if (IsEmpty(Q))
        runtime_error("Empty queue");
    return Q->Array[Q->Front];
}

void Dequeue(Queue Q)
{
    if (IsEmpty(Q))
        runtime_error("Empty queue");
    else
    {
        Q->Size--;
        Q->Front = Succ(Q->Front, Q);
    }
}

ElementType FrontAndDequeue(Queue Q)
{
    ElementType X = Front(Q);
    Dequeue(Q);
    return X;
}

#endif

#ifdef _Deque_h

int IsEmpty(Deque D)
{
    return D->Size == 0;
}

int IsFull(Deque D)
{
    return D->Size == D->Capacity;
}

Deque CreateDeque(int MaxElemnts)
{
    Deque D;
    if (MaxElemnts < MinDequeSize)
        runtime_error("Deque size if too small");
    D = (Deque)Malloc(sizeof(struct DequeRecord));
    D->Array = (ElementType *)Malloc(sizeof(ElementType) * MaxElemnts);
    D->Capacity = MaxElemnts;
    MakeEmpty(D);
    return D;
}

void DisposeDeque(Deque D)
{
    if (D != NULL)
    {
        Free(D->Array);
        Free(D);
    }
}

void MakeEmpty(Deque D)
{
    D->Size = 0;
    D->Front = 0;
    D->Rear = -1;
}

int Succ(int Value, Deque D)
{
    if (++Value == D->Capacity)
        Value = 0;
    return Value;
}

int Prev(int Value, Deque D)
{
    if (--Value == -1)
        Value = D->Capacity - 1;
    return Value;
}

void Push(ElementType X, Deque D)
{
    if (IsFull(D))
        runtime_error("Full deque");
    else if (IsEmpty(D))
        Inject(X, D);
    else
    {
        D->Size++;
        D->Front = Prev(D->Front, D);
        D->Array[D->Front] = X;
    }
}

void Pop(Deque D)
{
    if (IsEmpty(D))
        runtime_error("Empty deque");
    else
    {
        D->Size--;
        D->Front = Succ(D->Front, D);
    }
}

void Inject(ElementType X, Deque D)
{
    if (IsFull(D))
        runtime_error("Full deque");
    else
    {
        D->Size++;
        D->Rear = Succ(D->Rear, D);
        D->Array[D->Rear] = X;
    }
}

void Eject(Deque D)
{
    if (IsEmpty(D))
        runtime_error("Empty deque");
    else
    {
        D->Size--;
        D->Rear = Prev(D->Rear, D);
    }
}

ElementType Front(Deque D)
{
    if (IsEmpty(D))
        runtime_error("Empty deque");
    return D->Array[D->Front];
}

ElementType Rear(Deque D)
{
    if (IsEmpty(D))
        runtime_error("Empty deque");
    return D->Array[D->Rear];
}

ElementType FrontAndPop(Deque D)
{
    if (IsEmpty(D))
        runtime_error("Empty deque");
    ElementType X = Front(X);
    Pop(D);
    return X;
}

ElementType RearAndEject(Deque D)
{
    if (IsEmpty(D))
        runtime_error("Empty deque");
    ElementType X = Rear(X);
    Eject(D);
    return X;
}

#endif

#ifdef _Tree_H

SearchTree NewNode(ElementType X, SearchTree Left, SearchTree Right)
{
    SearchTree T = (SearchTree)Malloc(sizeof(struct TreeNode));
    T->Element = X;
    T->Left = Left;
    T->Right = Right;
    return T;
}

void DeleteNode(SearchTree T)
{
    Free(T);
}

SearchTree MakeEmpty(SearchTree T)
{
    if (T != NULL)
    {
        MakeEmpty(T->Left);
        MakeEmpty(T->Right);
        DeleteNode(T);
    }
    return NULL;
}

Position Find(ElementType X, SearchTree T)
{
    if (T == NULL)
        return NULL;
    if (X < T->Element)
        return Find(X, T->Left);
    else if (X > T->Element)
        return Find(X, T->Right);
    else
        return T;
}

Position FindMin(SearchTree T)
{
    if (T == NULL)
        return NULL;
    else if (T->Left == NULL)
        return T;
    else
        return FindMin(T->Left);
}

Position FindMax(SearchTree T)
{
    if (T == NULL)
        return NULL;
    else if (T->Right == NULL)
        return T;
    else
        return FindMax(T->Right);
}

SearchTree Insert(ElementType X, SearchTree T)
{
    if (T == NULL)
        T = NewNode(X, NULL, NULL);
    else if (X < T->Element)
        T->Left = Insert(X, T->Left);
    else if (X > T->Element)
        T->Right = Insert(X, T->Right);
    return T;
}

SearchTree Delete(ElementType X, SearchTree T)
{
    Position TmpCell;

    if (T == NULL)
        runtime_error("Element not found");
    else if (X < T->Element)
        T->Left = Delete(X, T->Left);
    else if (X > T->Element)
        T->Right = Delete(X, T->Right);
    else if (T->Left && T->Right)
    {
        TmpCell = FindMin(T->Right);
        T->Element = TmpCell->Element;
        T->Right = Delete(T->Element, T->Right);
    }
    else
    {
        TmpCell = T;
        if (T->Left == NULL)
            T = T->Right;
        else if (T->Right == NULL)
            T = T->Left;
        DeleteNode(TmpCell);
    }
    return T;
}

ElementType Retrieve(Position P)
{
    return P->Element;
}

Position FindPrev(ElementType X, SearchTree T)
{
    if (T == NULL)
        runtime_error("Empty Tree");
    Position P = NULL, N;
    if (T->Element == X)
        N = T->Left;
    else if (T->Element < X)
        N = T->Right;
    else
        N = T->Left;
    if (N != NULL)
    {
        P = FindPrev(X, N);
    }
    if (T->Element < X)
    {
        if (P == NULL || P->Element < T->Element)
            P = T;
    }
    return P;
}

Position FindNext(ElementType X, SearchTree T)
{
    if (T == NULL)
        runtime_error("Empty Tree");
    Position P = NULL, N;
    if (T->Element == X)
        N = T->Right;
    else if (T->Element < X)
        N = T->Right;
    else
        N = T->Left;
    if (N != NULL)
    {
        P = FindNext(X, N);
    }
    if (T->Element > X)
    {
        if (P == NULL || P->Element > T->Element)
            P = T;
    }
    return P;
}

#endif

#ifndef _Tree_H_NR

SearchTree NewNode(ElementType X, SearchTree Left, SearchTree Right)
{
    SearchTree T = (SearchTree)Malloc(sizeof(struct TreeNode));
    T->Element = X;
    T->Left = Left;
    T->Right = Right;
    return T;
}

void DeleteNode(SearchTree T)
{
    Free(T);
}

void ModifyLine(Position P, LinkLine L)
{
    if (L != NULL)
        *L = P;
}

SearchTree MakeEmpty(SearchTree T)
{
    if (T != NULL)
    {
        MakeEmpty(T->Left);
        MakeEmpty(T->Right);
        DeleteNode(T);
    }
    return NULL;
}

Position Find(ElementType X, SearchTree T)
{
    while (T != NULL)
    {
        if (T->Element > X)
            T = T->Left;
        else if (T->Element < X)
            T = T->Right;
        else
            break;
    }
    return T;
}

Position FindMin(SearchTree T)
{
    if (T != NULL)
        while (T->Left != NULL)
            T = T->Left;
    return T;
}

Position FindMax(SearchTree T)
{
    if (T != NULL)
        while (T->Right != NULL)
            T = T->Right;
    return T;
}

void Insert(ElementType X, LinkLine LT)
{
    LinkLine L = LT;
    SearchTree T = *LT;
    while (T != NULL)
    {
        if (T->Element > X)
        {
            L = &(T->Left);
            T = T->Left;
        }
        else if (T->Element < X)
        {
            L = &(T->Right);
            T = T->Right;
        }
        else
            break;
    }
    if (T == NULL)
    {
        T = NewNode(X, NULL, NULL);
        ModifyLine(T, L);
    }
}

void DeleteSpecialTree(SearchTree T, LinkLine L)
{
    if (T->Left != NULL)
        ModifyLine(T->Left, L);
    else
        ModifyLine(T->Right, L);
    DeleteNode(T);
}

void Delete(ElementType X, LinkLine LT)
{
    SearchTree T = *LT;
    LinkLine L = LT;
    while (T != NULL)
    {
        if (T->Element < X)
        {
            L = &(T->Right);
            T = T->Right;
        }
        else if (T->Element > X)
        {
            L = &(T->Left);
            T = T->Left;
        }
        else
            break;
    }
    if (T == NULL)
        runtime_error("Element not found");
    else
    {
        if (T->Left && T->Right)
        {
            SearchTree TP = T->Right;
            LinkLine LP = &(T->Right);
            while (TP->Left != NULL)
            {
                LP = &(TP->Left);
                TP = TP->Left;
            }
            T->Element = TP->Element;
            DeleteSpecialTree(TP, LP);
        }
        else
            DeleteSpecialTree(T, L);
    }
}

ElementType Retrieve(Position P)
{
    return P->Element;
}

Position FindPrev(ElementType X, SearchTree T)
{
    if (T == NULL)
        runtime_error("Empty Tree");
    Position P = NULL, N;
    while (T != NULL)
    {
        if (T->Element < X)
        {
            if (P == NULL || P->Element < T->Element)
                P = T;
            T = T->Right;
        }
        else if (T->Element > X)
            T = T->Left;
        else
            T = T->Left;
    }
    return P;
}

Position FindNext(ElementType X, SearchTree T)
{
    if (T == NULL)
        runtime_error("Empty Tree");
    Position P = NULL, N;
    while (T != NULL)
    {
        if (T->Element < X)
            T = T->Right;
        else if (T->Element > X)
        {
            if (P == NULL || P->Element > T->Element)
                P = T;
            T = T->Left;
        }
        else
            T = T->Right;
    }
    return P;
}

#endif