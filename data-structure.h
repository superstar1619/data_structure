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

#ifdef _List_H

struct Node;
typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;

static void DeleteNode(Position P);
static Position NewNode(ElementType X, Position PNext);

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

static Position CursorAlloc(void);
static void CursorFree(Position P);

static void DeleteNode(Position P);
static Position NewNode(ElementType X, Position PNext);

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

#ifdef _Stack_h

struct Node;
typedef struct Node *PtrToNode;
typedef PtrToNode Stack;

static PtrToNode NewNode(ElementType X, PtrToNode PNext);
static void DeleteNode(PtrToNode P);

int IsEmpty(Stack S);
Stack CreateStack(void);
void DisposeStack(Stack S);
void MakeEmpty(Stack S);
void Push(ElementType X, Stack S);
ElementType Top(Stack S);
void Pop(Stack S);

struct Node
{
    ElementType Element;
    PtrToNode Next;
};

#endif

#ifdef _Stack_H

struct StackRecord;
typedef struct StackRecord *Stack;

int IsEmpty(Stack S);
int IsFull(Stack S);
Stack CreateStack(int MaxElements);
void DisposeStack(Stack S);
void MakeEmpty(Stack S);
void Push(ElementType X, Stack S);
ElementType Top(Stack S);
void Pop(Stack S);
ElementType TopAndPop(Stack S);

#define EmptyTOS (-1)
#define MinStackSize (5)

struct StackRecord
{
    int Capacity;
    int TopOfStack;
    ElementType *Array;
};

#endif

#ifdef _Queue_h

struct QueueRecord;
typedef struct QueueRecord *Queue;

int IsEmpty(Queue Q);
int IsFull(Queue Q);
Queue CreateQueue(int MaxElements);
void DisposeQueue(Queue Q);
void MakeEmpty(Queue Q);
static int Succ(int Value, Queue Q);
void Enqueue(ElementType X, Queue Q);
ElementType Front(Queue Q);
void Dequeue(Queue Q);
ElementType FrontAndDequeue(Queue Q);

#define MinQueueSize (5)

struct QueueRecord
{
    int Capacity;
    int Front;
    int Rear;
    int Size;
    ElementType *Array;
};

#endif

#ifdef _Deque_h

struct DequeRecord;
typedef struct DequeRecord *Deque;

int IsEmpty(Deque D);
int IsFull(Deque D);
Deque CreateDeque(int MaxElements);
void DisposeDeque(Deque D);
void MakeEmpty(Deque D);
static int Succ(int Value, Deque D);
static int Prev(int Value, Deque D);
void Push(ElementType X, Deque D);
void Pop(Deque D);
void Inject(ElementType X, Deque D);
void Eject(Deque D);
ElementType Front(Deque D);
ElementType Rear(Deque D);
ElementType FrontAndPop(Deque D);
ElementType RearAndEject(Deque D);

#define MinDequeSize (5)

struct DequeRecord
{
    int Capacity;
    int Front;
    int Rear;
    int Size;
    ElementType *Array;
};

#endif

#ifdef _Tree_h

typedef struct TreeNode *PtrToNode;
struct TreeNode
{
    ElementType Element;
    PtrToNode FirstChild;
    PtrToNode NextSibing;
};

#endif

#ifdef _Tree_H

struct TreeNode;
typedef struct TreeNode *Position;
typedef struct TreeNode *SearchTree;

static SearchTree NewNode(ElementType X, SearchTree Left, SearchTree Right);
static void DeleteNode(SearchTree T);

SearchTree MakeEmpty(SearchTree T);
Position Find(ElementType X, SearchTree T);
Position FindMin(SearchTree T);
Position FindMax(SearchTree T);
SearchTree Insert(ElementType X, SearchTree T);
SearchTree Delete(ElementType X, SearchTree T);
ElementType Retrieve(Position P);
Position FindPrev(ElementType X, SearchTree T);
Position FindNext(ElementType X, SearchTree T);

struct TreeNode
{
    ElementType Element;
    SearchTree Left;
    SearchTree Right;
};

#endif

#ifdef _Tree_H_NR

struct TreeNode;
typedef struct TreeNode *Position;
typedef struct TreeNode *SearchTree;
typedef struct TreeNode **LinkLine;

static SearchTree NewNode(ElementType X, SearchTree Left, SearchTree Right);
static void DeleteNode(SearchTree T);
static void ModifyLine(Position P, LinkLine L);
SearchTree MakeEmpty(SearchTree T);
Position Find(ElementType X, SearchTree T);
Position FindMin(SearchTree T);
Position FindMax(SearchTree T);
void Insert(ElementType X, LinkLine LT);
static void DeleteSpecialTree(SearchTree T, LinkLine L);
void Delete(ElementType X, LinkLine LT);
ElementType Retrieve(Position P);
Position FindPrev(ElementType X, SearchTree T);
Position FindNext(ElementType X, SearchTree T);

struct TreeNode
{
    ElementType Element;
    SearchTree Left;
    SearchTree Right;
};

#endif

#ifdef _AvlTree_H

struct AvlNode;
typedef struct AvlNode *Position;
typedef struct AvlNode *AvlTree;

static AvlTree NewNode(ElementType X, AvlTree Left, AvlTree Right, int Height);
static void DeleteNode(AvlTree T);

static int Max(int ValueA, int ValueB);

static Position SingleRotateWithLeft(Position K2);
static Position SingleRotateWithRight(Position K1);
static Position DoubleRotateWithLeft(Position K3);
static Position DoubleRotateWithRight(Position K1);

static int GetHeight(Position P);
static void UpdateHeight(Position P);
AvlTree MakeEmpty(AvlTree T);
Position Find(ElementType X, AvlTree T);
Position FindMin(AvlTree T);
Position FindMax(AvlTree T);
AvlTree Insert(ElementType X, AvlTree T);
AvlTree Delete(ElementType X, AvlTree T);
ElementType Retrieve(Position P);
Position FindPrev(ElementType X, AvlTree T);
Position FindNext(ElementType X, AvlTree T);

struct AvlNode
{
    ElementType Element;
    AvlTree Left;
    AvlTree Right;
    int Height;
};

#endif
