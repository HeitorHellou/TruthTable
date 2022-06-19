#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

// -------------- STACK ------------------
typedef struct S_Node
{
  char operator;
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
// -------------- UTIL ------------------
/* ---------------------------------------------
   |OPERATOR | NAME              | PRECEDENCE  |
   ---------------------------------------------
   |!        | Negation          | 5           |
   ---------------------------------------------
   |&        | Conjunction       | 4           |
   ---------------------------------------------
   |+        | Disjunction       | 3           |
   ---------------------------------------------
   |>        | Implication       | 2           |
   ---------------------------------------------
   |:        | Biconditional     | 1           |
   ---------------------------------------------
----------------------------------------*/
void removeSpaces(char *str); // remove white spaces form string
int getPrecedence(char op); // gets operator precedence
int isOperand(char op); // check if the character is a letter
int countUniqueOperands(char *str); // count how many unique operands are in the expression
int* negation(int arr[], int size);
int* conjunction(int arr[], int arr2[], int size);
int* disjunction(int arr[], int arr2[], int size);
int* implication(int arr[], int arr2[], int size);
int* biconditional(int arr[], int arr2[], int size);

Queue* infixToPostfix(char *str); // main function to transform infix expression into postfix

int main()
{
  char infix[] = "((((R & S) + (S & (!Q))) > (Q : Q)) & ((!S) : (!P)))"; // string with the infix expression
  removeSpaces(infix); // removing blank spaces
  int count = countUniqueOperands(infix); // getting the total of operands
  int size = pow(2, count); // number of variables a proposition will have

  Queue *postfix = infixToPostfix(infix); // converting the expression

  LinkedList *operands = NULL; // List of operands with its truth table values

  int hash[128] = { 0 };
  int c = 1;
  for (int i = 0; infix[i]; i++)
  {
    if (isOperand(infix[i]))
      hash[infix[i]] = 1;
  }

  for (int i = 0; i < 128; i++)
  {
    if (hash[i] == 1)
    {
      insertLast(&operands, i, count, c);
      c++;
    }
  }

  //printList(operands);

  //printQueue(postfix);

  Stack *stack = NULL;
  char op;

  while (postfix->head != NULL)
  {
    op = deQueue(postfix);
    if (isOperand(op))
      pushArray(&stack, getOperandArray(operands, op));
    else 
    {
      switch (op)
      {
        case '!':
          pushArray(&stack, negation(popArray(&stack), size));
          break;
        case '&':
          pushArray(&stack, conjunction(popArray(&stack), popArray(&stack), size));
          break;
        case '+':
          pushArray(&stack, disjunction(popArray(&stack), popArray(&stack), size));
          break;
        case '>':
          pushArray(&stack, implication(popArray(&stack), popArray(&stack), size));
          break;
        case ':':
          pushArray(&stack, biconditional(popArray(&stack), popArray(&stack), size));
          break;
      }
    }
  }

  int *res = popArray(&stack);

  for (int i = 0; i < size; i++)
    printf("%d \n", res[i]);

  return 0;
}

Queue* infixToPostfix(char *str)
{
  Stack *stack = NULL; // Initializing the Stack
  Queue* queue = (Queue*)malloc(sizeof(Queue)); // Initializing the Queue
  queue->head = NULL;
  queue->tail = NULL;

  // For loop to check every char in the expression
  for (int i = 0; str[i]; i++)
  {
    // if its a operand enQueue it
    if (isOperand(str[i]) == 1)
      enQueue(queue, str[i]);
    // if its a operator we need to make some tests
    // if the operator is '(' or the stack is empty push into the stack
    // it its a ')' pop everything until you find a '('
    // if the precedence of the operator at the top of the stack is lesser than the scanned character, push into the stack
    // if the precedence of the operator at the top of the stack is greater, pop the stack and add it to the queue than push the scanned operator to the stack
    // repeat it until all characters are read 
    else if (str[i] == '(')
      push(&stack, str[i]);
    else 
    {
      if (str[i] == ')')
      {
        while (!isEmpty(stack) && peek(stack) != '(')
          enQueue(queue, pop(&stack));

        pop(&stack);
      }
      else 
      {
        if (isEmpty(stack))
          push(&stack, str[i]);
        else if (getPrecedence(str[i]) <= getPrecedence(peek(stack)))
        {
          enQueue(queue, pop(&stack));
          push(&stack, str[i]);
        }
        else 
          push(&stack, str[i]);
      }
    }
  }

  while (stack != NULL)
    enQueue(queue, pop(&stack));

  return queue;
}

// STACK FUNCTIONS
Stack* newStackNode(char op)
{
  Stack *node = (Stack*)malloc(sizeof(Stack));
  node->operator = op;
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
  char popped = temp->operator;
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
  return head_ref->operator;
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

// UTIL
void removeSpaces(char *str)
{
  char* temp = str;
  do 
  {
    while (*temp == ' ')
      ++temp;
  } while (*str++ = *temp++);
}

int getPrecedence(char op)
{
  if (op == '!')
    return 5;
  if (op == '&')
    return 4;
  if (op == '+')
    return 3;
  if (op == '>')
    return 2;
  return 1;
}

int isOperand(char op)
{
  return (op >= 'a' && op <= 'z') || (op >= 'A' && op <= 'Z');
}

int countUniqueOperands(char *str)
{
  int hash[128] = { 0 }; // there is 128 characters in C - each represented by a unique integer value -> ASCII value
  int c = 0;

  for (int i = 0; str[i]; i++)
  {
    // only counting the operads
    // setting the position corresponding to the ASCII value of str[i] in hash[] to 1
    if (isOperand(str[i]))
      hash[str[i]] = 1;
  }

  // counting the number of 1's in hash[]
  for (int i = 0; i < 128; i++)
    c += hash[i];

  return c;
}

int* negation(int arr[], int size)
{
  for (int i = 0; i < size; i++)
  {
    arr[i] = !arr[i];
  }

  return arr;
}

int* conjunction(int arr[], int arr2[], int size)
{
  int* res = (int*)malloc(size * sizeof(int));

  for (int i = 0; i < size; i++)
  {
    res[i] = arr[i] && arr2[i];
  }

  return res;
}

int* disjunction(int arr[], int arr2[], int size)
{
  int* res = (int*)malloc(size * sizeof(int));

  for (int i = 0; i < size; i++)
  {
    res[i] = arr[i] || arr2[i];
  }

  return res;
}

int* implication(int arr[], int arr2[], int size)
{
  int* res = (int*)malloc(size * sizeof(int));

  for (int i = 0; i < size; i++)
  {
    res[i] = !arr[i] || arr2[i];
  }

  return res;
}

int* biconditional(int arr[], int arr2[], int size)
{
  int* res = (int*)malloc(size * sizeof(int));

  for (int i = 0; i < size; i++)
  {
    res[i] = arr[i] == arr2[i];
  }

  return res;
}