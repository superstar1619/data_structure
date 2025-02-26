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

#ifndef _Stack_H

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
    if (IsFull(X))
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
        return S->Array[S->TopOfStack];
    runtime_error("Empty Stack");
    return 0;
}

#endif