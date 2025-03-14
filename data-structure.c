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

static void DeleteNode(Position P)
{
    Free(P);
}

static Position NewNode(ElementType X, Position PNext)
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

int IsLast(Position P, List L)
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
    if (!IsLast(P, L))
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
    if (P == NULL)
        return 0;
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

Position CursorAlloc(void)
{
    Position P;
    P = CursorSpace[0].Next;
    CursorSpace[0].Next = CursorSpace[P].Next;
    if (P == 0)
        runtime_error("Out of space!!!");
    return P;
}

void CursorFree(Position P)
{
    CursorSpace[P].Next = CursorSpace[0].Next;
    CursorSpace[0].Next = P;
}

static void DeleteNode(Position P)
{
    CursorFree(P);
}

static Position NewNode(ElementType X, Position PNext)
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

static PtrToNode NewNode(ElementType X, PtrToNode PNext)
{
    PtrToNode P = (PtrToNode)Malloc(sizeof(struct Node));
    P->Element = X;
    P->Next = PNext;
    return P;
}

static void DeleteNode(PtrToNode P)
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
    return Q->Size == 0;
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

#ifdef _Queue_H

PtrToNode NewNode(ElementType X, PtrToNode PNext)
{
    PtrToNode P;
    P = (PtrToNode)Malloc(sizeof(struct Node));
    P->Element = X;
    P->Next = PNext;
    return P;
}

void DeleteNode(PtrToNode P)
{
    Free(P);
}

int IsEmpty(Queue Q)
{
    return Q->Size == 0; // or Q->Front==NULL && Q->Rear==NULL;
}

Queue CreateQueue()
{
    Queue Q;
    Q = (Queue)Malloc(sizeof(struct QueueRecord));
    Q->Front = NULL;
    Q->Rear = NULL;
    Q->Size = 0;
    return Q;
}

void DisposeQueue(Queue Q)
{
    PtrToNode P;
    P = Q->Front;
    while (P != NULL)
    {
        PtrToNode PN = P->Next;
        DeleteNode(P);
        P = PN;
    }
    Free(Q);
}

void MakeEmpty(Queue Q)
{
    PtrToNode P;
    P = Q->Front;
    while (P != NULL)
    {
        PtrToNode PN = P->Next;
        DeleteNode(P);
        P = PN;
    }
    Q->Front = NULL;
    Q->Rear = NULL;
    Q->Size = 0;
}

void Enqueue(ElementType X, Queue Q)
{
    PtrToNode P = NewNode(X, NULL);
    if (IsEmpty(Q))
        Q->Front = Q->Rear = P;
    else
    {
        Q->Rear->Next = P;
        Q->Rear = P;
    }
    Q->Size++;
}

ElementType Front(Queue Q)
{
    if (IsEmpty(Q))
        runtime_error("Empty queue");
    return Q->Front->Element;
}

void Dequeue(Queue Q)
{
    if (IsEmpty(Q))
        runtime_error("Empty queue");
    else
    {
        PtrToNode P;
        Q->Size--;
        P = Q->Front;
        Q->Front = Q->Front->Next;
        DeleteNode(P);
    }
    if (IsEmpty(Q))
        Q->Front = Q->Rear = NULL;
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

static int Succ(int Value, Deque D)
{
    if (++Value == D->Capacity)
        Value = 0;
    return Value;
}

static int Prev(int Value, Deque D)
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
    ElementType X = Front(D);
    Pop(D);
    return X;
}

ElementType RearAndEject(Deque D)
{
    if (IsEmpty(D))
        runtime_error("Empty deque");
    ElementType X = Rear(D);
    Eject(D);
    return X;
}

#endif

#ifdef _Tree_H

static SearchTree NewNode(ElementType X, SearchTree Left, SearchTree Right)
{
    SearchTree T = (SearchTree)Malloc(sizeof(struct TreeNode));
    T->Element = X;
    T->Left = Left;
    T->Right = Right;
    return T;
}

static void DeleteNode(SearchTree T)
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

