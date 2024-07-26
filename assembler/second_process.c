#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../headers/util.h"
#include "../headers/address.h"
#include "../headers/second_process.h"
void executeSecondProcess(symbolTable **labels, binaryCode **dataCode, int IC, int DC, char *fileName)
{
    populateSymbolCode(labels, dataCode, IC, fileName);
    createEntry(labels, fileName);
    createObjectFile(dataCode, IC, DC, fileName);
}

void populateSymbolCode(symbolTable **labels, binaryCode **dataCode, int IC, char *fileName)
{
    binaryCode *current = *dataCode;
    symbolTable *label;
    char *externFileName = concatString(fileName, EXTERN_EXTENSION);
    unsigned short num;
    int entryNum = getAreCode(FALSE, TRUE, FALSE), i;

    for (i = 0; i < IC; i++)
    {
        if (current[i].label != NULL)
        {

            label = findLabelByName(labels, current[i].label);
            if (label == NULL)
            {
                printf("ERROR: Haven't found label by name %s \n", current[i].label);
                continue;
            }

            if (current[i].num == entryNum)
            {
                (*dataCode + i)->num = convertDecimalToBinary(label->line + IC_INITIAL_VALUE, REGISTER_BIT_THREE) | (*dataCode + i)->num;
            }
            else if (label->is_extern == TRUE)
            {
                (*dataCode + i)->num = convertDecimalToBinary(getAreCode(TRUE, FALSE, FALSE), ARE_SHIFT);
                createExtern(label, i + IC_INITIAL_VALUE, externFileName);
            }
            else if (current[i].recode)
            {

                num = convertDecimalToBinary(label->line + IC_INITIAL_VALUE, REGISTER_BIT_THREE) | convertDecimalToBinary(getAreCode(FALSE, TRUE, FALSE), ARE_SHIFT);
                (*dataCode + i)->num = num;
            }
        }
    }

    free(externFileName);
}

void createEntry(symbolTable **head, char *fileName)
{
    int i, found = FALSE;
    FILE *entryFile;
    symbolTable *current = *head;
    char *entryFileName = concatString(fileName, ENTRY_EXTENSION);
    entryFile = fopen(entryFileName, "w");

    if (entryFile == NULL)
    {
        printf("ERROR: Failed to open extern file\n");
        return;
    }

    for (i = 0; current != NULL; i++)
    {
        if (current->is_entry)
        {
            found = TRUE;
            fprintf(entryFile, "%4s \t", current->name);
            fprintf(entryFile, "%04d \n", current->line + IC_INITIAL_VALUE);
        }
        current = current->next;
    }

    fclose(entryFile);
    if (!found)
    {
        remove(entryFileName);
    }

    free(entryFileName);
}

void createExtern(symbolTable *label, int line, char *fileName)
{
    FILE *externFile;
    int found = FALSE;
    externFile = openFileAndCheckIfSuccess(fileName, "a");

    if (externFile != NULL)
    {

        found = TRUE;
        fprintf(externFile, "%4s \t", label->name);
        fprintf(externFile, "%04d \n", line);

        fclose(externFile);

        if (!found)
        {
            remove(fileName);
        }
    }
}

void createObjectFile(binaryCode **dataCode, int IC, int DC, char *fileName)
{
    int i;
    FILE *objFile;
    char *objFileName = concatString(fileName, OBJ_EXTENSION);
    objFile = openFileAndCheckIfSuccess(objFileName, "w");

    if (objFile != NULL)
    {
        fprintf(objFile, "\t%d", IC - DC);
        fprintf(objFile, "\t%d\n", DC);

        for (i = 0; i < IC; i++)
        {
            fprintf(objFile, "%04d \t", i + IC_INITIAL_VALUE);
            fprintf(objFile, "%05d\n", convertBinaryToOctal((*dataCode)[i].num));
        }
    }
    fclose(objFile);
    free(objFileName);
}

int convertBinaryToOctal(unsigned short num)
{
    unsigned short value;
    int mask = 7;
    int i;
    int octal[5] = {0};
    int result = 0;

    for (i = 4; i >= 0; i--)
    {
        value = num & mask;
        num >>= 3;
        octal[i] = convertBinaryToDecimal(value);
    }

    for (i = 0; i < 5; i++)
    {
        result = 10 * result + convertBinaryToDecimal(octal[i]);
    }
    return result;
}

int convertBinaryToDecimal(unsigned short num)
{
    int value = 0;
    int current = num;
    int fraction;
    int power = 1;
    while (current)
    {
        fraction = current % 10;
        current = current / 10;
        value = value + (fraction * 1);
        power = power * 2;
    }

    return value;
}