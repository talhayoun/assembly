#include "../headers/macro.h"
#include "../headers/util.h"
#include "../headers/pre-process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int initiatePreProcess(node *macros, FILE *fptr, char *fileName)
{
    FILE *newFile = openFileAndCheckIfSuccess(fileName, "w");
    int hasAppendedSuccessfully = FALSE;

    if (fptr == NULL)
        return FALSE;

    hasAppendedSuccessfully = appendFileLinesWithoutMacroDeclaration(macros, fptr, newFile);

    fclose(newFile);
    fclose(fptr);

    if (!hasAppendedSuccessfully)
    {
        return FALSE;
    }

    return TRUE;
}

void replaceMacrosWithContent(FILE *file, node *copyMacro)
{
    char *line;

    while (copyMacro != NULL)
    {
        /** Copies macro content to file */
        line = strcat(copyMacro->content, "\n");
        fprintf(file, line);
        copyMacro = copyMacro->next;
    }
}

int appendFileLinesWithoutMacroDeclaration(node *macros, FILE *fptr, FILE *amFile)
{
    char *fileLine = allocateMemoryForChar(READ_LINE_LENGTH);
    char *token, *copiedLine = allocateMemoryForChar(READ_LINE_LENGTH);
    int foundMacro = 0;

    if (fileLine == NULL || copiedLine == NULL)
    {
        return FALSE;
    }

    rewind(fptr);

    do
    {
        readLineFromFile(fptr, fileLine);

        /** Copies file line to a temp variable, so wont effect original line*/
        strcpy(copiedLine, fileLine);

        /** Splits file line by spaces*/
        token = strtok(copiedLine, " ");
        if (!strcmp(token, MACR))
        {
            /** If token equals macr ( macro declaration), we should skip this line and not add it to the file*/
            foundMacro = 1;
            continue;
        };

        if (!strcmp(token, ENDMACR))
        {
            /** If token equals endmacr ( end of macro declaration), we should skip this and not add it to the new file*/
            foundMacro = 0;
            continue;
        }

        if (!foundMacro)
        {
            /** Finds if token equals any macro name, if so, adds macro content to file */
            if (findMacroByNameAndReplace(token, &macros, amFile))
            {
                continue;
            }

            /** If doesn't equal macro and is regular line, add it*/
            if (!feof(fptr))
            {
                fileLine = strcat(fileLine, "\n");
            }

            fprintf(amFile, fileLine);
        }

    } while (!feof(fptr));
    return TRUE;
}

int findMacroByNameAndReplace(char *macroName, node **macros, FILE *file)
{
    node *macro;
    int found = 0;
    while (macroName != NULL)
    {
        /** Finds macro by name */
        macro = findMacroByName(macros, macroName);
        if (macro != NULL)
        {
            /** If found macro, replace macro declartion with its content */
            replaceMacrosWithContent(file, macro);
            found = 1;
        }
        macroName = strtok(NULL, " ");
    }

    return found;
}