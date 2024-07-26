#ifndef TYPE_H
#define TYPE_H
struct command
{
    char *name;
    int opcode;
    int arguments_amount;
    int src_allowed_operands[4];
    int src_length;
    int target_allowed_operands[4];
    int target_length;
};

typedef struct command cmd;

typedef struct data
{
    cmd *commands;
    char **instructions;
    char **registers;
} dataObject;

struct image
{
    char *label;
    char *line;
    int value;
    int is_data;
};

typedef struct image dataImage;

struct symbol_table
{
    int line;
    char *name;
    int is_entry;
    int is_extern;
    struct symbol_table *next;
};

typedef struct symbol_table symbolTable;

struct code
{
    /*int line;*/
    int recode;
    unsigned short num;
    char *label;
};

typedef struct code binaryCode;

#endif
