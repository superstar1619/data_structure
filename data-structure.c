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

#ifdef _DoubleList_H

static void DeleteNode(Position P)
{
    Free(P);
}

static Position NewNode(ElementType X, Position PPrev, Position PNext)
{
    Position P;
    P = (Position)Malloc(sizeof(struct Node));
    P->Element = X;
    P->Next = PNext;
    P->Prev = PPrev;
    return P;
}

List Init()
{
    List L;
    L = (List)Malloc(sizeof(struct ListRecord));
    L->Head = L->Tail = NewNode(0, NULL, NULL);
    return L;
}

int IsEmpty(List L)
{
    return L->Head == L->Tail;
}

int IsLast(Position P, List L)
{
    return P == L->Tail;
}

Position Find(ElementType X, List L)
{
    Position P = L->Head->Next;
    while (P != NULL && P->Element != X)
        P = P->Next;
    return P;
}

void Delete(ElementType X, List L)
{
    Position P = Find(X, L);
    if (P == NULL)
        runtime_error("Can not find the element in the list");
    Position PP, PN;
    PP = Prev(P);
    PN = Next(P);
    PP->Next = PN;
    if (PN != NULL)
        PN->Prev = PP;
    DeleteNode(P);
}

Position FindPrevious(ElementType X, List L)
{
    return Prev(Find(X, L));
}

void Insert(ElementType X, List L, Position P)
{
    Position PP = Prev(P);
    PP->Next = NewNode(X, PP, P);
    P->Prev = PP->Next;
}

void DeleteList(List L)
{
    Position P = L->Head;
    while (P != NULL)
    {
        Position PN = P->Next;
        DeleteNode(P);
        P = PN;
    }
    Free(L);
}

Position Header(List L)
{
    return L->Head;
}

Position First(List L)
{
    return L->Head->Next;
}

Position Prev(Position P)
{
    return P->Prev;
}

Position Next(Position P)
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

void DisposeStack(Stack S)
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

#ifdef _BinHeap_H

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

