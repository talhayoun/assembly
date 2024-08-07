#include "../headers/util.h"
#include "../headers/global.h"
#include "../headers/data.h"
#include "../headers/first_process.h"
#include "../headers/address.h"
#include <ctype.h>

void executeFirstProcess(node *macros, dataObject data, char *fileName, dataImage **dataImg, symbolTable **symbolT, int *IC, int *DC, binaryCode **dataCode, int *error)
{
    FILE *fptr = openFileAndCheckIfSuccess(fileName, "r");
    char *fileLine = allocateMemoryForChar(MAXIMUM_LINE_LENGTH);
    if (fileLine == NULL)
    {
        fclose(fptr);
        return;
    }
    do
    {
        readLineFromFile(fptr, fileLine);

        if (!validateFileLineLength(fileLine) || isLineAComment(fileLine, TRUE) || isEmptyLine(fileLine, TRUE))
        {
            printf("File line %d\n", *IC);
            continue;
        }

        /** Checks if line is either .data | .string | .extern | .entry */
        if (strstr(fileLine, DATA) || strstr(fileLine, STRING) || strstr(fileLine, ENTRY) || strstr(fileLine, EXTERN))
        {
            if (!parseDirective(fileLine, data, macros, DC, IC, dataImg, symbolT, dataCode))
            {
                *error = TRUE;
            };
        }
        else
        {
            if (!parseCommand(fileLine, data, macros, symbolT, IC, dataCode))
            {
                *error = TRUE;
            };
        }

    } while (!feof(fptr));

    populateDataImageCode(dataImg, dataCode, symbolT, IC, DC);

    free(fileLine);
    fclose(fptr);
}

void populateDataImageCode(dataImage **dataImg, binaryCode **dataCode, symbolTable **symbolT, int *IC, int *DC)
{
    int i;
    symbolTable *temp;
    char *label = allocateMemoryForChar(MAXIMUM_LINE_LENGTH);

    for (i = 0; i < *DC; i++)
    {
        if ((*dataImg + i)->label != NULL && strcmp((*dataImg + i)->label, label))
        {
            strcpy(label, (*dataImg + i)->label);
            temp = findLabelByName(symbolT, (*dataImg + i)->label);
            temp->line = *IC;
        }

        if ((*dataImg + i)->is_data)
        {
            saveCode(dataCode, (*dataImg + i)->value, IC, NULL, FALSE);
        }
        else
        {
            saveCode(dataCode, (*dataImg + i)->value, IC, (*dataImg + i)->label, FALSE);
        }
    }
    free(label);
}

int validateFileLineLength(char *fileLine)
{
    if (strlen(fileLine) >= MAXIMUM_LINE_LENGTH)
    {
        printf("ERROR | Exceeded maximum file length | ");
        return FALSE;
    }
    return TRUE;
}

int parseDirective(char *line, dataObject dataObj, node *macros, int *DC, int *IC, dataImage **dataImg, symbolTable **symbolT, binaryCode **dataCode)
{
    char *token = allocateMemoryForChar(strlen(line));
    char *copiedLine = allocateMemoryForChar(strlen(line));
    char *substr;
    int label = 0;

    strcpy(copiedLine, line);

    label = extractLabelIfExists(line, &token, dataObj, macros);

    if ((substr = strstr(copiedLine, DATA)))
    {
        /** Increment string past .data*/
        substr += strlen(DATA);

        if (!validateStringSeperatedByCommas(substr, TRUE)) /** Validates string seperated by commas and valid */
        {
            printf("ERROR |  Invalid numbers in .data | File line %d\n", *IC);
            return FALSE;
        }
        if (label)
        {
            setLabelInSymbolTable(symbolT, token, *IC, FALSE, FALSE);
        };
        setNumbersInDataImage(dataImg, dataCode, substr, DC, IC, label == TRUE ? token : NULL);
    }
    else if ((substr = strstr(copiedLine, STRING)))
    {
        substr += strlen(STRING);
        substr = trimSpaces(substr);
        if (!validateParentheses(substr))
        {
            printf("ERROR | String missing parentheses | File Line %d \n", *IC);
            return FALSE;
        };

        if (label)
        {
            setLabelInSymbolTable(symbolT, token, *IC, FALSE, FALSE);
        }
        setStringInDataImage(dataImg, dataCode, substr, DC, IC, label == TRUE ? token : NULL);
    }
    else if ((substr = strstr(copiedLine, ENTRY)))
    {
        substr += strlen(ENTRY);
        setLabelInSymbolTable(symbolT, substr, *IC, FALSE, TRUE);
    }
    else if ((substr = strstr(copiedLine, EXTERN)))
    {

        substr += strlen(EXTERN);
        setLabelInSymbolTable(symbolT, substr, *IC, TRUE, FALSE);
    }

    free(copiedLine);
    return TRUE;
}

