#include "../headers/type.h"
#include "../headers/util.h"
#include "../headers/global.h"
#include "../headers/address.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

node *createNode(char *content, char *name)
{
    node *newNode = (node *)malloc(sizeof(node));
    if (newNode == NULL)
    {
        printf("Failed to allocate memory location\n");
    }

    newNode->name = malloc(sizeof(char) * strlen(name));
    strcpy(newNode->name, name);
    newNode->content = allocateMemoryForChar(strlen(content));
    strcpy(newNode->content, content);
    newNode->next = NULL;

    return newNode;
}

void printNode(node *ref)
{
    printf("Printing node...\n");
    printf("Node name: %s\n", ref->name);
    printf("Node content: %s\n", ref->content);
}

void printAllNodes(node **head)
{
    node *current = *head;
    if (current == NULL)
    {
        return;
    }

    while (current != NULL)
    {
        printNode(current);
        current = current->next;
    }
}

node *findNodeIfExists(node **head, char *name)
{
    node *current = *head;

    while (current != NULL)
    {
        if (!strcmp(current->name, name))
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void setNextNode(node **head, char *name, char *content)
{
    node *newNode = createNode(content, name);
    node *current = findNodeIfExists(head, name);
    node *temp = *head;
    if (current == NULL)
    {
        if (*head == NULL)
        {
            *head = newNode;
            return;
        }

        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = newNode;
        return;
    }

    /** As long as node next exists and its name doesnt equal provided parameter name, proceed to next node*/
    while (current->next != NULL && !strcmp(current->name, name))
    {
        current = current->next;
    }

    /** If reached end of linked list, set new node at end*/
    if (current->next == NULL)
    {
        current->next = newNode;
    }
    else
    {
        /** If reached last node with same name, set this new node after it*/
        newNode->next = current->next;
        current->next = newNode;
    }
}

void freeMacros(node **head)
{
    node *current = *head;
    node *temp;

    while (current != NULL)
    {
        temp = current->next;
        free(current->name);
        free(current->content);
        free(current);
        current = temp;
    }
}

symbolTable *findLabelByName(symbolTable **symbolT, char *label)
{

    symbolTable *current = *symbolT;
    if (current == NULL)
    {
        return NULL;
    }

    while (current != NULL)
    {
        if (!strcmp(current->name, label))
        {
            return current;
        }

        current = current->next;
    }
    return NULL;
}

void setLabelInSymbolTable(symbolTable **symbolT, char *label, int line, int isExtern, int isEntry)
{
    symbolTable *current = *symbolT;

    label = trimSpaces(label);

    if (current == NULL)
    {
        *symbolT = createNewLabelSymbol(label, line, isExtern, isEntry);
        return;
    }

    while (current->next != NULL)
    {
        if (!strcmp(current->name, label))
        {
            if (current->is_entry == TRUE)
            {
                current->line = line;
                return;
            };

            if (isEntry == TRUE)
            {
                current->is_entry = TRUE;
                return;
            }
        }
        current = current->next;
    }

    current->next = createNewLabelSymbol(label, line, isExtern, isEntry);
}

symbolTable *createNewLabelSymbol(char *label, int line, int isExtern, int isEntry)
{
    symbolTable *current = malloc(sizeof(symbolTable));
    if (current == NULL)
    {
        printf("ERROR | Failed to allocate memory location for new label symbol \n");
        return NULL;
    }

    current->is_entry = FALSE;
    current->is_extern = FALSE;
    (current)->name = allocateMemoryForChar(strlen(label));

    strcpy((current)->name, label);

    (current)->line = line;
    (current)->next = NULL;

    if (isEntry)
    {
        current->is_entry = TRUE;
    }

    if (isExtern)
    {
        current->is_extern = TRUE;
    }

    return current;
}

void printSymbolTable(symbolTable **symbolT)
{
    symbolTable *current = *symbolT;
    if (*symbolT == NULL)
    {
        return;
    }

    printf(" ---- Printing symbol table ----\n");
    while (current != NULL)
    {
        printf("Symbol: %s LINE - %d\n", current->name, current->line + IC_INITIAL_VALUE);
        current = current->next;
    }
}

void freeSymbolTable(symbolTable **symbolT)
{
    symbolTable *current = *symbolT;
    symbolTable *temp;

    while (current != NULL)
    {
        temp = current->next;
        free(current->name);
        free(current);
        current = temp;
    }
}

void freeDataCode(binaryCode **head, int length)
{
    int i;
    for (i = 0; i < length; i++)
    {
        if ((*head + i) != NULL)
        {
            free((*head + i));
        }
    }
    printf("Freeeing code\n");
}

void freeDataImage(dataImage **head, int length)
{
    int i;
    for (i = 0; i < length; i++)
    {

        if ((*head + i) != NULL)
        {
            if ((*head + i)->label != NULL)
            {

                free((*head + i)->label);
            }
            free((*head + i));
        }
    }
    printf("Freeeing data image\n");
}
void printDataImage(dataImage **head, int length)
{
    dataImage *current = *head;
    int i;
    printf(" ---- Printing data image ----\n");

    for (i = 0; i < length; i++)
    {
        if (current[i].is_data)
        {
            printf("Image: %d\n", current[i].value);
        }
        else
        {
            printf("Image: %c\n", current[i].value);
        }
    }
}

int allocateMemoryCode(binaryCode **dataCode, int length)
{
    *dataCode = (binaryCode *)realloc(*dataCode, sizeof(binaryCode) * length);
    if (*dataCode == NULL)
    {
        printf("ERROR: Failed to re allocate memory\n");
        return FALSE;
    }
    return TRUE;
}

int saveCode(binaryCode **dataCode, unsigned short code, int *IC, char *label, int shouldRecode)
{
    if (allocateMemoryCode(dataCode, *IC + 1))
    {
        (*dataCode + *IC)->num = code;
        (*dataCode + *IC)->recode = shouldRecode;

        if (label == NULL)
        {
            (*dataCode + *IC)->label = NULL;
        }
        else
        {
            (*dataCode + *IC)->label = allocateMemoryForChar(strlen(label));
            strcpy((*dataCode + *IC)->label, label);
        }

        (*IC)++;
        return TRUE;
    }
    return FALSE;
}

void printCode(binaryCode **code, int length)
{
    binaryCode *current = *code;
    int i;
    printf(" ---- Printing code image ----\n");

    for (i = 0; i < length; i++)
    {
        printf("Line %d\t %u %s IC:%d\n", i + 100, current[i].num, current[i].label, i);
        printBinary(current[i].num);
    }
}