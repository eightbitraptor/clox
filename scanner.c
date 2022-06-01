#include <stdio.h>
#include <string.h>

#include "common.h"
#include "memory.h"
#include "scanner.h"

typedef struct {
    const char *start;
    const char *current;
    int line;
} scanner_t;

scanner_t *scanner;

#define SCANNER_CURRENT_CHAR (*(scanner->current))

static bool
scanner_at_end_p(void)
{
    return SCANNER_CURRENT_CHAR == '\0';
}

static bool
scanner_is_digit_p(char c)
{
    return '0' <= c && c <= '9';
}

static bool
scanner_is_alpha_p(char c)
{
    return ('a' <= c && c <= 'z') ||
           ('A' <= c && c <= 'Z') ||
           c == '_';
}

static token_t
scanner_make_token(token_type_t type)
{
    token_t token = {
        .type = type,
        .start = scanner->start,
        .length = (int)(scanner->current - scanner->start),
        .line = scanner->line,
    };
    return token;
}

static token_t
scanner_error_token(const char * msg)
{
    token_t token = {
        .type = TOKEN_ERROR,
        .start = msg,
        .length = (int)strlen(msg),
        .line = scanner->line,
    };
    return token;
}

static char
scanner_advance(void)
{
    scanner->current++;
    return scanner->current[-1];
}

static bool
scanner_match(char expected)
{
    if (scanner_at_end_p()) return false;
    if (SCANNER_CURRENT_CHAR != expected) return false;

    scanner->current++;
    return true;
}

static char
scanner_peek(void)
{
    return SCANNER_CURRENT_CHAR;
}

static char
scanner_peek_next()
{
    if (scanner_at_end_p()) {
        return '\0';
    } else {
        return scanner->current[1];
    }
}

static void
scanner_skip_whitespace(void)
{
    while (1) {
        char c = scanner_peek();
        switch(c) {
            case ' ':
            case '\r':
            case '\t':
                scanner_advance();
                break;
            case '\n':
                scanner->line++;
                scanner_advance();
                break;
            case '/':
                if (scanner_peek_next() == '/') {
                    while(scanner_peek() != '\n' && !scanner_at_end_p()) 
                        scanner_advance();
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

static token_type_t
scanner_check_keyword(int start, int len, const char *rest, token_type_t type)
{
    if (scanner->current - scanner->start == start + len && 
            memcmp(scanner->start + start, rest, len) == 0) {
        return type;
    }

    return TOKEN_IDENTIFIER;
}

static token_type_t
scanner_identifier_type(void)
{
    switch(scanner->start[0]) {
        case 'a': return scanner_check_keyword(1, 2, "nd", TOKEN_AND);
        case 'c': return scanner_check_keyword(1, 4, "lass", TOKEN_CLASS);
        case 'e': return scanner_check_keyword(1, 3, "lse", TOKEN_ELSE);
        case 'f': {
            if (scanner->current - scanner->start > 1) {
                switch(scanner->start[1]) {
                    case 'a': return scanner_check_keyword(2, 3, "lse", TOKEN_FALSE);
                    case 'o': return scanner_check_keyword(2, 1, "r", TOKEN_FOR);
                    case 'u': return scanner_check_keyword(2, 1, "n", TOKEN_FUN);
                }
            };
            break;
        }
        case 'i': return scanner_check_keyword(1, 1, "f", TOKEN_IF);
        case 'n': return scanner_check_keyword(1, 2, "il", TOKEN_NIL);
        case 'o': return scanner_check_keyword(1, 1, "o", TOKEN_OR);
        case 'p': return scanner_check_keyword(1, 4, "rint", TOKEN_PRINT);
        case 'r': return scanner_check_keyword(1, 5, "eturn", TOKEN_RETURN);
        case 's': return scanner_check_keyword(1, 4, "uper", TOKEN_SUPER);
        case 't': {
            if (scanner->current - scanner->start > 1) {
                switch(scanner->start[1]) {
                    case 'h': return scanner_check_keyword(2, 2, "is", TOKEN_THIS);
                    case 'r': return scanner_check_keyword(2, 2, "ue", TOKEN_TRUE);
                }
            };
            break;
        }
        case 'v': return scanner_check_keyword(1, 2, "ar", TOKEN_VAR);
        case 'w': return scanner_check_keyword(1, 4, "hile", TOKEN_WHILE);
    }

    return TOKEN_IDENTIFIER;
}

static token_t
scanner_scan_string(void)
{
    while(scanner_peek() != '"' && !scanner_at_end_p()) {
        if (scanner_peek() == '\n') scanner->line++;
        scanner_advance();
    }

    if (scanner_at_end_p()) return scanner_error_token("Unterminated String");

    scanner_advance();
    return scanner_make_token(TOKEN_STRING);
}

static token_t
scanner_scan_number(void)
{
    while(scanner_is_digit_p(scanner_peek())) scanner_advance();

    if (scanner_peek() == '.' && scanner_is_digit_p(scanner_peek_next())) {
        scanner_advance();

        while(scanner_is_digit_p(scanner_peek())) scanner_advance();
    }
    return scanner_make_token(TOKEN_NUMBER);
}

static token_t
scanner_scan_identifier(void)
{
    while(scanner_is_alpha_p(scanner_peek()) || 
            scanner_is_digit_p(scanner_peek())) {
        scanner_advance();
    }

    return scanner_make_token(scanner_identifier_type());
}

void
scanner_init(const char *source)
{
    scanner = clox_malloc(sizeof(scanner_t));
    scanner->start = source;
    scanner->current = source;
    scanner->line = 1;
}

token_t
scanner_scan_token(void)
{
    scanner_skip_whitespace();
    scanner->start = scanner->current;

    if (scanner_at_end_p()) {
        return scanner_make_token(TOKEN_EOF);
    }

    char c = scanner_advance();
    if (scanner_is_alpha_p(c)) return scanner_scan_identifier();
    if (scanner_is_digit_p(c)) return scanner_scan_number();

    switch(c) {
        case '(': return scanner_make_token(TOKEN_LEFT_PAREN);
        case ')': return scanner_make_token(TOKEN_RIGHT_PAREN);
        case '{': return scanner_make_token(TOKEN_LEFT_BRACE);
        case '}': return scanner_make_token(TOKEN_RIGHT_BRACE);
        case ';': return scanner_make_token(TOKEN_SEMICOLON);
        case ',': return scanner_make_token(TOKEN_COMMA);
        case '.': return scanner_make_token(TOKEN_PERIOD);
        case '-': return scanner_make_token(TOKEN_MINUS);
        case '+': return scanner_make_token(TOKEN_PLUS);
        case '/': return scanner_make_token(TOKEN_SLASH);
        case '*': return scanner_make_token(TOKEN_STAR);
        case '!':
            return scanner_make_token(
                    scanner_match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=':
            return scanner_make_token(
                    scanner_match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '>':
            return scanner_make_token(
                    scanner_match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '<':
            return scanner_make_token(
                    scanner_match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '"': return scanner_scan_string();
    }

    return scanner_error_token("Unexpected character");
}
