#include "../headers/macro.h"
#include "../headers/pre-process.h"
#include "../headers/first_process.h"
#include "../headers/second_process.h"
#include "../headers/address.h"
#include "../headers/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *asFilePtr;
    node *macros;
    cmd commands[] = {
        {MOV_CMD, 0, 2, {0, 1, 2, 3}, 4, {1, 2, 3}, 3},
        {CMP_CMD, 1, 2, {0, 1, 2, 3}, 4, {0, 1, 2, 3}, 4},
        {ADD_CMD, 2, 2, {0, 1, 2, 3}, 4, {1, 2, 3}, 3},
        {SUB_CMD, 3, 2, {0, 1, 2, 3}, 4, {1, 2, 3}, 3},
        {LEA_CMD, 4, 2, {1}, 1, {1, 2, 3}, 3},
        {CLR_CMD, 5, 1, {0}, 0, {1, 2, 3}, 3},
        {NOT_CMD, 6, 1, {0}, 0, {1, 2, 3}, 3},
        {INC_CMD, 7, 1, {0}, 0, {1, 2, 3}, 3},
        {DEC_CMD, 8, 1, {0}, 0, {1, 2, 3}, 3},
        {JMP_CMD, 9, 1, {0}, 0, {1, 2}, 2},
        {BNE_CMD, 10, 1, {0}, 0, {1, 2}, 2},
        {RED_CMD, 11, 1, {0}, 0, {1, 2, 3}, 3},
        {PRN_CMD, 12, 1, {0}, 0, {0, 1, 2, 3}, 4},
        {JSR_CMD, 13, 1, {0}, 0, {1, 2}, 2},
        {RTS_CMD, 14, 0, {0}, 0, {0}, 0},
        {STOP_CMD, 15, 0, {0}, 0, {0}, 0},
    };

    char *registers[] = {REG_0, REG_1, REG_2, REG_3, REG_4, REG_5, REG_6, REG_7};

    char *instructions[] = {DATA, STRING, ENTRY, EXTERN};

    dataImage *dataImg = NULL;
    symbolTable *symbolT = NULL;
    binaryCode *dataCode = NULL;

    char *asFileName, *amFileName;
    int IC = 0, DC = 0, i = 1;
    int error = FALSE;
    /** Sets data object */
    dataObject data;
    data.commands = commands;
    data.registers = registers;
    data.instructions = instructions;

    while (i < argc)
    {
        dataImg = NULL;
        symbolT = NULL;
        dataCode = NULL;
        IC = 0;
        DC = 0;
        error = FALSE;
        asFileName = concatString(argv[i], AS_EXTENSION); /** Copies argument file name and appends .as to it */
        if (asFileName == NULL)
            continue;

        printf("Current file being parsed %s\n", asFileName);

        amFileName = concatString(argv[i], AM_EXTENSION); /**Copying argument file then adding .am extension */
        if (amFileName == NULL)
        {
            free(asFileName);
            continue;
        }

        asFilePtr = openFileAndCheckIfSuccess(asFileName, "r");

        if (asFilePtr == NULL)
        {
            free(asFileName);
            free(amFileName);
            i++;
            continue;
        }
        printf("Scanning for macros....\n");
        macros = scanForMacro(asFilePtr, data, &error);

        printf("Initiating pre process....\n");
        if (!error && initiatePreProcess(macros, asFilePtr, amFileName))
        {
            printf("Initiating first process....\n");
            executeFirstProcess(macros, data, amFileName, &dataImg, &symbolT, &IC, &DC, &dataCode, &error);

            if (!error)
            {
                printf("Initiating second process....\n");
                executeSecondProcess(&symbolT, &dataCode, IC, DC, argv[i]);
            }
        }

        printf("Calling free nodes\n");
        freeMacros(&macros);
        freeSymbolTable(&symbolT);
        freeDataImage(&dataImg, DC);
        freeDataCode(&dataCode, IC);
        free(asFileName);
        free(amFileName);
        i++; /** Increments i for next file */
    }
    return 0;
}
