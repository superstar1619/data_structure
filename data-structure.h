#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <limits.h>

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

#ifdef _DoubleList_H

struct Node;
typedef struct Node *PtrToNode;
struct ListRecord;
typedef struct ListRecord *List;
typedef PtrToNode Position;

static void DeleteNode(Position P);
static Position NewNode(ElementType X, Position PPrev, Position PNext);

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
Position Prev(Position P);
Position Next(Position P);
ElementType Retrieve(Position P);

struct Node
{
    ElementType Element;
    Position Prev;
    Position Next;
};

struct ListRecord
{
    Position Head;
    Position Tail;
};

#endif

#ifdef _Cursor_H

#define SpaceSize 1000010

typedef int PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;

void InitializeCursorSpace(void);

Position CursorAlloc(void);
void CursorFree(Position P);

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

#ifdef _Queue_H

struct Node;
typedef struct Node *PtrToNode;

struct QueueRecord;
typedef struct QueueRecord *Queue;

PtrToNode NewNode(ElementType X, PtrToNode PNext);
void DeleteNode(PtrToNode P);

int IsEmpty(Queue Q);
Queue CreateQueue();
void DisposeQueue(Queue Q);
void MakeEmpty(Queue Q);
void Enqueue(ElementType X, Queue Q);
ElementType Front(Queue Q);
void Dequeue(Queue Q);
ElementType FrontAndDequeue(Queue Q);

#define MinQueueSize (5)

struct Node
{
    ElementType Element;
    PtrToNode Next;
};