ElementType FindMin(PriorityQueue H)
{
    if (IsEmpty(H))
        runtime_error("Priority queue is empty");
    return H->Elements[1];
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

#ifdef _LeftHeap_H

static PriorityQueue NewNode(ElementType X, PriorityQueue Left, PriorityQueue Right, int Npl)
{
    PriorityQueue H;
    H = (PriorityQueue)Malloc(sizeof(struct TreeNode));
    H->Element = X;
    H->Left = Left;
    H->Right = Right;
    H->Npl = Npl;
    return H;
}

void DeleteNode(PriorityQueue H)
{
    Free(H);
}

static void SwapChild(PriorityQueue H)
{
    PriorityQueue TmpCell;
    TmpCell = H->Left;
    H->Left = H->Right;
    H->Right = TmpCell;
}

PriorityQueue Initialize(void)
{
    return NULL;
}

ElementType FindMin(PriorityQueue H)
{
    return H->Element;
}

int IsEmpty(PriorityQueue H)
{
    return H == NULL;
}

PriorityQueue Merge(PriorityQueue H1, PriorityQueue H2)
{
    if (H1 == NULL)
        return H2;
    if (H2 == NULL)
        return H1;
    if (H1->Element < H2->Element)
        return Merge1(H1, H2);
    else
        return Merge1(H2, H1);
}

static PriorityQueue Merge1(PriorityQueue H1, PriorityQueue H2)
{
    if (H1->Left == NULL)
        H1->Left = H2;
    else
    {
        H1->Right = Merge(H1->Right, H2);
        if (H1->Left->Npl < H1->Right->Npl)
            SwapChild(H1);
        H1->Npl = H1->Right->Npl + 1;
    }
    return H1;
}

PriorityQueue Insert1(ElementType X, PriorityQueue H)
{
    PriorityQueue SingleNode;

    SingleNode = NewNode(X, NULL, NULL, 0);
    H = Merge(SingleNode, H);
    return H;
}

PriorityQueue DeleteMin1(PriorityQueue H)
{
    PriorityQueue LeftHeap, RightHeap;

    if (IsEmpty(H))
        runtime_error("Priority queue is empty");

    LeftHeap = H->Left;
    RightHeap = H->Right;
    DeleteNode(H);
    return Merge(LeftHeap, RightHeap);
}

#endif

#ifdef _Binomial_H

BinQueue Initialize(void)
{
    BinQueue H;

    H = (BinQueue)Malloc(sizeof(struct Collection));

    MakeEmpty(H);

    return H;
}

BinTree NewNode(ElementType X, Position LeftChild, Position NextSibling)
{
    BinTree T;
    T = (BinTree)Malloc(sizeof(struct BinNode));
    T->Element = X;
    T->LeftChild = LeftChild;
    T->NextSibling = NextSibling;
    return T;
}

void DeleteNode(BinTree T)
{
    Free(T);
}

void DestroyBinTree(BinTree T)
{
    if (T == NULL)
        return;
    DestroyBinTree(T->LeftChild);
    DestroyBinTree(T->NextSibling);
    DeleteNode(T);
}

void Destroy(BinQueue H)
{
    int i, j;
    for (i = 0, j = 1; j < H->CurrentSize; i++, j *= 2)
        DestroyBinTree(H->TheTrees[i]);
    Free(H);
}

void MakeEmpty(BinQueue H)
{
    int i = 0;
    H->CurrentSize = 0;
    for (i = 0; i < MaxTrees; i++)
        H->TheTrees[i] = NULL;
}

BinTree CombineTrees(BinTree T1, BinTree T2)
{
    if (T1->Element > T2->Element)
        return CombineTrees(T2, T1);
    T2->NextSibling = T1->LeftChild;
    T1->LeftChild = T2;
    return T1;
}

BinQueue Merge(BinQueue H1, BinQueue H2)
{
    BinTree T1, T2, Carry = NULL;
    int i, j;
    if (H1->CurrentSize + H2->CurrentSize > Capacity)
        runtime_error("Merge would exceed capacity");

    H1->CurrentSize += H2->CurrentSize;
    for (i = 0, j = 1; j <= H1->CurrentSize; i++, j *= 2)
    {
        T1 = H1->TheTrees[i];
        T2 = H2->TheTrees[i];
        switch (!!T1 + 2 * !!T2 + 4 * !!Carry)
        {
        case 0:
        case 1:
            break;
        case 2:
            H1->TheTrees[i] = T2;
            H2->TheTrees[i] = NULL;
            break;
        case 3:
            Carry = CombineTrees(T1, T2);
            H1->TheTrees[i] = H2->TheTrees[i] = NULL;
            break;
        case 4:
            H1->TheTrees[i] = Carry;
            Carry = NULL;
            break;
        case 5:
            Carry = CombineTrees(T1, Carry);
            H1->TheTrees[i] = NULL;
            break;
        case 6:
            Carry = CombineTrees(T2, Carry);
            H2->TheTrees[i] = NULL;
            break;
        case 7:
            H1->TheTrees[i] = Carry;
            Carry = CombineTrees(T1, T2);
            H2->TheTrees[i] = NULL;
            break;
        }
    }
    Destroy(H2);
    return H1;
}

BinQueue Insert(ElementType Item, BinQueue H)
{
    BinQueue NewH;
    NewH = Initialize();
    NewH->CurrentSize = 1;
    NewH->TheTrees[0] = NewNode(Item, NULL, NULL);
    return Merge(H, NewH);
}

ElementType DeleteMin(BinQueue H)
{
    int i, j;
    int MinTree;
    BinQueue DeletedQueue;
    Position DeletedTree, OldRoot;
    ElementType MinItem;

    if (IsEmpty(H))
    {
        runtime_error("Empty binomial queue");
        // return -Infinity;
    }

    MinItem = Infinity;
    for (i = 0, j = 1; j <= H->CurrentSize; i++, j *= 2)
    {
        if (H->TheTrees[i] && H->TheTrees[i]->Element < MinItem)
        {
            MinItem = H->TheTrees[i]->Element;
            MinTree = i;
        }
    }

    DeletedTree = H->TheTrees[MinTree];
    OldRoot = DeletedTree;
    DeletedTree = DeletedTree->LeftChild;
    DeleteNode(OldRoot);

    DeletedQueue = Initialize();
    DeletedQueue->CurrentSize = (1 << MinTree) - 1;
    for (j = MinTree - 1; j >= 0; j--)
    {
        DeletedQueue->TheTrees[j] = DeletedTree;
        DeletedTree = DeletedTree->NextSibling;
        DeletedQueue->TheTrees[j]->NextSibling = NULL;
    }

    H->TheTrees[MinTree] = NULL;
    H->CurrentSize -= DeletedQueue->CurrentSize + 1;

    Merge(H, DeletedQueue);
    return MinItem;
}

ElementType FindMin(BinQueue H)
{
    int i, j;
    ElementType MinItem = Infinity;
    for (i = 0, j = 1; j <= H->CurrentSize; i++, j *= 2)
    {
        if (H->TheTrees[i] < MinItem)
            MinItem = H->TheTrees[i];
    }
    return MinItem;
}

int IsEmpty(BinQueue H)
{
    return H->CurrentSize == 0;
}

int IsFull(BinQueue H)
{
    return H->CurrentSize == Capacity;
}

#endif

#ifdef _Sort_h

void Swap(ElementType *a, ElementType *b)
{
    ElementType Tmp;
    Tmp = *a;
    *a = *b;
    *b = Tmp;
}

void Insertionsort(ElementType A[], int N)
{
    int j, P;
    ElementType Tmp;
    for (P = 1; P < N; P++)
    {
        Tmp = A[P];
        for (j = P; j > 0 && A[j - 1] > Tmp; j--)
            A[j] = A[j - 1];
        A[j] = Tmp;
    }
}

void Shellsort(ElementType A[], int N)
{
    int i, j, Increment;
    ElementType Tmp;

    for (Increment = N / 2; Increment > 0; Increment /= 2)
        for (i = Increment; i < N; i++)
        {
            Tmp = A[i];
            for (j = i; j >= Increment; j -= Increment)
                if (Tmp < A[j - Increment])
                    A[j] = A[j - Increment];
                else
                    break;
            A[j] = Tmp;
        }
}

void PrecDown(ElementType A[], int i, int N)
{
    int Child;
    ElementType Tmp;

    for (Tmp = A[i]; LeftChild(i) < N; i = Child)
    {
        Child = LeftChild(i);
        if (Child != N - 1 && A[Child + 1] > A[Child])
            Child++;
        if (Tmp < A[Child])
            A[i] = A[Child];
        else
            break;
    }
    A[i] = Tmp;
}

void Heapsort(ElementType A[], int N)
{
    int i;

    for (i = N / 2; i >= 0; i--)
        PrecDown(A, i, N);
    for (i = N - 1; i > 0; i--)
    {
        Swap(&A[0], &A[i]);
        PrecDown(A, 0, i);
    }
}

void MSort(ElementType A[], ElementType TmpArray[], int Left, int Right)
{
    int Center;

    if (Left < Right)
    {
        Center = (Left + Right) / 2;
        MSort(A, TmpArray, Left, Center);
        MSort(A, TmpArray, Center + 1, Right);
        Merge(A, TmpArray, Left, Center + 1, Right);
    }
}

void Mergesort(ElementType A[], int N)
{
    ElementType *TmpArray;
    TmpArray = (ElementType *)Malloc(N * sizeof(ElementType));
    MSort(A, TmpArray, 0, N - 1);
    Free(TmpArray);
}

void Merge(ElementType A[], ElementType TmpArray[], int Lpos, int Rpos, int RightEnd)
{
    int i, LeftEnd, NumElements, TmpPos;

    LeftEnd = Rpos - 1;
    TmpPos = Lpos;
    NumElements = RightEnd - Lpos + 1;

    while (Lpos <= LeftEnd && Rpos <= RightEnd)
        if (A[Lpos] <= A[Rpos])
            TmpArray[TmpPos++] = A[Lpos++];
        else
            TmpArray[TmpPos++] = A[Rpos++];
    while (Lpos <= LeftEnd)
        TmpArray[TmpPos++] = A[Lpos++];
    while (Rpos <= RightEnd)
        TmpArray[TmpPos++] = A[Rpos++];

    for (i = 0; i < NumElements; i++, RightEnd--)
        A[RightEnd] = TmpArray[RightEnd];
}

void Quicksort(ElementType A[], int N)
{
    Qsort(A, 0, N - 1);
}

ElementType Median3(ElementType A[], int Left, int Right)
{
    int Center = (Left + Right) / 2;

    if (A[Left] > A[Center])
        Swap(&A[Left], &A[Center]);
    if (A[Left] > A[Right])
        Swap(&A[Left], &A[Right]);
    if (A[Center] > A[Right])
        Swap(&A[Center], &A[Right]);

    Swap(&A[Center], &A[Right - 1]);
    return A[Right - 1];
}

void Qsort(ElementType A[], int Left, int Right)
{
    int i, j;
    ElementType Pivot;

    if (Left + Cutoff <= Right)
    {
        Pivot = Median3(A, Left, Right);
        i = Left;
        j = Right - 1;
        while (i < j)
        {
            do
            {
                i++;
            } while (A[i] < Pivot);
            do
            {
                j--;
            } while (A[j] > Pivot);
            if (i < j)
                Swap(&A[i], &A[j]);
        }
        Swap(&A[i], &A[Right - 1]);

        Qsort(A, Left, i - 1);
        Qsort(A, i + 1, Right);
    }
    else
        Insertionsort(A + Left, Right - Left + 1);
}

void Qselect(ElementType A[], int k, int Left, int Right)
{
    int i, j;
    ElementType Pivot;

    if (Left + Cutoff <= Right)
    {
        Pivot = Median3(A, Left, Right);
        i = Left;
        j = Right - 1;
        while (i < j)
        {
            do
            {
                i++;
            } while (A[i] < Pivot);
            do
            {
                j--;
            } while (A[j] > Pivot);
            if (i < j)
                Swap(&A[i], &A[j]);
        }
        Swap(&A[i], &A[Right - 1]);
        if (k <= i)
            Qselect(A, k, Left, i - 1);
        else if (k > i + 1)
            Qselect(A, k, i + 1, Right);
    }
    else
        Insertionsort(A + Left, Right - Left + 1);
}

void Bucketsort(ElementType A[], int N)
{
    int i;
    int *TmpArray;

    TmpArray = (int *)Malloc(MaxNum * sizeof(int));
    for (i = 0; i < MaxNum; i++)
        TmpArray[i] = 0;

    Bsort(A, TmpArray, N);

    Free(TmpArray);
}

void Bsort(ElementType A[], int TmpArray[], int N)
{
    int i, j;
    int P;
    for (int i = 0; i < N; i++)
    {
        if (A[i] >= MaxNum)
            runtime_error("The number exceeds the limit of Bucketsort");
        TmpArray[A[i]]++;
    }

    P = 0;
    for (int i = 0; i < MaxNum; i++)
    {
        for (j = 0; j < TmpArray[i]; j++)
            A[P++] = i;
    }
}

#endif

#ifdef _DisjSet_H

void Initialize(DisjSet S)
{
    int i;

    for (i = NumSets; i > 0; i--)
        S[i] = -1;
}

// void SetUnion(DisjSet S, SetType Root1, SetType Root2)
// {
//     S[Root2] = Root1;
// }

// SetType Find(ElementType X, DisjSet S)
// {
//     if (S[X] <= 0)
//         return X;
//     else
//         return Find(S[X], S);
// }

// void SetUnion(DisjSet S, SetType Root1, SetType Root2)
// {
//     if (S[Root2] < S[Root1])
//         S[Root1] = Root2;
//     else
//     {
//         if (S[Root1] == S[Root2])
//             S[Root1]--;
//         S[Root2] = Root1;
//     }
// }

void SetUnion(DisjSet S, SetType Root1, SetType Root2)
{
    if (S[Root2] < S[Root1])
    {
        S[Root2] += S[Root1];
        S[Root1] = Root2;
    }
    else
    {
        S[Root1] += S[Root2];
        S[Root2] = Root1;
    }
}

SetType Find(ElementType X, DisjSet S)
{
    if (S[X] <= 0)
        return X;
    else
        return S[X] = Find(S[X], S);
}

#endif

#ifdef _Random_H

// double Random(void)
// {
//     Seed = (A * Seed) % M;
//     return (double)Seed / M;
// }

// void Initialize(unsigned long InitVal)
// {
//     Seed = InitVal;
// }

double Random(void)
{
    long TmpSeed;

    TmpSeed = A * (Seed % Q) - R * (Seed / Q);
    if (TmpSeed >= 0)
        Seed = TmpSeed;
    else
        Seed = TmpSeed;
    return (double)Seed / M;
}

void Initialize(unsigned long InitVal)
{
    Seed = InitVal;
}

#endif

#ifdef _Three_Tuple_List_H

static void DeleteNode(Position P)
{
    Free(P);
}

static Position NewNode(ElementType X, int Row, int Col, Position PNext)
{
    Position P = (Position)Malloc(sizeof(struct Node));
    P->Element = X;
    P->Row = Row;
    P->Col = Col;
    P->PNext = PNext;
    return P;
}

void DeleteList(List L)
{
    MakeEmpty(L);
    Free(L);
}

List Init(int Rt, int Ct)
{
    List L = (List)Malloc(sizeof(struct ListRecord));
    L->Head = L->Tail = NULL;
    MakeEmpty(L);
    L->Rt = Rt;
    L->Ct = Ct;
    return L;
}

void MakeEmpty(List L)
{
    Position P = L->Head;
    while (P)
    {
        Position PN = P->PNext;
        DeleteNode(P);
        P = PN;
    }
    L->Head = L->Tail = NULL;
}

int IsEmpty(List L)
{
    return L->Head == NULL;
}

int IsLast(Position P, List L)
{
    return P == L->Tail;
}

void InsertLast(ElementType X, int Row, int Col, List L)
{
    Position P = NewNode(X, Row, Col, NULL);
    if (IsEmpty(L))
    {
        L->Head = L->Tail = P;
    }
    else
    {
        L->Tail->PNext = P;
        L->Tail = P;
    }
}

ElementType Retrieve(Position P)
{
    return P->Element;
}

List Reverse(List L)
{
    List LN = Init(L->Ct, L->Rt);
    for (int i = 1; i <= L->Ct; i++)
    {
        Position P = L->Head;
        while (P)
        {
            if (P->Col == i)
                InsertLast(P->Element, P->Col, P->Row, LN);
            P = P->PNext;
        }
    }
    return LN;
}

List Add(List La, List Lb)
{
    if (La->Ct != Lb->Ct || La->Rt != La->Rt)
        runtime_error("Invalid Martix Add");
    List L = Init(La->Rt, La->Ct);
    Position Pa = La->Head, Pb = Lb->Head;
    while (Pa && Pb)
    {
        if (Pa->Col == Pb->Col && Pa->Row == Pb->Row)
        {
            if (Pa->Element + Pb->Element != 0)
                InsertLast(Pa->Element + Pb->Element, Pa->Row, Pa->Col, L);
            Pa = Pa->PNext;
            Pb = Pb->PNext;
        }
        else if (Pa->Row < Pb->Row || Pa->Row == Pb->Row && Pa->Col < Pb->Col)
        {
            InsertLast(Pa->Element, Pa->Row, Pa->Col, L);
            Pa = Pa->PNext;
        }
        else
        {
            InsertLast(Pb->Element, Pb->Row, Pb->Col, L);
            Pb = Pb->PNext;
        }
    }
    while (Pa)
    {
        InsertLast(Pa->Element, Pa->Row, Pa->Col, L);
        Pa = Pa->PNext;
    }
    while (Pb)
    {
        InsertLast(Pb->Element, Pb->Row, Pb->Col, L);
        Pb = Pb->PNext;
    }
    return L;
}

Position FindNext(Position P)
{
    if (!P)
        return P;
    while (P->PNext && P->PNext->Row == P->Row)
        P = P->PNext;
    P = P->PNext;
    return P;
}

List Mul(List La, List Lb)
{
    if (La->Ct != Lb->Rt)
        runtime_error("Invalid Martix Mul");
    List L = Init(La->Rt, Lb->Ct);
    Lb = Reverse(Lb);

    for (Position Pas = La->Head, Pae = FindNext(Pas); Pas != Pae; Pas = Pae, Pae = FindNext(Pas))
    {
        for (Position Pbs = Lb->Head, Pbe = FindNext(Pbs); Pbs != Pbe; Pbs = Pbe, Pbe = FindNext(Pbs))
        {
            ElementType Result = 0;
            Position P1 = Pas;
            Position P2 = Pbs;
            while (P1 != Pae && P2 != Pbe)
            {
                if (P1->Col == P2->Col)
                {
                    Result += P1->Element * P2->Element;
                    P1 = P1->PNext;
                    P2 = P2->PNext;
                }
                else if (P1->Col < P2->Col)
                    P1 = P1->PNext;
                else
                    P2 = P2->PNext;
            }
            if (Result != 0)
                InsertLast(Result, Pas->Row, Pbs->Row, L);
        }
    }
    DeleteList(Lb);
    return L;
}

#endif

#ifdef _Orthogonal_List_H

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

#endif

#ifndef _Splay_H

static Position NullNode = NULL;

Position NewNode(ElementType X, SplayTree Left, SplayTree Right)
{
    Position P;
    P = (Position)Malloc(sizeof(struct SplayNode));
    P->Element = X;
    P->Left = Left;
    P->Right = Right;
    return P;
}

void DeleteNode(Position P)
{
    Free(P);
}

SplayTree Initialize(void)
{
    if (NullNode == NULL)
    {
        NullNode = (SplayTree)Malloc(sizeof(struct SplayNode));
        NullNode->Left = NullNode->Right = NullNode;
    }
    return NullNode;
}

SplayTree Splay(ElementType Item, Position X)
{
    static struct SplayNode Header;
}

#endif
