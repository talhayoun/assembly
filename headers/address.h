#include "type.h"

/**
 * Finds operand address type
 * @param operand - Operand value
 * @param dataObj - An object contains registers, instructions and commands
 * @param macros - A pointer to macros table
 * @returns address type value
 */
int findAddressNumber(char *operand, dataObject dataObj, node *macros);

/**
 * Finds provided command in commands array
 * @param command - Command string
 * @param commands - An array or commands
 * @returns Command index from commands array
 */
int findOpcode(char *command, cmd *commands);

/**
 * Converts address type number to match location in table
 * @param num - Address type number
 * @returns Address number to match correct location in table
 */
int convertAddressNumberToBinary(int num);

/**
 * Returns correct location of number for are codes
 * @param isExternal - Is line external
 * @param isEntry - Is line entry
 * @param isAbsolute - Is line absolute
 * @returns Are code
 */
int getAreCode(int isExternal, int isEntry, int isAbsolute);

void printBinary(unsigned short num);

/**
 * Converts number to unsigned short with mask
 * @param num - Number
 * @param mask - Mask value
 * @returns Unsigned short value
 */
unsigned short convertDecimalToBinary(int num, int mask);

/**
 * @param op - Operand
 * @param address - Address type
 * @param symbolT - A pointer to symbol table
 * @returns - Returns code populated ARE and rest of num
 */
unsigned short getCodeByAddress(char *op, int address, symbolTable **symbolT);

/**
 * Returns code based on address type
 * Returns 0 if label is not coded yet
 * Returns -1 incase error
 * @param address - Address type
 * @param operands - A pointer to operands string
 * @param symbolT - A pointer to symbol table
 * @param registers - An array of registers
 * @param byte - Mask
 */
unsigned short codeByAddress(int address, char *operands, symbolTable **symbolT, char **registers, int byte);