struct QueueRecord
{
    PtrToNode Front;
    PtrToNode Rear;
    int Size;
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
static AvlTree TryRotate(AvlTree T);
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

#ifdef _HashSep_H

#define MinTableSize (5)

struct ListNode;
typedef struct ListNode *Position;
struct HashTbl;
typedef struct HashTbl *HashTable;

char *ConvertToString(ElementType KeyValue);

typedef unsigned int Index;
Index Hash(ElementType KeyValue, int TableSize);

Position NewNode(ElementType X, Position PNext);
void DeleteNode(Position P);

static int IsPrime(int Value);
static int NextPrime(int Value);
HashTable InitializeTable(int TableSize);
void DestroyList(List L);
void DestroyTable(HashTable H);
Position Find(ElementType Key, HashTable H);
void Insert(ElementType Key, HashTable H);
ElementType Retrieve(Position P);

struct ListNode
{
    ElementType Element;
    Position Next;
};

typedef Position List;

struct HashTbl
{
    int TableSize;
    List *TheLists;
};

#endif

#ifdef _HashQuad_H

#define MinTableSize (5)

typedef unsigned int Index;
typedef Index Position;

struct HashTbl;
typedef struct HashTbl *HashTable;

char *ConvertToString(ElementType KeyValue);
Index Hash(ElementType KeyValue, int TableSize);

static int IsPrime(int Value);
static int NextPrime(int Value);

HashTable InitializeTable(int TableSize);
void DestroyTable(HashTable H);
Position Find(ElementType Key, HashTable H);
void Insert(ElementType Key, HashTable H);
ElementType Retrieve(Position P, HashTable H);
HashTable Rehash(HashTable H);

enum KindOfEntry
{
    Legitimate,
    Empty,
    Delete
};

struct HashEntry
{
    ElementType Element;
    enum KindOfEntry Info;
};

typedef struct HashEntry Cell;

struct HashTbl
{
    int TableSize;
    Cell *TheCells;
};

#endif

#ifdef _BinHeap_H

#define MinData (INT_MIN)

#define MinPQSize (5)

struct HeapStruct;
typedef struct HeapStruct *PriorityQueue;

PriorityQueue Initialize(int MaxElements);
void Destroy(PriorityQueue H);
void MakeEmpty(PriorityQueue H);
void Insert(ElementType X, PriorityQueue H);
ElementType DeleteMin(PriorityQueue H);
ElementType FindMin(PriorityQueue H);
int IsEmpty(PriorityQueue H);
int IsFull(PriorityQueue H);

struct HeapStruct
{
    int Capacity;
    int Size;
    ElementType *Elements;
};

#endif

#ifdef _LeftHeap_H

struct TreeNode;
typedef struct TreeNode *PriorityQueue;

static PriorityQueue NewNode(ElementType X, PriorityQueue Left, PriorityQueue Right, int Npl);
static void DeleteNode(PriorityQueue H);
static void SwapChild(PriorityQueue H);
PriorityQueue Initialize(void);
ElementType FindMin(PriorityQueue H);
int IsEmpty(PriorityQueue H);
PriorityQueue Merge(PriorityQueue H1, PriorityQueue H2);
static PriorityQueue Merge1(PriorityQueue H1, PriorityQueue H2);

#define Insert(X, H) (H = Insert1((X), H))

PriorityQueue Insert1(ElementType X, PriorityQueue H);
PriorityQueue DeleteMin1(PriorityQueue);

struct TreeNode
{
    ElementType Element;
    PriorityQueue Left;
    PriorityQueue Right;
    int Npl;
};

#endif

#ifdef _Binomial_H

#define Infinity (30000L)

#define MaxTrees (14)
#define Capacity (16383)

struct BinNode;
typedef struct BinNode *BinTree;
typedef struct BinNode *Position;
struct Collection;
typedef struct Collection *BinQueue;

BinQueue Initialize(void);
BinTree NewNode(ElementType X, Position LeftChild, Position NextSibling);
void DeleteNode(BinTree T);
void DestroyBinTree(BinTree T);
void Destroy(BinQueue H);
void MakeEmpty(BinQueue H);
BinTree CombineTrees(BinTree T1, BinTree T2);
BinQueue Insert(ElementType Item, BinQueue H);
ElementType DeleteMin(BinQueue H);
BinQueue Merge(BinQueue H1, BinQueue H2);
ElementType FindMin(BinQueue H);
int IsEmpty(BinQueue H);
int IsFull(BinQueue H);

struct BinNode
{
    ElementType Element;
    Position LeftChild;
    Position NextSibling;
};

struct Collection
{
    int CurrentSize;
    BinTree TheTrees[MaxTrees];
};

#endif

#ifdef _Sort_h

void Swap(ElementType *a, ElementType *b);

void Insertionsort(ElementType A[], int N);

void Shellsort(ElementType A[], int N);

#define LeftChild(i) (2 * (i) + 1)
void PrecDown(ElementType A[], int i, int N);
void Heapsort(ElementType A[], int N);

void MSort(ElementType A[], ElementType TmpArray[], int Left, int Right);
void Mergesort(ElementType A[], int N);
void Merge(ElementType A[], ElementType TmpArray[], int Lpos, int Rpos, int RightEnd);

#define Cutoff (3)
void Quicksort(ElementType A[], int N);
ElementType Median3(ElementType A[], int Left, int Right);
void Qsort(ElementType A[], int Left, int Right);
void Qselect(ElementType A[], int k, int Left, int Right);

#define MaxNum (16383)
void Bucketsort(ElementType A[], int N);
void Bsort(ElementType A[], int TmpArray[], int N);

#endif

#ifdef _DisjSet_H

#define NumSets 1010
typedef int DisjSet[NumSets + 1];
typedef int SetType;

void Initialize(DisjSet S);
void SetUnion(DisjSet S, SetType Root1, SetType Root2);
SetType Find(ElementType X, DisjSet S);

#endif

#ifdef _Random_H

static unsigned long Seed = 1;

#define A 48271L
#define M 2147483647L
#define Q (M / A)
#define R (M % A)

double Random(void);
void Initialize(unsigned long InitVal);

#endif

#ifdef _Three_Tuple_List_H

struct Node;
typedef struct Node *PtrToNode;
struct ListRecord;
typedef struct ListRecord *List;
typedef PtrToNode Position;

static void DeleteNode(Position P);
static Position NewNode(ElementType X, int Row, int Col, Position PNext);
void DeleteList(List L);
List Init(int Rt, int Ct);
void MakeEmpty(List L);
int IsEmpty(List L);
int IsLast(Position P, List L);
void InsertLast(ElementType X, int Row, int Col, List L);
ElementType Retrieve(Position P);

List Reverse(List L);
List Add(List La, List Lb);
Position FindNext(Position P);
List Mul(List La, List Lb);

struct Node
{
    int Row, Col;
    ElementType Element;
    PtrToNode PNext;
};

struct ListRecord
{
    Position Head, Tail;
    int Rt, Ct;
};

/*
struct Node;
typedef struct Node *PtrToNode;
struct ListRecord;
typedef struct ListRecord *List;
typedef PtrToNode Position;

static void DeleteNode(Position P);
static Position NewNode(ElementType X, Position PPrev, Position PNext);

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
Position Prev(Position P);
Position Next(Position P);
ElementType Retrieve(Position P);

struct Node
{
    ElementType Element;
    Position Prev;
    Position Next;
};

struct ListRecord
{
    Position Head;
    Position Tail;
};

*/
#endif

#ifdef _Orthogonal_List_H

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

#endif

#ifndef _Splay_H

struct SplayNode;
typedef struct SplayNode *SplayTree;
typedef struct SplayNode *Position;
#define Infinity 30000
#define NegInfinity (-30000)

Position NewNode(ElementType X, SplayTree Left, SplayTree Right);
void DeleteNode(Position P);
SplayTree MakeEmpty(SplayTree T);
SplayTree Find(ElementType X, SplayTree T);
SplayTree FindMin(SplayTree T);
SplayTree FindMax(SplayTree T);
SplayTree Initialize(void);
SplayTree Insert(ElementType X, SplayTree T);
SplayTree Remove(ElementType X, SplayTree T);
ElementType Retrieve(SplayTree T);

struct SplayNode
{
    ElementType Element;
    SplayTree Left;
    SplayTree Right;
};

#endif