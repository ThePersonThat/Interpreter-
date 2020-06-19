#pragma once
#include <string>
#include <vector>

#define WITHOUT_POSITION -1

typedef enum {
	LEFT_BRACKET, RIGHT_BRACKET,
	SQ_LEFT_BRACKET, SQ_RIGHT_BRACKET,
	OP_PLUS, OP_MINUS, OP_MUL, OP_DIV,
	OP_MORE, OP_LESS, OP_EQL,
	OP_MORE_EQ, OP_LESS_EQ,
	OP_EQ, OP_NOT_EQ,
	F_LEFT_BRACKET, F_RIGHT_BRACKET,
	STRING,
	FUNC, COMMA, 
	NUMBER, 
	VAR, EQL,
	VAR_ARRAY,
	EOO, // end of operation
	EOE // end of expresion 
}TokenType_t;


class AlgorithmException : public std::exception
{
public:
	AlgorithmException(const char* msg, int position);
	AlgorithmException(const std::string& msg, int position);
	

	int getPostion();

private:
	int pos_error;
};


class Token {

private:
	std::string value;
	TokenType_t type;
	int position;

public: 
	
	Token() = default;

	void set(TokenType_t token, const std::string& value, int postion);
	void set(TokenType_t token, char value, int postion);

	std::string toString();
	int getCharPosition();
	std::string getValue();
	TokenType_t getType();
};

class BufferTokens
{
private:
	std::vector<Token> vector;
	int pos;

public: 
	BufferTokens(const std::vector<Token>& tokens);
	void isAvaliable();
	Token next();
	int getPosition();
	int getCharPosition();
	void back();
	void setPostion(int position);
	void setToken(const Token& token, int position);
};

extern std::string output_buffer;

static std::string typeOf(TokenType_t number); // for debug

std::vector<Token> analysisExp(const std::string& exp);
void parseExp(const std::vector<Token>& tokens);
static double expr(BufferTokens& buffer);
static double plusminus(BufferTokens& buffer);
static double muldiv(BufferTokens& buffer);
static double factor(BufferTokens& buffer);
static double func(BufferTokens& buffer);
static double forloop(BufferTokens& buffer);
static void output(BufferTokens& buffer);
static double forloopWithCondition(BufferTokens& buffer);
static double if_statment(BufferTokens& buffer);
void clear();
void setMap();
