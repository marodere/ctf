#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
 * tokenizer
 */

struct {
    enum {
        TK_NONE,
        TK_ADD,
        TK_MUL,
        TK_OPEN_BRACKET,
        TK_CLOSE_BRACKET,
        TK_END,
        TK_NUMBER
    } token;
    float value;
} cur_tok;

void next_tok(const char* nptr, char** endptr) {
    // most of the tokens have strlen == 1
    *endptr = (char*)nptr + 1;
    switch(*nptr) {
        case '+':
            cur_tok.token = TK_ADD;
        break;
        case '*':
            cur_tok.token = TK_MUL;
        break;
        case '(':
            cur_tok.token = TK_OPEN_BRACKET;
        break;
        case ')':
            cur_tok.token = TK_CLOSE_BRACKET;
        break;
        case '\0':
            cur_tok.token = TK_END;
        break;
        default:
            cur_tok.token = TK_NUMBER;
            cur_tok.value = strtod(nptr, endptr);
            // ensure that at least one character has been parsed
            // to avoid infinite loop on malformed input
            assert(nptr != *endptr);
        break;
    }
}

/*
 * end of tokenizer
 */

/*
 * BNF.
 * формально,
 * 
 * выражение ::= слагаемое [ + выражение ]
 * слагаемое ::= множитель [ * слагаемое ]
 * множитель ::= число ИЛИ ( выражение ) // в смысле выражение в скобках
 * 
 * в самом деле,
 */ 

float expression(char** input);

float multiplier(char** input) {
    float result;
    next_tok(*input, input);
    switch(cur_tok.token) {
        case TK_NUMBER:
            result = cur_tok.value;
        break;
        case TK_OPEN_BRACKET:
            result = expression(input);
            assert(cur_tok.token == TK_CLOSE_BRACKET);
        break;
        default:
            assert(false);
        break;
    }
    next_tok(*input, input);
    return result;
}

float summand(char** input) {
    float result = multiplier(input);
    switch(cur_tok.token) {
        case TK_MUL:
            return result * summand(input);
        break;
        case TK_ADD:
        case TK_CLOSE_BRACKET:
        case TK_END:
            return result;
        break;
        default:
            assert(false);
        break;
    }
}

float expression(char** input) {
    float result = summand(input);
    switch(cur_tok.token) {
        case TK_ADD:
            return result + expression(input);
        break;
        case TK_CLOSE_BRACKET:
        case TK_END:
            return result;
        break;
        default:
            assert(false);
        break;
    }
}

/*
 * и всё!
 */

static const char* tests[] = {
    "2+2",
    "2*2",
    
    "1.42+5.892",
    "2.9248*2.5",

    "(1+3*5+6)",
    "1+(3*5)+6",

    "(1+3)*2*(5+6)",
    "((((2*2)*2)+2)+2)",
    "(2*(2*(2+(2+2))))",
    "(1+3)*((5+0))*((((1+(0*(0))))))",
    
    //
    // сделай сам, чтоб работало
    // начни с правильной формальной BNF!
    //
    //"5-2",
    //"5/2",
    //"-4-(+2)", // he-he
    //"3^2+1",
    //
    
    ""
};

int main(int argc, char* argv[]) {
    int tn;
    for(tn=0; *tests[tn] != '\0'; ++tn) {
        char* s = (char*)tests[tn];
        printf("%s = %.3f\n", tests[tn], expression(&s));
    }
    return 0;
}
