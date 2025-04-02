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

struct Function {
    char* name;
    char* content;
};

struct StringVariable *strVars = NULL;
struct FloatVariable *floatVars = NULL;
int str_vars_count = 0;
int float_vars_count = 0;

struct Function *functions = NULL;
int function_count = 0;

void init_vars(int size) {
    strVars = (struct StringVariable*) malloc(size * sizeof(struct StringVariable));
    floatVars = (struct FloatVariable*) malloc(size * sizeof(struct FloatVariable));
}

void init_functions(int size) {
    functions = (struct Function*) malloc(size * sizeof(struct Function));
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

void parse_function_name(const char* token, char* function_name, char* function_argument) {
    int is_arg = 0;
    int function_name_index = 0;
    int function_arg_index = 0;
    
    for(int i = 0; i < strlen(token); i++) {
        if(token[i] == ')') {
            break;
        }
        if(token[i] == '(') {
            is_arg = 1;
            i++; // skip the first parenthesis
        }
        if(is_arg == 0) {
            function_name[function_name_index++] = token[i];
        } else {
            function_argument[function_arg_index++] = token[i];
        }
    }
    function_name[function_name_index] = '\0';
    function_argument[function_arg_index] = '\0';
}

void relax_print_str(char* value) {
    
}

void relax_print_float(float value) {

}

void relax_print(char* arg) {
    if(arg[0] == '\"') {
        char value[50] = "";
        int value_index = 0;
        for(int i = 1; i < strlen(arg) - 1; i++) {
            value[value_index++] = arg[i];
        }
        value[value_index] = '\0';
        relax_print_str(value);
    } else if(isdigit(arg[0])) {
        relax_print_float(strtof(arg, NULL));
    } else {
        if(is_string_variable(arg)) {
            struct StringVariable strVar = get_string(arg);
        } else {
            struct FloatVariable floatVar = get_float(arg);
        }
    }
}

void interpret(const char** tokens, int size) {
    if(size == 1) {
        char function_name[50] = "";
        char function_argument[50] = "";
        parse_function_name(tokens[0], function_name, function_argument);

        if(strcmp(function_name, "print") == 0) {
            printf("R: %s\n", function_argument);
        } else {
            // Check if it's a custom function call
            for(int i = 0; i < function_count; i++) {
                if(strcmp(functions[i].name, function_name) == 0) {
                    const char* func_tokens[1] = {functions[i].content};
                    interpret(func_tokens, 1);
                    break;
                }
            }
        }
        return;
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
        if(strcmp(tokens[1], "{") == 0) {
            printf("C: function declaration\n");
            if(size == 4 && strcmp(tokens[3], "}") == 0) {
                char function_name[50] = "";
                char function_argument[50] = "";
                parse_function_name(tokens[0], function_name, function_argument);
                
                functions[function_count].name = strdup(function_name);
                functions[function_count].content = strdup(tokens[2]);
                function_count++;
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
    init_functions(100);
    
    const char* num_tokens[] = { "num", "=", "0", "+", "1", "-", "0.5" };
    interpret(num_tokens, 7);

    const char* str_tokens[] = { "str", "=", "\"somestr\"" };
    interpret(str_tokens, 3);

    const char* function_tokens[] = { "myfunc()", "{", "print(balls)", "}"};
    interpret(function_tokens, 4);

    const char* myfunc_tokens[] = { "myfunc()" };
    interpret(myfunc_tokens, 1);

    print_float_vars();
    print_string_vars();

    return 0;
}