#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./type.h"

/**
 * Begins first process
 * Saves labels in symbol table
 * Saves .data, .string, .extern, .entry types and its values
 * Converts code to binary
 * Saves code as binary
 * @param macros - A pointer to macros table
 * @param data - An array that contains instructions, registers and commands
 * @param fileName - File to parse
 * @param dataImg - A pointer to code image
 * @param symbolT - A pointer to symbol table
 * @param IC - Instruction counter
 * @param DC - Data counter
 * @param dataCode - A pointer to data code
 * @param error - whether found an error
 */
void executeFirstProcess(node *macros, dataObject data, char *fileName, dataImage **dataImg, symbolTable **symbolT, int *IC, int *DC, binaryCode **dataCode, int *error);

/**
 * Handler for .data | .string | .extern | .entry
 * @param macros - A pointer to macros table
 * @param data - An array that contains instructions, registers and commands
 * @param fileName - File to parse
 * @param dataImg - A pointer to code image
 * @param symbolT - A pointer to symbol table
 * @param IC - Instruction counter
 * @param DC - Data counter
 * @param dataCode - A pointer to data code
 * @return True if execute successfully
 */
int parseDirective(char *line, dataObject data, node *macros, int *DC, int *IC, dataImage **dataImg, symbolTable **symbolT, binaryCode **dataCode);

/**
 * @param dataImg - A pointer to code image
 * @param dataCode - A pointer to data code
 * @param numbers - An array of numbers in string
 * @param DC - Data counter
 * @param IC - Instruction counter
 * @param label - Associated label name
 */
void setNumbersInDataImage(dataImage **dataImg, binaryCode **dataCode, char *numbers, int *DC, int *IC, char *label);

/**
 * Splits string to characters and saves each one in a different memory location
 * @param dataImg - A pointer to code image
 * @param dataCode - A pointer to data code
 * @param str - A string
 * @param DC - Data counter
 * @param IC - Instruction counter
 * @param label - Associated label name
 */
void setStringInDataImage(dataImage **dataImg, binaryCode **dataCode, char *str, int *DC, int *IC, char *label);

/**
 * Parses file line by few steps
 * Extracts command and validates it
 * Extract operands and check if they are valid
 * Finds addresses type
 * Finds opcode
 * Codes operands to binary
 * Saves code
 * @param line - File line
 * @param dataObj - An array that contains instructions, registers and commands
 * @param macros - A pointer to macros table
 * @param symbolT - A pointer to symbol table
 * @param IC - Instruction counter
 * @param dataCode - A pointer to data code
 * @return True if parsed successfully
 */
int parseCommand(char *line, dataObject dataObj, node *macros, symbolTable **symbolT, int *IC, binaryCode **dataCode);

/**
 * Checks if str is a comment and validates it
 * @param line - File line
 * @param str - Label
 * @param dataObj - An array that contains instructions, registers and commands
 * @param macros - A pointer to macros table
 * @return True if validated
 */
int extractLabelIfExists(char *line, char **str, dataObject dataObj, node *macros);

/**
 * Converts address types to binary code in corresponding location
 * Saves binary code of opcode + address types
 * @param opcode - Command opcode
 * @param addressOne - Operand one address type
 * @param addressTwo - Operand two address type
 * @param dataCode - A pointer to data code
 * @param IC - Instruction counter
 */
int codeCommand(int opcode, int addressOne, int addressTwo, binaryCode **dataCode, int *IC);

/**
 * Incase both operands of address type 2 or 3, code them together
 * else code each operand and saves it
 * @param addressTypes - An array of address types
 * @param operands - An array of operands
 * @param symbolT - A pointer to symbol table
 * @param registers - An array of registers
 * @param dataCode - A pointer to data code
 * @param IC - Instruction counter
 */
int saveCommandTwoOperands(int addressType[], char **operands, symbolTable **symbolT, char **registers, binaryCode **code, int *IC);

/**
 * Code an instruction with one operand
 * @param addressTypes -  address type
 * @param operands - An array of operands
 * @param symbolT - A pointer to symbol table
 * @param registers - An array of registers
 * @param dataCode - A pointer to data code
 * @param IC - Instruction counter
 */
int saveCommandOneOperand(int addressType, char **operands, symbolTable **symbolT, char **registers, binaryCode **code, int *IC);

/**
 * Receives label in token parameter and saves label in symbol table
 * then sets lineWithoutLabel parameter without label definition
 * @param IC - Instruction counter
 * @param lineWithoutLabel - A pointer to string
 * @param line - Instruction string
 * @param token - Label
 * @param symbolT - A pointer to symbol table
 * @return boolean
 */
int saveLabel(int *IC, char **lineWithoutLabel, char *line, char *token, symbolTable **symbolT);

/**
 * Checks if file doesn't exceed maximum length
 * @param fileLine - File line
 * @return true if is valid
 */
int validateFileLineLength(char *fileLine);

/**
 * Accepts str and removes the first word out of it until there is space
 * @param str - A string
 * @return A reference to found command
 */
char *extractCommand(char *str);

/**
 * Removes leading and trailing parantheses
 * @param str - A string
 * @returns New string without parantheses
 */
char *removeParantheses(char *str);

/**
 * Checks if both operands are either address type 3 or 4
 * @param addressOne - Address type
 * @param addressTwo - Address type
 * @return boolean
 */
int areBothOperandsRegistersAddressType(int addressOne, int addressTwo);

/**
 * Validates provided operands src and target are allowed for specific command
 * @param src - Address type
 * @param target - Address type
 * @param command - Command
 * @param length - whether src and target exist or only one of them
 * @returns boolean
 */
int validateCommandOperands(int src, int target, cmd command, int length);

void populateDataImageCode(dataImage **dataImg, binaryCode **dataCode, symbolTable **symbolT, int *IC, int *DC);