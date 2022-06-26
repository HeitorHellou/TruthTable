#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
// -------------- STACK ------------------
typedef struct S_Node
{
  char op;
  int *arr;
  struct S_Node *next;
}Stack;

Stack* newStackNode(char op); // creates new stack node
void push(Stack **head_ref, char c); // push a node into the stack
void pushArray(Stack **head_ref, int arr[]);
char pop(Stack **head_ref); // pop the stack
int* popArray(Stack **head_ref);
int isEmpty(Stack *head_ref); // check if the stack is empty
char peek(Stack *head_ref); // return the top element of the stack
// -------------- STACK ------------------
// -------------- QUEUE ------------------
typedef struct Node
{
  char operand;
  struct Node *next;
}Q_Node;

typedef struct Queue
{
  Q_Node *head, *tail;
}Queue;

Q_Node* newQueueNode(Q_Node *n, char c); // creates new queue node
void enQueue(Queue *queue_ref, char c); // inserts a node in the queue
char deQueue(Queue *queue_ref); // removes a node form the queue
void printQueue(Queue *queue_ref); // printing the queue
// -------------- QUEUE ------------------
// -------------- LIST -------------------
typedef struct L_Node
{
  char operand;
  int size;
  int *arr;
  struct L_Node *next;
} LinkedList;

void insertLast(LinkedList **head_ref, char op, int total, int pos);  // insert a new node at the end of the list
int* getOperandArray(LinkedList *head_ref, char op);
void printList(LinkedList *head_ref);
// -------------- LIST -------------------

// STACK FUNCTIONS
Stack* newStackNode(char op)
{
  Stack *node = (Stack*)malloc(sizeof(Stack));
  node->op = op;
  node->next = NULL;
  return node;
}

void push(Stack **head_ref, char c)
{
  Stack *node = newStackNode(c);
  node->next = *head_ref;
  *head_ref = node;
}

void pushArray(Stack **head_ref, int arr[])
{
  Stack *node = (Stack*)malloc(sizeof(Stack));
  node->arr = arr;
  node->next = *head_ref;
  *head_ref = node;
}

char pop(Stack **head_ref)
{
  if (isEmpty(*head_ref))
    return 0;
  Stack *temp = *head_ref;
  *head_ref = (*head_ref)->next;
  char popped = temp->op;
  free(temp);

  return popped;
}

int* popArray(Stack **head_ref)
{
  if (isEmpty(*head_ref))
    return 0;
  Stack *temp = *head_ref;
  *head_ref = (*head_ref)->next;
  int *popped = temp->arr;
  free(temp);

  return popped;
}

int isEmpty(Stack *head_ref)
{
  return head_ref == NULL;
}

char peek(Stack *head_ref)
{
  if (isEmpty(head_ref))
    return 0;
  return head_ref->op;
}

// QUEUE FUNCTIONS
Q_Node* newQueueNode(Q_Node *n, char op)
{
  n = (Q_Node*)malloc(sizeof(Q_Node));
  n->operand = op;
  n->next = NULL;
  return n;
}

void enQueue(Queue *queue_ref, char c)
{
  Q_Node *node = NULL;
  node = newQueueNode(node, c);
  if (queue_ref->tail == NULL) 
  {
    queue_ref->head = queue_ref->tail = node;
    return;
  }
    
  queue_ref->tail->next = node;
  queue_ref->tail = node;
}

char deQueue(Queue* queue_ref)
{
  if (queue_ref->head == NULL)
    return ':';

  char operand = queue_ref->head->operand;
  queue_ref->head = queue_ref->head->next;

  if (queue_ref->head == NULL)
    queue_ref->tail = NULL;

  return operand;
}

void printQueue(Queue *q)
{
  Q_Node *temp = q->head;
  while (temp != NULL)
  {
    printf("%c", temp->operand);
    temp = temp->next;
  }
}

// LIST
void insertLast(LinkedList **head_ref, char op, int total, int pos)
{
  // Creating the new node
  int max = (int)pow(2, total);
  LinkedList *node = (LinkedList*)malloc(sizeof(LinkedList));
  node->operand = op; // operand
  node->size = max;
  node->arr = malloc(max * sizeof(int)); // array representing the operand 
  // fills the array correspondly with the number of operands
  // 1 - True / 0 - False
  for (int i = 0; i < max; i++)
    node->arr[i] = (i & (int)pow(2, (total - pos))) == 0;
  node->next = NULL;

  LinkedList *last = *head_ref;

  // if the list is empty insert node
  if (*head_ref == NULL)
  {
    *head_ref = node;
    return;
  }

  // else traverse till last node
  while (last->next != NULL)
    last = last->next;

  // change the next of last node
  last->next = node;
  return;
}

int* getOperandArray(LinkedList *head_ref, char op)
{
  LinkedList *temp = head_ref;
  while (temp != NULL)
  {
    if (temp->operand == op)
      return temp->arr;

    temp = temp->next;
  }

  return 0;
}

void printList(LinkedList *head_ref)
{
  while (head_ref != NULL)
  {
    printf("%c \n", head_ref->operand);
    for (int i = 0; i < head_ref->size; i++)
    {
      printf("%d \n", head_ref->arr[i]);
    }
    head_ref = head_ref->next;
  }
  printf("\n");
}