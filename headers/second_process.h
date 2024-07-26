#include "../headers/type.h"

/**
 * Executes second process
 * Fills symbol table with yet filled symbol table location
 * Creates object file
 * Creates an extern file
 * Creates an entry file
 * @param labels - A reference to symbol table
 * @param dataCode - A reference to data code array
 * @param IC - Instruction counter
 * @param DC - Data counter
 * @param fileName - Current parsed file name
 */
void executeSecondProcess(symbolTable **labels, binaryCode **dataCode, int IC, int DC, char *fileName);

/**
 * Loops all code array and if element index has label
 * Either adds it to extern file or entry file
 * If recode sets to true, finds the new location and updates created symbol node
 * @param labels - A reference to symbol table
 * @param dataCode - A reference to data code array
 * @param IC - Instruction counter
 * @param fileName - Current parsed file name
 */
void populateSymbolCode(symbolTable **labels, binaryCode **dataCode, int IC, char *fileName);

/**
 * Creates an entry file
 * @param head - A reference to symbol table
 * @param fileName - Current parsed file name
 */
void createEntry(symbolTable **head, char *fileName);

/**
 * Creates an extern file
 * @param label - A reference to symbol table
 * @param line - Current line
 * @param fileName - Current parsed file name
 */
void createExtern(symbolTable *label, int line, char *fileName);

/**
 * Creates an object file
 * @param dataCode - A reference to data code array
 * @param IC - Instruction counter
 * @param DC - Data counter
 * @param fileName - Current parsed file name
 */
void createObjectFile(binaryCode **dataCode, int IC, int DC, char *fileName);

/**
 * Converts binary code to octal
 * @param num - Binary code
 * @returns Octal num
 */
int convertBinaryToOctal(unsigned short num);

/**
 * Converts binary code to decimal
 * @param num - Binary code
 * @returns Decimal num
 */
int convertBinaryToDecimal(unsigned short num);