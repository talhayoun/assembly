#include "./type.h"

struct list_el
{
    char *content;
    char *name;
    struct list_el *next;
};

typedef struct list_el node;

/**
 * Creates a new node and assigns name and content to it
 * @param content -  Node content
 * @param name - Node name
 * @returns Returns a reference to the newly created node
 */
node *createNode(char *content, char *name);

/**
 * Prints node name and content
 * @param ref - Node reference
 */
void printNode(node *ref);

/**
 * Prints all node children
 * @param head - Node head
 */
void printAllNodes(node **head);

/**
 * Free all nodes
 * @param head - Node head
 */
void freeMacros(node **head);

/**
 * Loops nodes and searches for a node the same name as parameter
 * @param head - Node head
 * @param name - A string to look for
 * @returns Returns a reference to found node or NULL
 */
node *findNodeIfExists(node **head, char *name);

/**
 * Creates a new node and sets it in nodes array
 * @param head - Node head
 * @param name - A node name
 * @param content - Node content
 */
void setNextNode(node **head, char *name, char *content);

/**
 * Creates a new symbol label
 * @param label - A string label name
 * @param line - Line number
 * @param isExtern - Whether this symbol is extern or not
 * @param isEntry - Whether this symbol is entry or not
 * @returns A reference to newly created symbol
 */
symbolTable *createNewLabelSymbol(char *label, int line, int isExtern, int isEntry);

/**
 * Creates a new symbol label and saves it in symbol table
 * @param symbolT - A pointer to symbol table
 * @param label - Label name
 * @param line - Label line
 * @param isExtern - Whether this symbol is extern or not
 * @param isEntry - Whether this symbol is entry or not
 */
void setLabelInSymbolTable(symbolTable **symbolT, char *label, int line, int isExtern, int isEntry);

/**
 * Prints symbol table
 * @param symbolT - A pointer to symbol table
 */
void printSymbolTable(symbolTable **symbolT);

/**
 * Frees symbol table memory location
 * @param symbolT - A pointer to symbol table
 */
void freeSymbolTable(symbolTable **symbolT);

/**
 * Frees data code memory location
 * @param head - A pointer to data code
 * @param length - Array length
 */
void freeDataImage(dataImage **head, int length);

/**
 * Prints data code memory location
 * @param head - A pointer to data code
 * @param length - Array length
 */
void printDataImage(dataImage **head, int length);

/**
 * Attempts to relloc memory location for data code if fails prints error
 * @param dataCode - A pointer to binary code array
 * @param length - Array length
 */
int allocateMemoryCode(binaryCode **dataCode, int length);

/**
 * Allocates memory location for data code array and saves new one
 * Incase operand is label, assign label to its label name
 * Sets recode to true incase label is extern or is entry or not in symbol table yet
 * @param dataCode - A pointer to data code array
 * @param code - Binary code
 * @param IC - Instruction code
 * @param label - Label name
 * @param shouldRecode - Boolean ( True | False)
 * @returns Boolean
 */
int saveCode(binaryCode **dataCode, unsigned short code, int *IC, char *label, int shouldRecode);

/**
 * Attempts to find label by name
 * @param symbolT - A reference to symbol table
 * @param label - Label name
 * @returns A reference to found label or NULL
 */
symbolTable *findLabelByName(symbolTable **symbolT, char *label);

/**
 * Prints all code array
 * @param code - A reference to code array
 * @param length - Code array length
 */
void printCode(binaryCode **code, int length);

/**
 * Frees data code array memory
 * @param head - A reference to code array
 * @param length - Array length
 */
void freeDataCode(binaryCode **head, int length);