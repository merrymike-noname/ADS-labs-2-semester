#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    char key;
    struct Node* next;
} Node;

void insertElement(Node** head, char key) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->next = NULL;
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    Node* curr = *head;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = newNode;
}

void sortedInsert(Node** head, Node* newNode) {
    if (*head == NULL || newNode->key <= (*head)->key) {
        newNode->next = *head;
        *head = newNode;
        return;
    }

    Node* curr = *head;
    while (curr->next != NULL && newNode->key > curr->next->key) {
        curr = curr->next;
    }
    newNode->next = curr->next;
    curr->next = newNode;
}

void sortList(Node** head) {
    Node* sortedList = NULL;
    Node* curr = *head;
    while (curr != NULL) {
        Node* nextNode = curr->next;
        sortedInsert(&sortedList, curr);
        curr = nextNode;
    }
    *head = sortedList;
}

void printList(Node* head) {
    Node* curr = head;
    while (curr != NULL) {
        printf("%c ", curr->key);
        curr = curr->next;
    }
    printf("\n");
}

void freeList(Node* head) {
    Node* curr = head;
    while (curr != NULL) {
        Node* nextNode = curr->next;
        free(curr);
        curr = nextNode;
    }
}

int main() {
    Node* head = NULL;
    int n;
    char key;
    printf("Input number ov elements: ");
    scanf("%d", &n);

    printf("Input elements:\n");
    for (int i = 0; i < n; i++) {
        printf("Element %d: ", i + 1);
        scanf(" %c", &key);
        insertElement(&head, key);
    }

    printf("Default list: ");
    printList(head);
    sortList(&head);
    printf("Sorted list: ");
    printList(head);
    freeList(head);
    return 0;
}