void setNumbersInDataImage(dataImage **dataImg, binaryCode **dataCode, char *numbers, int *DC, int *IC, char *label)
{
    int amount;
    int i;
    int *nums = extractNumbersSeperatedByCommas(numbers, &amount);

    *dataImg = (dataImage *)realloc(*dataImg, sizeof(dataImage) * ((*DC) + amount));
    if (dataImg == NULL)
    {
        printf("ERROR: Failed to realloc memory location for data image\n");
        return;
    };

    for (i = 0; i < amount; i++)
    {

        if (label)
        {
            (*dataImg + *DC)->label = allocateMemoryForChar(strlen(label));
            strcpy((*dataImg + *DC)->label, label);
        }
        else
        {
            (*dataImg + *DC)->label = NULL;
        }

        (*dataImg + *DC)->value = nums[i];
        (*dataImg + *DC)->is_data = TRUE;
        (*DC)++;
    }
}

void setStringInDataImage(dataImage **dataImg, binaryCode **dataCode, char *str, int *DC, int *IC, char *label)
{
    str = removeParantheses(str);

    while (*str)
    {
        *dataImg = (dataImage *)realloc(*dataImg, sizeof(dataImage) * ((*DC) + 1));

        if (*dataImg == NULL)
        {
            return;
        }

        if (label)
        {
            (*dataImg + *DC)->label = allocateMemoryForChar(strlen(label));

            strcpy((*dataImg + *DC)->label, label);
        }

        /** Incase last character is zero, appends it */
        (*dataImg + *DC)->value = *str == '0' ? 0 : *str;

        (*dataImg + *DC)->is_data = FALSE;

        str++;

        (*DC)++;
    }
}

char *extractCommand(char *str)
{
    char *command = allocateMemoryForChar(MAXIMUM_COMMAND_LENGTH);
    int i = 0;

    while (*str)
    {
        if (isspace(*str))
        {
            break;
        }

        command[i] = *str;
        i++;
        str++;
    }
    command[i] = '\0';
    return command;
}

int parseCommand(char *line, dataObject dataObj, node *macros, symbolTable **symbolT, int *IC, binaryCode **dataCode)
{
    char *token = allocateMemoryForChar(strlen(line));
    char *copyLine = allocateMemoryForChar(strlen(line));
    int foundLabel = FALSE;
    char *command;
    char **operands;
    int i = 0, operandsLength, value, commandIndex, opcode, addressType[2];

    strcpy(copyLine, line);

    addressType[0] = -1;
    addressType[1] = -1;

    foundLabel = extractLabelIfExists(line, &token, dataObj, macros);
    copyLine = trimSpaces(copyLine);
    if (foundLabel)
    {
        saveLabel(IC, &copyLine, line, token, symbolT);
    }
    copyLine = trimSpaces(copyLine);

    command = extractCommand(copyLine);

    copyLine += strlen(command);

    commandIndex = findCommand(command, dataObj.commands);

    if (commandIndex == -1)
    {
        printf("ERROR | Invalid command | File line %d\n", *IC);
        free(copyLine);
        return TRUE;
    }

    if (!validateStringSeperatedByCommas(copyLine, FALSE))
    {
        free(copyLine);
        return FALSE;
    }

    operands = extractOperandsSeperatedByCommas(copyLine, &operandsLength);
    if (dataObj.commands[commandIndex].arguments_amount != operandsLength)
    {
        printf("ERROR | Too many operands (%d) provided for (%s) command | File line %d\n", operandsLength, command, *IC);
        free(copyLine);
        free(operands);
        return FALSE;
    }

    for (i = 0; i < operandsLength; i++)
    {
        addressType[i] = findAddressNumber(operands[i], dataObj, macros);
    }

    if (!validateCommandOperands(addressType[0], addressType[1], dataObj.commands[commandIndex], operandsLength))
    {
        printf("ERROR | Command %s doesn't accept these address type | File Line %d\n", dataObj.commands[commandIndex].name, *IC);
        free(copyLine);
        free(operands);
        return FALSE;
    };

    opcode = findOpcode(command, dataObj.commands);

    if (operandsLength == 2)
    {
        if (!codeCommand(opcode, addressType[0], addressType[1], dataCode, IC))
        {
            free(copyLine);
            free(operands);
            return FALSE;
        };
        if (!saveCommandTwoOperands(addressType, operands, symbolT, dataObj.registers, dataCode, IC))
        {
            free(copyLine);
            free(operands);
            return FALSE;
        };
    }
    else if (operandsLength == 1)
    {
        if (!codeCommand(opcode, addressType[1], addressType[0], dataCode, IC))
        {
            free(copyLine);
            free(operands);
            return FALSE;
        };
        if (!saveCommandOneOperand(addressType[0], operands, symbolT, dataObj.registers, dataCode, IC))
        {
            free(copyLine);
            free(operands);
            return FALSE;
        };
    }
    else
    {
        value = convertDecimalToBinary(opcode, OPERAND_SHIFT) |
                convertDecimalToBinary(4, ARE_SHIFT);

        if (!saveCode(dataCode, value, IC, NULL, FALSE))
        {

            free(copyLine);
            free(operands);
            return FALSE;
        };
    }

    free(operands);
    return TRUE;
}

