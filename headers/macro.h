#ifndef MACRO_H
#define MACRO_h
#include "./global.h"
#include "./data.h"
#include "./type.h"
#include <stdio.h>

/**
 * Finds if macro name is valid or not
 * Cannot be either a command or instruction value
 * @param macroName - Macro name
 * @param commands - A pointer to commands array
 * @param instructions - A pointer to instructions array
 * @return True or False ( 1 / 0)
 */
int isMacroNameIlegal(char *value, cmd *commands, char **instructions);

/**
 * Checks if value has any characters before/after its declartion
 * @param value - A string
 * @return True or False ( 1 / 0)
 */

int hasExtraCharactersAfterValue(char *value);

/**
 * Finds if this macro is valid and returns its name in new memory
 * @param macro - Macro name
 * @param data - An object that contains registers, instructions and commands array
 * @return A reference to macro name
 */
char *validateMacroNameAndSaveIt(char *macro, dataObject data);

/**
 * This functions adds a new macro to macros table by savings its content
 * @param fptr - A pointer to am file
 * @param fileLine - A line from as file
 * @param name - Macro name
 * @param head - A pointer to macros table
 * @returns Boolean
 */
int addNewMacro(FILE *fptr, char *fileLine, char *name, node **head);

/**
 * Scans file (fptr) for macros and returns a pointer macros head table
 * @param fptr - A pointer to file to look for macros
 * @param data - An object that contains instructions, registers and commands
 * @param error - Sets error to true incase found error to abort program.
 * @return A new macro node pointer
 */

node *scanForMacro(FILE *fptr, dataObject data, int *error);

/**
 * Finds if there is a macro with the same name provided in value paremeter
 * @param macros - A pointer to linked list of macros
 * @param value - Name to compare
 * @return A reference to found node or null
 */
node *findMacroByName(node **macros, char *value);
#endif