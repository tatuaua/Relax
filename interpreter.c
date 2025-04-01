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
    char base_string[1024] = "";
    strcat(base_string, tokens[offset]);
    return strdup(base_string);
}

void interpret(const char** tokens, int size) {
    if(size == 1) {
        printf("C: function call\n");
        char function_name[50];
        char function_argument[50];
        int is_arg = 0;
        int function_name_index = 0;
        int function_arg_index = 0;
        for(int i = 0; i < strlen(tokens[0]); i++) {
            if(tokens[0][i] == ')') {
                break;
            }
            if(tokens[0][i] == '(') {
                is_arg = 1;
                i++; // skip the first parenhesis
            }
            if(is_arg == 0) {
                function_name[function_name_index] = tokens[0][i];
                function_name_index++;
            } else {
                function_argument[function_arg_index] = tokens[0][i];
                function_arg_index++;
            }
        }
        function_name[function_name_index+1] = '\0';
        function_argument[function_arg_index+1] = '\0';

        if(strcmp(function_name, "print") == 0) {
            printf("R: %s", function_argument);
        }
    }
    
    if (isalpha(tokens[0][0])) {
        if (strcmp(tokens[1], "=") == 0) {
            printf("C: assignment\n");
            if (isdigit(tokens[2][0])) {
                floatVars[float_vars_count].name = strdup(tokens[0]);
                printf("C: calculation\n");
                floatVars[float_vars_count].value = evaluate_calculation(tokens, size, 2);
                float_vars_count++;
            } else if (tokens[2][0] == '\"') {
                strVars[str_vars_count].name = strdup(tokens[0]);
                printf("C: string\n");
                strVars[str_vars_count].value = evaluate_string(tokens, size, 2);
                str_vars_count++;
            }
        }
    }
}

void print_float_vars() {
    printf("\nC: floats:\n");
    for (int i = 0; i < float_vars_count; i++) {
        printf("C: %s = %.2f\n", floatVars[i].name, floatVars[i].value);
    }
}

void print_string_vars() {
    printf("\nC: strings:\n");
    for (int i = 0; i < str_vars_count; i++) {
        printf("C: %s = %s\n", strVars[i].name, strVars[i].value);
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

    const char* print_tokens[] = { "print(balls)" };
    interpret(print_tokens, 1);

    print_float_vars();
    print_string_vars();

    return 0;
}