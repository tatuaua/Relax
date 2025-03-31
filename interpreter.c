#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct StringVariable{
    char* name;
    char* value;
};

struct FloatVariable {
    char* name;
    float value;
};

struct StringVariable *strVars = NULL;
struct FloatVariable *floatVars = NULL;
int str_vars_count = 0;
int float_vars_count = 0;

void init_vars(int size) {
    strVars = (struct StringVariable*) malloc(size * sizeof(struct StringVariable));
    floatVars = (struct FloatVariable*) malloc(size * sizeof(struct FloatVariable));
}

float evaluate_calculation(const char** tokens, int size, int offset) {
    float base_number = strtof(tokens[offset], NULL);
    for (int i = offset + 1; i < size; i = i + 2) {
        if (strcmp(tokens[i], "+") == 0) {
            base_number += strtof(tokens[i+1], NULL);
            printf("base number after plus: %.2f\n", base_number);
        } else if (strcmp(tokens[i], "-") == 0) {
            base_number -= strtof(tokens[i+1], NULL);
            printf("base number after minus: %.2f\n", base_number);
        }
    }

    printf("number: %.2f\n", base_number);
    return base_number;
}

void interpret(const char** tokens, int size) {
    if (isalpha(tokens[0][0])) {
        if (strcmp(tokens[1], "=") == 0) {
            printf("assignment\n");
            if(isdigit(tokens[2][0])) {
                floatVars[float_vars_count].name = strdup(tokens[0]);
                printf("calculation\n");
                floatVars[float_vars_count].value = evaluate_calculation(tokens, size, 2);
                float_vars_count++;
            }
        }
    }
}

int main() {
    init_vars(100);
    const char* tokens[] = { "num", "=", "0", "+", "1", "-", "0.5" };
    int size = sizeof(tokens) / sizeof(tokens[0]);
    interpret(tokens, size);
    return 0;
}