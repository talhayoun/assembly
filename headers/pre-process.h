
#include <stdio.h>

/**
 * This functions starts the pre process which replaces all defined macros with their values
 * @param macros - A pointer to macros table
 * @param fptr - A pointer to assembly filen
 * @param newFile - A pointer to write new assembly file without macros[]
 * @return boolean
 */
int initiatePreProcess(node *macros, FILE *fptr, char *fileName);

/**
 * This function accepts copyMacro as argument which its content should
 *  be copied to the file and replaced with macro declartion
 * @param file - A pointer to file to write content
 * @param copyMacro - Macro to copy its content
 */
void replaceMacrosWithContent(FILE *file, node *copyMacro);

/**
 * This function reads line by line in .as file and looks for macros
 * then copies macro content without macro declartion
 * @param macros - A pointer to macros table
 * @param fptr - A pointer to as file
 * @param amFile - A pointer to am file
 * @returns Boolean for success or failure
 */

int appendFileLinesWithoutMacroDeclaration(node *macros, FILE *fptr, FILE *amFile);

/**
 * This function looks for existing macro provided as arugment macroName
 * and if found, it replaces macro declartion with its content
 * @param macroName - Macro name to look for
 * @param macros - A pointer to macros table
 * @param file - A pointer to file
 */
int findMacroByNameAndReplace(char *macroName, node **macros, FILE *file);