#pragma once

#include <sstream>
#include <vector>

using namespace std;

enum class LogicalOperation {
    Or,
    And,
};

enum class Comparison {
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    NotEqual,
};

enum class TokenType {
    DATE,
    EVENT,
    COLUMN,
    LOGICAL_OP,
    COMPARE_OP,
    PAREN_LEFT,
    PAREN_RIGHT,
};

struct Token {
    const string Value;
    const TokenType Type;
};

vector<Token> Tokenize(istream& cl);