#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// -------------- STACK ------------------
typedef struct S_Node
{
  char operator;
  struct S_Node *next;
}Stack;

Stack* newStackNode(char op); // creates new stack node
void push(Stack **head_ref, char c); // push a node into the stack
char pop(Stack **head_ref); // pop the stack
int isEmpty(Stack *head_ref); // check if the stack is empty
int peek(Stack *head_ref); // return the top element of the stack
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
int isOperand(char op); // check if the operand is a letter

void infixToPostfix(char *str); // main function to transform infix expression into postfix

int main()
{
  char infix[] = "P + ! Q"; // string with the infix expression
  removeSpaces(infix); // removing blank spaces

  infixToPostfix(infix); // converting the expression

  return 0;
}

void infixToPostfix(char *str)
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
        if (isEmpty(stack) || peek(stack) == '(')
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

  printQueue(queue);
}

// STACK FUNCTIONS
Stack* newStackNode(char op)
{
  Stack *node = (Stack *)malloc(sizeof(Stack));
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

int isEmpty(Stack *head_ref)
{
  return head_ref == NULL;
}

int peek(Stack *head_ref)
{
  if (isEmpty(head_ref))
    return 0;
  return head_ref->operator;
}

// QUEUE FUNCTIONS
Q_Node* newQueueNode(Q_Node *n, char op)
{
  n = (Q_Node *)malloc(sizeof(Q_Node));
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