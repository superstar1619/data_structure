#include <stdio.h>
#include <stdlib.h>

void unix_error(char *msg);
void runtime_error(char *msg);

void *Malloc(size_t size);
void *Realloc(void *ptr, size_t size);
void *Calloc(size_t nmemb, size_t size);
void Free(void *ptr);

#ifndef ElementType
#define ElementType int
#endif

#ifdef _List_H

struct Node;
typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;

void DeleteNode(Position P);
Position NewNode(ElementType X, Position PNext);

List Init();
List MakeEmpty(List L);
int IsEmpty(List L);
int IsLast(Position P, List L);
Position Find(ElementType X, List L);
void Delete(ElementType X, List L);
Position FindPrevious(ElementType X, List L);
void Insert(ElementType X, List L, Position P);
void DeleteList(List L);
Position Header(List L);
Position First(List L);
Position Advance(Position P);
ElementType Retrieve(Position P);

struct Node
{
    ElementType Element;
    Position Next;
};

#endif

#ifdef _Cursor_H

#define SpaceSize 1024

typedef int PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;

void InitializeCursorSpace(void);

Position CursorAlloc(void);
static void CursorFree(Position P);

void DeleteNode(Position P);
Position NewNode(ElementType X, Position PNext);

List MakeEmpty(List L);
int IsEmpty(const List L);
int IsLast(const Position P, const List L);
Position Find(ElementType X, const List L);
void Delete(ElementType X, List L);
Position FindPrevious(ElementType X, const List L);
void Insert(ElementType X, List L, Position P);
void DeleteList(List L);
Position Header(const List L);
Position First(const List L);
Position Advance(const Position P);
ElementType Retrieve(const Position P);
struct Node
{
    ElementType Element;
    Position Next;
};

struct Node CursorSpace[SpaceSize];

#endif