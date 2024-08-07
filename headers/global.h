#ifndef GLOBAL_H
#define GLOBAL_H
#include "./linked-list.h"

#define FALSE 0
#define TRUE 1

#define MACR "macr"
#define ENDMACR "endmacr"

#define REGISTER_AMOUNT 8

#define INSTRUCTIONS_AMOUNT 4

#define COMMANDS_AMOUNT 16

#define MAXIMUM_LINE_LENGTH 81
#define READ_LINE_LENGTH 100

#define MAXIMUM_COMMAND_LENGTH 5
#define MAXIMUM_OPERANDS_LENGTH 3

#define COMMENT_SYMBOL ';'
#define LABEL_SYMBOL_SPACE ": "
#define LABEL_SYMBOL ':'

#define LABEL_LENGTH 31

#define IC_INITIAL_VALUE 100

#define OPERAND_SHIFT 11

#define DEST_REGISTER_SHIFT 7

#define SOURCE_REGISTER_SHIFT 3

#define REGISTER_BIT_SIX 6
#define REGISTER_BIT_THREE 3

#define ARE_SHIFT 0

#define EXTERN_EXTENSION ".ext"
#define ENTRY_EXTENSION ".ent"
#define OBJ_EXTENSION ".ob"
#define AM_EXTENSION ".am"
#define AS_EXTENSION ".as"
#endif