int codeCommand(int opcode, int addressOne, int addressTwo, binaryCode **dataCode, int *IC)
{
    unsigned short value;
    int addressOneNum = convertAddressNumberToBinary(addressOne);
    int addressTwoNum = convertAddressNumberToBinary(addressTwo);

    value = convertDecimalToBinary(opcode, OPERAND_SHIFT) |
            convertDecimalToBinary(addressOneNum, DEST_REGISTER_SHIFT) |
            convertDecimalToBinary(addressTwoNum, SOURCE_REGISTER_SHIFT) |
            convertDecimalToBinary(4, ARE_SHIFT);

    if (!saveCode(dataCode, value, IC, NULL, FALSE))
    {
        return FALSE;
    };

    return TRUE;
}

int saveCommandTwoOperands(int addressType[], char **operands, symbolTable **symbolT, char **registers, binaryCode **dataCode, int *IC)
{
    unsigned short result[2];
    char *label = addressType[0] == 1 ? operands[0] : NULL;
    int shouldRecode = FALSE;

    result[0] = codeByAddress(addressType[0], operands[0], symbolT, registers, REGISTER_BIT_SIX);
    result[1] = codeByAddress(addressType[1], operands[1], symbolT, registers, REGISTER_BIT_THREE);

    if (areBothOperandsRegistersAddressType(addressType[0], addressType[1]))
    {
        if (!saveCode(dataCode, result[0] | result[1], IC, NULL, FALSE))
        {
            return FALSE;
        };
    }
    else
    {
        shouldRecode = result[0] == 0 ? TRUE : FALSE;
        if (!saveCode(dataCode, result[0], IC, label, shouldRecode))
        {
            return FALSE;
        };

        label = addressType[1] == 1 ? operands[1] : NULL;

        shouldRecode = result[1] == 0 ? TRUE : FALSE;
        if (!saveCode(dataCode, result[1], IC, label, shouldRecode))
        {
            return FALSE;
        };
    }

    return TRUE;
}

int saveCommandOneOperand(int addressType, char **operands, symbolTable **symbolT, char **registers, binaryCode **dataCode, int *IC)
{
    unsigned short result;
    char *label = addressType == 1 ? operands[0] : NULL;
    int shouldRecode = FALSE;

    result = codeByAddress(addressType, operands[0], symbolT, registers, REGISTER_BIT_THREE);

    if (!result)
    {
        shouldRecode = TRUE;
    }

    saveCode(dataCode, result, IC, label, shouldRecode);

    return TRUE;
}

int extractLabelIfExists(char *line, char **str, dataObject dataObj, node *macros)
{
    int label = FALSE;
    if (strstr(line, LABEL_SYMBOL_SPACE))
    {
        *str = strtok(line, " ");
        if (validateLabel(*str, dataObj, macros))
        {
            label = TRUE;
        };
    };

    return label;
}

int saveLabel(int *IC, char **lineWithoutLabel, char *line, char *token, symbolTable **symbolT)
{
    symbolTable *labelExist;

    labelExist = findLabelByName(symbolT, token);

    if (labelExist)
    {
        printf("ERROR | Label already exists | File line %d\n", *IC);
        token = strcat(token, ":");

        line = strstr(line, token);

        *lineWithoutLabel += strlen(line);

        if (labelExist->is_entry)
        {
            labelExist->line = *IC;
        }
        return FALSE;
    }

    setLabelInSymbolTable(symbolT, token, *IC, FALSE, FALSE);
    token = strcat(token, ":");

    line = strstr(line, token);

    *lineWithoutLabel += strlen(line);

    return TRUE;
}

char *removeParantheses(char *str)
{
    if (str[0] == '"')
    {
        str++;
    }

    if (str[strlen(str) - 1] == '"')
    {
        str[strlen(str) - 1] = '0';
    }
    return str;
}

int areBothOperandsRegistersAddressType(int addressOne, int addressTwo)
{
    if ((addressOne == 2 || addressOne == 3) && (addressTwo == 2 || addressTwo == 3))
    {
        return TRUE;
    }

    return FALSE;
}

int validateCommandOperands(int src, int target, cmd command, int operandsLength)
{
    int i;
    int src_allowed = FALSE;
    int target_allowed = FALSE;

    if (operandsLength == 1)
    {
        target = src;
        src = -1;
    };

    if (command.src_length == 0)
    {
        src_allowed = TRUE;
    }

    if (command.target_length == 0)
    {
        target_allowed = TRUE;
    }

    for (i = 0; i < command.src_length; i++)
    {
        if (command.src_allowed_operands[i] == src)
        {
            src_allowed = TRUE;
        }
    }

    for (i = 0; i < command.target_length; i++)
    {
        if (command.target_allowed_operands[i] == target)
        {
            target_allowed = TRUE;
        }
    }

    return src_allowed && target_allowed;
}