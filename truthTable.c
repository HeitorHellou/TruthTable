#include <string.h>

#include "structures.h"

// -------------- UTIL ------------------
void removeSpaces(char *str);                        // remove white spaces form string
int getPrecedence(char op);                          // gets operator precedence
int isOperand(char op);                              // check if the character is a letter
int countUniqueOperands(char *str);                  // count how many unique operands are in the expression
int *negation(int arr[], int size);                  // perform a negations on the array
int *conjunction(int arr[], int arr2[], int size);   // perform a conjunction operation on the 2 array variables
int *disjunction(int arr[], int arr2[], int size);   // perform a disjunction operation on the 2 array variables
int *implication(int arr[], int arr2[], int size);   // perform a implication operation on the 2 array variables
int *biconditional(int arr[], int arr2[], int size); // perform a biconditional operation on the 2 array variables

Queue *infixToPostfix(char *str); // main function to transform infix expression into postfix

int main()
{
  int solve = 1;
  int a = 1;
  while (solve)
  {
    char *infix; // string with the infix expression
    infix = malloc(1024 * sizeof(char));
    printf("Expression: ");
    scanf("%[^\n]", infix);
    infix = realloc(infix, strlen(infix) + 1);
    removeSpaces(infix);                    // removing blank spaces
    int count = countUniqueOperands(infix); // getting the total of operands
    int size = pow(2, count);               // number of variables a proposition will have

    Queue *postfix = infixToPostfix(infix); // converting the expression

    LinkedList *operands = NULL; // List of operands with its truth table values

    int hash[128] = {0};
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
        insertLast(&operands, i, count, c); // Setting up each variable value
        c++;
      }
    }

    Stack *stack = NULL;
    char op;
    // perform operations
    while (postfix->head != NULL)
    {
      op = deQueue(postfix);
      // if its an operand add to the stack
      if (isOperand(op))
        pushArray(&stack, getOperandArray(operands, op));
      // if its an operator perform the operations
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

    int *res = popArray(&stack); // getting the final result array from the stack

    // printing the answer
    for (int i = 0; i < size; i++)
      printf("%d \n", res[i]);

    fflush(stdin);
    printf("Press ENTER key to Continue\n");
    getchar();
  }

  return 0;
}

Queue *infixToPostfix(char *str)
{
  Stack *stack = NULL;                           // Initializing the Stack
  Queue *queue = (Queue *)malloc(sizeof(Queue)); // Initializing the Queue
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

// UTIL
void removeSpaces(char *str)
{
  char *temp = str;
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
  int hash[128] = {0}; // there is 128 characters in C - each represented by a unique integer value -> ASCII value
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

int *negation(int arr[], int size)
{
  for (int i = 0; i < size; i++)
  {
    arr[i] = !arr[i];
  }

  return arr;
}

int *conjunction(int arr[], int arr2[], int size)
{
  int *res = (int *)malloc(size * sizeof(int));

  for (int i = 0; i < size; i++)
  {
    res[i] = arr[i] && arr2[i];
  }

  return res;
}

int *disjunction(int arr[], int arr2[], int size)
{
  int *res = (int *)malloc(size * sizeof(int));

  for (int i = 0; i < size; i++)
  {
    res[i] = arr[i] || arr2[i];
  }

  return res;
}

int *implication(int arr[], int arr2[], int size)
{
  int *res = (int *)malloc(size * sizeof(int));

  for (int i = 0; i < size; i++)
  {
    res[i] = !arr[i] || arr2[i];
  }

  return res;
}

int *biconditional(int arr[], int arr2[], int size)
{
  int *res = (int *)malloc(size * sizeof(int));

  for (int i = 0; i < size; i++)
  {
    res[i] = arr[i] == arr2[i];
  }

  return res;
}