#ifndef UTIL_H
#define UTIL_H
#include "./type.h"
#include "./global.h"
#include <stdio.h>

/**
 * Trims spaces leading and trailing spaces from string
 * @param value - A string
 * @return A string without spaces
 */
char *trimSpaces(char *value);

/**
 * Reads a new line from file, removes initial spaces and new line created by fgets
 * @param fptr - Pointer to file
 * @param value - Reference to line extracted from file
 * @returns Reference to line after spaces and new line removal
 */
char *readLineFromFile(FILE *fptr, char *value);

/**
 * Removes new line from string
 * @param str - A string
 */
void removeBlankLine(char *str);

/**
 * This function loops commands values and check if parameter value equals any of them
 * @param value - String to compare
 * @param commands - An array of commands
 * @return Command index or -1 if not found
 */
int findCommand(char *value, cmd *commands);

/**
 * This function loops instructions values and check if parameter value equals any of them
 * @param value - String to compare
 * @param commands - A pointer to array of instructions
 * @return A reference to instruction
 */
char *findInstruction(char *value, char **instructions);

/** Finds register in registers array
 * @param value = string to check against
 * @param registers - An array with registers
 * @return A reference to register
 */
char *findRegister(char *value, char **registers);

/**
 * This functions accepts an array and looks for value in it
 * @param value - Finds value in arr
 * @param length - Array length
 * @return A reference to found index
 */
char *findByValue(char *value, char **arr, int length);

/**
 * Checks if this string doesn't contain any alpha or digit
 * @param str - A string
 * @return TRUE if empty
 */
int isEmptyLine(char *str);

/**
 * Checks if string starts with ; which is a comment
 * @param str - A string
 * @param printError - Boolean
 * @return TRUE if is a comment
 */
int isLineAComment(char *str, int printError);

/**
 * Looks if a string has two commas in a row
 * @param value - A string
 * @return FALSE if has more than single comma
 */
int validateSingleCommaBetweenOperands(char *value);

/**
 * Checks if label is valid
 * @param label - A string
 * @param data - An object that contains commands, instructions and registers arrays
 * @param macros - A pointer to macro table
 * @return False if label is not valid
 */
int validateLabel(char *label, dataObject data, node *macros);

/**
 * This function checks either for alpha numerical or numbers only based on parameter value
 * @param arr - A file line
 * @param numbersOnly - Indicator whether should be numbers only ( 1 equals numbers only are valid)
 * @return True if valid
 */
int validateStringSeperatedByCommas(char *arr, int numbersOnly);

/**
 * This function splits by comma and converts string to number and saves it in numbers array
 * @param numbers - A string seperated by commas
 * @param amount - How many numbers extracted
 * @return A reference to nums array
 */
int *extractNumbersSeperatedByCommas(char *numbers, int *amount);

/**
 * This function splits by comma and  saves it in ops array
 * @param operands - A string seperated by commas
 * @param amount - How many operands extracted
 * @return A reference to ops array
 */
char **extractOperandsSeperatedByCommas(char *operands, int *amount);

/**
 * This function validates string has leading and trailing parentheses
 * @param str - A string
 * @return TRUE if it has
 */
int validateParentheses(char *str);

/**
 * Copies str to a new memory location and concatenate extension
 * @param str - string name string
 * @param extension - An extension to add to the string
 * @return A reference to the new string
 */
char *concatString(char *str, char *extension);

/**
 * This functions attempts to open a file and return a reference
 * @param fileName - File name to open
 * @param mode - File open mode
 * @return return a reference to opened file
 */
FILE *openFileAndCheckIfSuccess(char *fileName, char *mode);

/**
 * This functions attempts to allocate memory location
 * @param length - Length of memory
 * @return A reference to the new char
 */
char *allocateMemoryForChar(int length);

/**
 * Checks if string has leading or trailing comma
 * @param str - A string
 * @throw An error
 * @return boolean
 */
int checkLeadingAndTrailingComma(char *str);
#endif