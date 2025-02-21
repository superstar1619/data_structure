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