#ifdef _Tree_H_NR

static SearchTree NewNode(ElementType X, SearchTree Left, SearchTree Right)
{
    SearchTree T = (SearchTree)Malloc(sizeof(struct TreeNode));
    T->Element = X;
    T->Left = Left;
    T->Right = Right;
    return T;
}

static void DeleteNode(SearchTree T)
{
    Free(T);
}

static void ModifyLine(Position P, LinkLine L)
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

static void DeleteSpecialTree(SearchTree T, LinkLine L)
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

#ifdef _AvlTree_H

static AvlTree NewNode(ElementType X, AvlTree Left, AvlTree Right, int Height)
{
    AvlTree T = (AvlTree)Malloc(sizeof(struct AvlNode));
    T->Element = X;
    T->Left = Left;
    T->Right = Right;
    T->Height = Height;
    return T;
}

static void DeleteNode(AvlTree T)
{
    Free(T);
}

static int Max(int ValueA, int ValueB)
{
    if (ValueA > ValueB)
        return ValueA;
    else
        return ValueB;
}

static Position SingleRotateWithLeft(Position K2)
{
    Position K1;

    K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;

    UpdateHeight(K2);
    UpdateHeight(K1);
    return K1;
}

static Position SingleRotateWithRight(Position K1)
{
    Position K2;

    K2 = K1->Right;
    K1->Right = K2->Left;
    K2->Left = K1;

    UpdateHeight(K1);
    UpdateHeight(K2);
    return K2;
}

static Position DoubleRotateWithLeft(Position K3)
{
    K3->Left = SingleRotateWithRight(K3->Left);

    return SingleRotateWithLeft(K3);
}

static Position DoubleRotateWithRight(Position K1)
{
    K1->Right = SingleRotateWithLeft(K1->Right);

    return SingleRotateWithRight(K1);
}

static int GetHeight(Position P)
{
    if (P == NULL)
        return -1;
    else
        return P->Height;
}

static void UpdateHeight(Position P)
{
    if (P != NULL)
        P->Height = Max(GetHeight(P->Left), GetHeight(P->Right)) + 1;
}

static AvlTree TryRotate(AvlTree T)
{
    if (GetHeight(T->Right) - GetHeight(T->Left) == 2)
    {
        if (GetHeight(T->Right->Right) >= GetHeight(T->Right->Left))
            T = SingleRotateWithRight(T);
        else
            T = DoubleRotateWithRight(T);
    }
    else if (GetHeight(T->Left) - GetHeight(T->Right) == 2)
    {
        if (GetHeight(T->Left->Left) >= GetHeight(T->Left->Right))
            T = SingleRotateWithLeft(T);
        else
            T = DoubleRotateWithLeft(T);
    }
    return T;
}

AvlTree MakeEmpty(AvlTree T)
{
    if (T != NULL)
    {
        MakeEmpty(T->Left);
        MakeEmpty(T->Right);
        DeleteNode(T);
    }
    return NULL;
}

Position Find(ElementType X, AvlTree T)
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

Position FindMin(AvlTree T)
{
    if (T == NULL)
        return NULL;
    else if (T->Left == NULL)
        return T;
    else
        return FindMin(T->Left);
}

Position FindMax(AvlTree T)
{
    if (T == NULL)
        return NULL;
    else if (T->Right == NULL)
        return T;
    else
        return FindMax(T->Right);
}

AvlTree Insert(ElementType X, AvlTree T)
{
    if (T == NULL)
    {
        T = NewNode(X, NULL, NULL, 0);
    }
    else if (X < T->Element)
    {
        T->Left = Insert(X, T->Left);
        T = TryRotate(T);
    }
    else if (X > T->Element)
    {
        T->Right = Insert(X, T->Right);
        T = TryRotate(T);
    }
    UpdateHeight(T);
    return T;
}

