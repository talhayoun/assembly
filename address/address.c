#include "../headers/util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

int findAddressNumber(char *operand, dataObject dataObj, node *macros)
{
    char *copyOperand = allocateMemoryForChar(strlen(operand));

    strcpy(copyOperand, operand);

    if (*copyOperand == '#')
    {
        copyOperand++;
        if (*copyOperand == '-' || *copyOperand == '+')
        {
            copyOperand++;
        }

        while (*copyOperand)
        {
            if (!isdigit(*copyOperand))
            {
                printf("Invalid copyOperand in # address number\n");
                return -1;
            }
            copyOperand++;
        }
        return 0;
    }
    else if (*copyOperand == '*')
    {
        copyOperand++;
        if (!findRegister(copyOperand, dataObj.registers))
        {
            return -1;
        };
        return 2;
    }
    else if (findRegister(copyOperand, dataObj.registers))
    {
        return 3;
    }
    else if (validateLabel(copyOperand, dataObj, macros))
    {
        return 1;
    }
    return -1;
}

int findOpcode(char *command, cmd *commands)
{
    int commandIndex = findCommand(command, commands);
    int opcode;
    if (commandIndex == -1)
    {
        printf("ERROR: Failed to find command by opcode\n");
        return -1;
    }

    opcode = commands[commandIndex].opcode;

    return opcode;
}

unsigned short convertDecimalToBinary(int num, int mask)
{
    unsigned short op;

    op = num << mask;
    return op;
}

void printBinary(unsigned short num)
{
    int i;
    for (i = 14; i >= 0; i--)
    {
        if (num >> i & 1)
        {
            printf("1");
        }
        else
        {
            printf("0");
        }
    }
    printf("\n");
}

int convertAddressNumberToBinary(int num)
{
    if (num == 0)
    {
        return 1;
    }
    else if (num == 1)
    {
        return 2;
    }
    else if (num == 2)
    {
        return 4;
    }
    else if (num == 3)
    {
        return 8;
    }

    return 0;
}

int getAreCode(int isExternal, int isEntry, int isAbsolute)
{
    if (isAbsolute)
        return 4;

    if (isEntry)
        return 2;

    if (isExternal)
        return 1;

    return 0;
}

unsigned short getCodeByAddress(char *op, int address, symbolTable **symbolT)
{
    int num;
    int ARE;
    symbolTable *label;
    if (address == 0)
    {
        ARE = convertDecimalToBinary(getAreCode(FALSE, FALSE, TRUE), ARE_SHIFT);
        num = convertDecimalToBinary(atoi(op), SOURCE_REGISTER_SHIFT);
    }
    else if (address == 1)
    {
        label = findLabelByName(symbolT, op);
        /** If label is not created yet or label is entry, we don't know label line yet and should process it in second process */
        if (label == NULL || label->is_entry)
        {
            return 0;
        }

        ARE = convertDecimalToBinary(getAreCode(label->is_extern, label->is_entry || !label->is_extern ? TRUE : FALSE, FALSE), ARE_SHIFT);
        num = convertDecimalToBinary(label->line + IC_INITIAL_VALUE, SOURCE_REGISTER_SHIFT);
    }

    return num | ARE;
}

unsigned short codeByAddress(int address, char *operand, symbolTable **symbolT, char **registers, int byte)
{
    unsigned short value;
    switch (address)
    {
    case 0:
        operand++;
        value = getCodeByAddress(operand, address, symbolT);
        return value;
    case 1:
        value = getCodeByAddress(operand, address, symbolT);
        if (value == 0)
        {
            return 0;
        }
        return value;
    case 2:
        operand++;
    case 3:
        operand++;
        return convertDecimalToBinary(atoi(operand), byte) | convertDecimalToBinary(getAreCode(FALSE, FALSE, TRUE), ARE_SHIFT);
    }

    return -1;
}
