#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct StringVariable {
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
        } else if (strcmp(tokens[i], "-") == 0) {
            base_number -= strtof(tokens[i+1], NULL);
        }
    }
    return base_number;
}

char* evaluate_string(const char** tokens, int size, int offset) {
    char* base_string = "";
    strcat(base_string, tokens[offset]);
    return base_string;
}

void interpret(const char** tokens, int size) {
    if (isalpha(tokens[0][0])) {
        if (strcmp(tokens[1], "=") == 0) {
            printf("assignment\n");
            if (isdigit(tokens[2][0])) {
                floatVars[float_vars_count].name = strdup(tokens[0]);
                printf("calculation\n");
                floatVars[float_vars_count].value = evaluate_calculation(tokens, size, 2);
                float_vars_count++;
            } else if (tokens[2][0] == '\"') {
                strVars[str_vars_count].name = strdup(tokens[0]);
                printf("string\n");
                strVars[str_vars_count].value = strdup(evaluate_string(tokens, size, 2));
                str_vars_count++;
            }
        }
    }
}

void print_float_vars() {
    printf("\nfloats:\n");
    for (int i = 0; i < float_vars_count; i++) {
        printf("%s = %.2f\n", floatVars[i].name, floatVars[i].value);
    }
}

void print_string_vars() {
    printf("\nstrings:\n");
    for (int i = 0; i < str_vars_count; i++) {
        printf("%s = %s\n", strVars[i].name, strVars[i].value);
    }
}

int main() {
    init_vars(100);
    const char* num_tokens[] = { "num", "=", "0", "+", "1", "-", "0.5" };
    int size = sizeof(num_tokens) / sizeof(num_tokens[0]);
    interpret(num_tokens, size);

    const char* str_tokens[] = { "str", "=", "\"somestr\"" };
    int size2 = sizeof(str_tokens) / sizeof(str_tokens[0]);
    interpret(str_tokens, size2);

    print_float_vars();
    print_string_vars();

    return 0;
}