AvlTree Delete(ElementType X, AvlTree T)
{
    Position TmpCell;
    if (T == NULL)
        runtime_error("Element not found");
    else if (X < T->Element)
    {
        T->Left = Delete(X, T->Left);
        T = TryRotate(T);
    }
    else if (X > T->Element)
    {
        T->Right = Delete(X, T->Right);
        T = TryRotate(T);
    }
    else if (T->Left && T->Right)
    {
        TmpCell = FindMin(T->Right);
        T->Element = TmpCell->Element;
        T->Right = Delete(T->Element, T->Right);
        T = TryRotate(T);
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
    UpdateHeight(T);
    return T;
}

ElementType Retrieve(Position P)
{
    return P->Element;
}

Position FindPrev(ElementType X, AvlTree T)
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

Position FindNext(ElementType X, AvlTree T)
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

#ifdef _HashSep_H

char *ConvertToString(ElementType KeyValue)
{
    char *s = (char *)Malloc(20);
    sprintf(s, "%d", KeyValue);
    return s;
}

Index Hash(ElementType KeyValue, int TableSize)
{
    char *P = ConvertToString(KeyValue);
    const char *Key = P;
    unsigned int HashVal = 0;
    while (*Key != '\0')
        HashVal = ((HashVal << 5) + *Key++) % TableSize;
    Free(P);
    return HashVal;
}

Position NewNode(ElementType X, Position PNext)
{
    Position P;
    P = (Position)Malloc(sizeof(struct ListNode));
    P->Element = X;
    P->Next = PNext;
    return P;
}

void DeleteNode(Position P)
{
    Free(P);
}

static int IsPrime(int Value)
{
    int i, sq = sqrt(Value);
    for (i = 2; i <= sqrt(Value); i++)
        if (Value % i == 0)
            return 0;
    return 1;
}

static int NextPrime(int Value)
{
    while (!IsPrime(Value))
        Value++;
    return Value;
}

HashTable InitializeTable(int TableSize)
{
    HashTable H;
    int i;

    if (TableSize < MinTableSize)
    {
        runtime_error("Table size too small");
        return NULL;
    }

    H = (HashTable)Malloc(sizeof(struct HashTbl));

    H->TableSize = NextPrime(TableSize);
    H->TheLists = (List *)Malloc(sizeof(List) * H->TableSize);

    int i;
    for (i = 0; i < H->TableSize; i++)
        H->TheLists[i] = NULL;
    return H;
}

Position Find(ElementType Key, HashTable H)
{
    Position P;
    P = H->TheLists[Hash(Key, H->TableSize)];
    while (P != NULL && P->Element != Key)
        P = P->Next;
    return P;
}

void Insert(ElementType Key, HashTable H)
{
    Position Pos, NewCell;
    List *L;

    Pos = Find(Key, H);
    if (Pos == NULL)
    {
        L = &(H->TheLists[Hash(Key, H->TableSize)]);
        NewCell = NewNode(Key, *L);
        *L = NewCell;
    }
}

void DestroyList(List L)
{
    Position P;
    while (L)
    {
        P = L;
        L = L->Next;
        DeleteNode(P);
    }
}

void DestroyTable(HashTable H)
{
    int i;
    for (i = 0; i < H->TableSize; i++)
    {
        DestroyList(H->TheLists[i]);
    }
    Free(H->TheLists);
    Free(H);
}

ElementType Retrieve(Position P)
{
    return P->Element;
}

#endif

#ifdef _HashQuad_H

char *ConvertToString(ElementType KeyValue)
{
    char *s = (char *)Malloc(20);
    sprintf(s, "%d", KeyValue);
    return s;
}

Index Hash(ElementType KeyValue, int TableSize)
{
    char *P = ConvertToString(KeyValue);
    const char *Key = P;
    unsigned int HashVal = 0;
    while (*Key != '\0')
        HashVal = ((HashVal << 5) + *Key++) % TableSize;
    Free(P);
    return HashVal;
}

static int IsPrime(int Value)
{
    int i, sq = sqrt(Value);
    for (i = 2; i <= sqrt(Value); i++)
        if (Value % i == 0)
            return 0;
    return 1;
}

static int NextPrime(int Value)
{
    while (!IsPrime(Value))
        Value++;
    return Value;
}

HashTable InitializeTable(int TableSize)
{
    HashTable H;
    int i;

    if (TableSize < MinTableSize)
    {
        runtime_error("Table size too small");
        return NULL;
    }

    H = (HashTable)Malloc(sizeof(struct HashTbl));

    H->TableSize = NextPrime(TableSize);

    H->TheCells = (Cell *)Malloc(sizeof(Cell) * H->TableSize);

    for (int i = 0; i < H->TableSize; i++)
        H->TheCells[i].Info = Empty;

    return H;
}

void DestroyTable(HashTable H)
{
    if (H == NULL)
        runtime_error("Destroy Unknown");
    Free(H->TheCells);
    Free(H);
}

Position Find(ElementType Key, HashTable H)
{
    Position CurrentPos;
    int CollisionNum;

    CollisionNum = 0;
    CurrentPos = Hash(Key, H->TableSize);
    while (H->TheCells[CurrentPos].Info != Empty && H->TheCells[CurrentPos].Element != Key)
    {
        CurrentPos += 2 * ++CollisionNum - 1;
        CurrentPos = CurrentPos % H->TableSize;
    }
    return CurrentPos;
}

void Insert(ElementType Key, HashTable H)
{
    Position Pos;
    Pos = Find(Key, H);
    if (H->TheCells[Pos].Info != Legitimate)
    {
        H->TheCells[Pos].Info = Legitimate;
        H->TheCells[Pos].Element = Key;
    }
}

ElementType Retrieve(Position P, HashTable H)
{
    return H->TheCells[P].Element;
}

HashTable Rehash(HashTable H)
{
    int i, OldSize;
    Cell *OldCells;
    HashTable OldH;

    OldH = H;
    OldCells = H->TheCells;
    OldSize = H->TableSize;

    H = InitializeTable(2 * OldSize);

    for (int i = 0; i < OldSize; i++)
        if (OldCells[i].Info == Legitimate)
            Insert(OldCells[i].Element, H);

    DestroyTable(OldH);
    return H;
}

#endif

#ifndef _BinHeap_H

PriorityQueue Initialize(int MaxElements)
{
    PriorityQueue H;

    if (MaxElements < MinPQSize)
        runtime_error("Priority queue size is too small");

    H = (PriorityQueue)Malloc(sizeof(struct HeapStruct));

    H->Elements = (ElementType *)Malloc(sizeof(ElementType) * MaxElements);
    H->Capacity = MaxElements;
    H->Size = 0;
    H->Elements[0] = MinData;

    return H;
}

void Destroy(PriorityQueue H)
{
    Free(H->Elements);
    Free(H);
}

void MakeEmpty(PriorityQueue H)
{
    H->Size = 0;
}

void Insert(ElementType X, PriorityQueue H)
{
    int i;

    if (IsFull(H))
        runtime_error("Priority queue is full");

    for (i = ++H->Size; H->Elements[i / 2] > X; i /= 2)
        H->Elements[i] = H->Elements[i / 2];
    H->Elements[i] = X;
}

ElementType DeleteMin(PriorityQueue H)
{
    int i, Child;
    ElementType MinElement, LastElement;

    if (IsEmpty(H))
        runtime_error("Priority queue is empty");

    MinElement = H->Elements[1];
    LastElement = H->Elements[H->Size--];

    for (i = 1; i * 2 <= H->Size; i = Child)
    {
        if (Child != H->Size && H->Elements[Child + 1] < H->Elements[Child])
            Child++;

        if (LastElement > H->Elements[Child])
            H->Elements[i] = H->Elements[Child];
        else
            break;
    }
    H->Elements[i] = LastElement;
    return MinElement;
}

int IsEmpty(PriorityQueue H)
{
    return H->Size == 0;
}

int IsFull(PriorityQueue H)
{
    return H->Size == H->Capacity;
}

#endif