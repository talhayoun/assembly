#include "../headers/util.h"
#include "../headers/global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int isMacroNameIlegal(char *macroName, cmd *commands, char **instructions)
{
    /** Validates if macroName equals either one of the instructions or commmands*/
    char *instruction = findInstruction(macroName, instructions);
    int commandIndex = findCommand(macroName, commands);
    if (instruction || commandIndex != -1)
    {
        return TRUE;
    };

    return FALSE;
}

int hasExtraCharactersAfterValue(char *value)
{
    int i;
    int hasSpace = 0;
    int length = strlen(value);
    for (i = 0; i < length; i++)
    {
        if (hasSpace && !isspace(value[i]))
        {
            printf("ERROR: Found extra characters for (%s)\n", value);
            return TRUE;
        }

        if (isspace(value[i]))
        {
            hasSpace = 1;
        }
    }

    return FALSE;
}

char *validateMacroNameAndSaveIt(char *macro, dataObject data)
{
    char *name;
    macro = strtok(NULL, "");
    if (macro != NULL)
    {
        name = allocateMemoryForChar(strlen(macro));

        if (name == NULL)
        {
            return NULL;
        }

        if (!hasExtraCharactersAfterValue(macro) && !isMacroNameIlegal(name, data.commands, data.instructions))
        {
            strcpy(name, macro);
            return name;
        }
    }
    return NULL;
}

int addNewMacro(FILE *fptr, char *fileLine, char *name, node **head)
{
    char *content;
    /** Reads next line after macro declaration*/
    fileLine = readLineFromFile(fptr, fileLine);

    /** As long as line doesnt equal endmacr, loop lines*/
    while (strstr(fileLine, ENDMACR) == NULL)
    {
        content = allocateMemoryForChar(strlen(fileLine));
        if (content == NULL)
        {
            return FALSE;
        }

        strcpy(content, fileLine);
        setNextNode(head, name, content); /** Saves macro content */
        fileLine = readLineFromFile(fptr, fileLine);
    }

    if (hasExtraCharactersAfterValue(fileLine))
    {
        free(content);
        return FALSE;
    }
    return TRUE;
}

node *scanForMacro(FILE *fptr, dataObject data, int *error)
{
    char *fileLine = allocateMemoryForChar(READ_LINE_LENGTH);
    char *token, *name;
    node *head = NULL;

    if (fileLine == NULL)
    {
        *error = TRUE;
        return NULL;
    }

    do
    {
        fileLine = readLineFromFile(fptr, fileLine);

        if (isLineAComment(fileLine, FALSE))
        {
            continue;
        }

        /**Splits string by spaces*/
        token = strtok(fileLine, " ");

        if (!strcmp(token, MACR))
        {
            /** Validates macro name is legal and copies it to new memory allocation */
            name = validateMacroNameAndSaveIt(token, data);

            if (name == NULL || !addNewMacro(fptr, fileLine, name, &head))
            {
                free(head);
                free(name);
                *error = TRUE;
                return NULL;
            };

            free(name);
        }

    } while (!feof(fptr));
    return head;
}

node *findMacroByName(node **macros, char *value)
{
    return findNodeIfExists(macros, value);
}
