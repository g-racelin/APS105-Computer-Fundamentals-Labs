#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

// Implement these functions
void handleInsert(LinkedList*);
void handleRemove(LinkedList*);
void handleCurrentCal(LinkedList*, int);
void handleVoltage(LinkedList*, int);
void handlePrint(LinkedList*);
void handleQuit(LinkedList*);
Node * createNode (int value, char label []);
bool insertAtFront (LinkedList *circuit, int resistance, char label []);
void deleteAtFront (LinkedList * circuit);
double calculateCurrent (LinkedList *circuit, int voltage);

int main(void) {
  LinkedList circuit;
  initList(&circuit);
  char command = '\0';
  printf("Welcome to our circuit simulator\n");
  // Enter the source voltage!
  printf("What is the source of the voltage?\n");
  int voltage;
  readInputNumber(&voltage);

  while (command != 'Q') {
    command = getCommand();

    switch (command) {
      case 'I':
        handleInsert(&circuit);
        break;
      case 'R':
        handleRemove(&circuit);
        break;
      case 'C':
        handleCurrentCal(&circuit, voltage);
        break;
      case 'V':
        handleVoltage(&circuit, voltage);
        break;
      case 'P':
        handlePrint(&circuit);
        break;
      case 'Q':
        handleQuit(&circuit);
        break;
    }
  }
  return 0;
}

void handleInsert(LinkedList* circuit) {
  int resistance = 0;
  printf("What's the value of the resistor: ");
  readInputNumber(&resistance);
  printf("What's the label of the resistor: ");
  char labelName[STRING_MAX];
  readInputString(labelName, STRING_MAX);

  Node * current = circuit -> head;
  if (current == NULL || strcmp (circuit -> head -> name, labelName) > 0){
    insertAtFront (circuit, resistance, labelName);
  } else {
    while (current -> next != NULL && strcmp (current -> next -> name, labelName) <= 0){ // while the new label is later in alphabet than current (stop when new label becomes earlier in alphabet
      current = current -> next;
      if (strcmp (current -> name, labelName) == 0){ //WHY DOES THIS NOT WORK
        printf ("A resistor with %s label already exists.", labelName);
        return;
      }
    }
    if (strcmp (current -> name, labelName) == 0){ //WHY DOES THIS NOT WORK
      printf ("A resistor with %s label already exists.", labelName);
      return;
    }
    Node * newNode = createNode (resistance, labelName);
    newNode -> next = current -> next;
    current -> next = newNode;
  }
}

void handleRemove(LinkedList* circuit) {
  printf ("What's the label of the resistor you want to remove: ");
  char labelName[STRING_MAX];
  readInputString(labelName, STRING_MAX);
  if (circuit -> head == NULL){
    printf ("The resistor with %s label does not exist. \n", labelName);
    return;
  }
  if (strcmp (circuit -> head -> name, labelName) == 0){
    deleteAtFront (circuit);
    return;
  } 
  Node *current = circuit -> head;
  while (current -> next != NULL && strcmp (current -> next -> name, labelName) != 0){
    current = current -> next;
  }
  if (current -> next != NULL){
    Node *temp = current -> next;
    current -> next = temp -> next;
    free (temp);
    return;
  } else {
    printf ("The resistor with %s label does not exist. \n", labelName);
  }
}

void handleCurrentCal(LinkedList* circuit, int voltage) {
  double i;
  i = calculateCurrent (circuit, voltage);
  printf ("The current in the circuit is %.6lfA \n", i); 
}

void handleVoltage(LinkedList* circuit, int voltage) {
  double i, v; 
  int resistance;
  i = calculateCurrent (circuit, voltage);
  printf ("What's the label of the resistor you want to find the voltage across: ");
  char labelName[STRING_MAX];
  readInputString(labelName, STRING_MAX);
  Node *current = circuit -> head;
  if (strcmp (circuit -> head -> name, labelName) == 0){
    resistance = circuit -> head -> value;
    v = i*resistance;
    printf ("Voltage across resistor is %.6lfV \n", v);
    return;
  } else if (circuit -> head == NULL){
    printf ("The resistor with %s label does not exist. \n", labelName);
    return;
  }
  while (current -> next != NULL && strcmp (current -> next -> name, labelName) != 0){
    current = current -> next;
    if (current -> next != NULL){
      resistance = current -> next -> value;
      v = i*resistance;
      printf ("Voltage across resistor is %.6lfV \n", v);
      return;
    }
  }
  if (current -> next == NULL){
    printf ("The resistor with %s label does not exist. \n", labelName);
  }
}

void handlePrint(LinkedList* circuit) {
  Node * current = circuit -> head;
  while (current != NULL){
    printf ("%s %d Ohms\n", current -> name, current -> value);
    current = current -> next;
  }
}

void handleQuit(LinkedList* circuit) {
  printf ("Removing all resistors in the circuit...\n");
  handlePrint (circuit);
  while (circuit -> head != NULL) {
    deleteAtFront (circuit);
  }
  circuit -> head = NULL;
}

Node * createNode (int resistance, char label []){
  Node * newNode = (Node *) malloc (sizeof (Node));
  if (newNode != NULL){
    newNode -> value = resistance; 
    strcpy (newNode -> name, label);
    newNode -> next = NULL;
  }
  return newNode;
}

bool insertAtFront (LinkedList *circuit, int resistance, char label []){
  Node * temp = createNode (resistance, label);
  if (temp == NULL){
    return false;
  }
  temp -> next = circuit -> head;
  circuit -> head = temp;
  return true;
}

void deleteAtFront (LinkedList *circuit){
  if (circuit -> head == NULL){
    return;
  }
  Node * newHead = circuit -> head -> next;
  free (circuit -> head);
  circuit -> head = newHead; 
}

double calculateCurrent (LinkedList *circuit, int voltage){
  Node *current = circuit -> head; // consider the special cases for this
  int totalR = 0;
  double i;
  while (current -> next != NULL){
    totalR += current -> value;
    current = current -> next;
  }
  totalR += current -> value;
  i = (double)(voltage) / totalR;
  return i;
}