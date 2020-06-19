#include "token.h"
#include <map>
#include <iterator>
#include <functional>

typedef const std::vector<double>& ARGUMENTS;
typedef std::function<double(ARGUMENTS)> function;
typedef std::vector<double> ELEMENTS_ARRAY;


static std::map <std::string, function> mapFunctions;
static std::map <std::string, double> mapVariables; 
static std::map <std::string, ELEMENTS_ARRAY> mapArrays;
std::string output_buffer;


void setMap()
{
	mapFunctions["sqrt"] = [](ARGUMENTS args) -> double
	{
		if (args.size() != 1)
			throw(AlgorithmException(std::string("Помилка! Неправильна кількість аргументів в функції 'sqrt'. Використання: sqrt(вираз)"), WITHOUT_POSITION));

		return sqrt(args[0]);
	};

	mapFunctions["abs"] = [](ARGUMENTS args) -> double
	{
		if (args.size() != 1)
			throw(AlgorithmException(std::string("Помилка! Неправильна кількість аргументів в функції 'abs'. Використання: abs(вираз)"), WITHOUT_POSITION));

		return abs(args[0]);
	};


	mapFunctions["min"] = [](ARGUMENTS args) -> double 
	{
		double min = INT_MAX;

		for (int i = 0; i < args.size(); i++) 
		{
			if (min > args[i])
				min = args[i];
		}

		return min;
	};

	mapFunctions["max"] = [](ARGUMENTS args) -> double 
	{
		double max = INT_MIN;

		for (int i = 0; i < args.size(); i++)
		{
			if (max < args[i])
				max = args[i];
		}

		return max;
	};

	mapFunctions["pow"] = [](ARGUMENTS args) -> double 
	{
		if (args.size() != 2)
			throw(AlgorithmException(std::string("Помилка! Неправильна кількість аргументів в функції 'pow'. Використання: pow(вираз, степінь)"), WITHOUT_POSITION));

		return pow(args[0], args[1]);
	};

	mapFunctions["algbsum"] = [](ARGUMENTS args) -> double
	{
		double sum = 0;

		for (int i = args[1]; i < args[2]; i++)
		{
			sum += args[0];
		}

		return sum;
	};

	mapFunctions["avr"] = [](ARGUMENTS args) -> double
	{
		double sum = 0;

		for (int i = 0; i < args.size(); i++)
			sum += args[i];

		return sum / args.size();
	};
}

AlgorithmException::AlgorithmException(const char* msg, int position) : exception(msg)
{
	this->pos_error = position;
}

AlgorithmException::AlgorithmException(const std::string& msg, int position) : exception(msg.c_str())
{
	this->pos_error = position;
}

int AlgorithmException::getPostion()
{
	return this->pos_error;
}


int Token::getCharPosition()
{
	return this->position;
}

void Token::set(TokenType_t type, const std::string& value, int position)
{
	this->value = value.c_str();
	this->type = type;
	this->position = position;
}

void Token::set(TokenType_t type, char value, int position)
{
	this->value = value;
	this->type = type;
	this->position = position;
}

std::string Token::getValue()
{
	return this->value;
}

TokenType_t Token::getType()
{
	return this->type;
}

std::string Token::toString() 
{
	return "type: " + typeOf(this->type) + "; value: " + this->value + "\n";
}

BufferTokens::BufferTokens(const std::vector<Token>& vector)
{
	this->vector = vector;
	this->pos = 0;
}

Token BufferTokens::next() 
{
	return vector[pos++];
}

int BufferTokens::getPosition()
{
	return pos;
}

void BufferTokens::back()
{
	pos--;
}

void BufferTokens::setPostion(int position)
{
	this->pos = position;
}

void BufferTokens::setToken(const Token& token, int position)
{
	vector[position] = token;
}

void BufferTokens::isAvaliable()
{
	int value = this->pos + 1;

	if (value > vector.size())
		throw(AlgorithmException("Помилка аналізу!", WITHOUT_POSITION));
}


int BufferTokens::getCharPosition()
{
	return this->vector[pos - 1].getCharPosition();
}

static std::string typeOf(TokenType_t number)
{
	switch (number)
	{
	case NUMBER:
		return "NUMBER";
	case EOE:
		return "END";
	case RIGHT_BRACKET:
		return "RIGHT_BRACKET";
	case LEFT_BRACKET:
		return "LEFT_BRACKET";
	case OP_PLUS:
		return "OP_PLUS";
	case OP_MINUS:
		return "OP_MINUS";
	case OP_MUL:
		return "OP_MUL";
	case OP_DIV:
		return "OP_DIV";
	case COMMA:
		return "COMMA";
	case FUNC:
		return "FUNC";
	case EQL:
		return "EQL";
	case VAR:
		return "VAR";
	case EOO:
		return "EOO";
	case VAR_ARRAY:
		return "VAR_ARRAY";
	case SQ_LEFT_BRACKET:
		return "SQ_LEFT_BRACKET";
	case SQ_RIGHT_BRACKET:
		return "SQ_RIGHT_BRACKET";
	case STRING:
		return "STRING";
	case OP_MORE:
		return "OP_MORE";
	case OP_LESS:
		return "OP_LESS";
	case OP_EQL:
		return "OP_EQL";
	case F_LEFT_BRACKET:
		return "F_LEFT_BRACKET";
	case F_RIGHT_BRACKET:
		return "F_RIGHT_BRACKET";
	case OP_LESS_EQ:
		return "OP_LESS_EQ";
	case OP_MORE_EQ:
		return "OP_MORE_EQ";
	case OP_EQ:
		return "OP_EQ";
	}
}


std::vector<Token> analysisExp(const std::string& exp)
{
	std::vector<Token> tokens;
	size_t size = exp.length();
	int pos = 0;
	Token token;

	if(exp[size - 1] == ';')
		throw(AlgorithmException("Помилка! Символ ';' в кінці алгоритму ставити не треба!", WITHOUT_POSITION));

	if (exp[size - 1] == ',')
		throw(AlgorithmException(std::string("Несподіваний символ ',' на позиції: ") + std::to_string(size) , size));

	while (pos < size)
	{
		switch (exp[pos])
		{
		case '+':
			token.set(OP_PLUS, '+', pos);
			tokens.push_back(token);
			pos++;
			continue;
		case '-':
			token.set(OP_MINUS, '-', pos);
			tokens.push_back(token);
			pos++;
			continue;
		case '*':
			token.set(OP_MUL, '*', pos);
			tokens.push_back(token);
			pos++;
			continue;
		case '/':
			token.set(OP_DIV, '/', pos);
			tokens.push_back(token);
			pos++;
			continue;
		case '(':
			token.set(LEFT_BRACKET, '(', pos);
			tokens.push_back(token);
			pos++;
			continue;
		case ')':
			token.set(RIGHT_BRACKET, ')', pos);
			tokens.push_back(token);
			pos++;
			continue;
		case ',':
			token.set(COMMA, ',', pos);
			tokens.push_back(token);
			pos++;
			continue;
		case '=':
			if (exp[++pos] == '=')
			{
				token.set(OP_EQ, "==", pos);
				tokens.push_back(token);
				pos++;
				continue;
			}
			else pos--;

			token.set(EQL, '=', pos);
			tokens.push_back(token);
			pos++;
			continue;
		case ';':
			token.set(EOO, ';', pos);
			tokens.push_back(token);
			pos++;
			continue;
		case '[':
			token.set(SQ_LEFT_BRACKET, '[', pos);
			tokens.push_back(token);
			pos++;
			continue;
		case '<':
			if (exp[++pos] == '=')
			{
				token.set(OP_LESS_EQ, "<=", pos);
				tokens.push_back(token);
				pos++;
				continue;
			}
			else pos--;

			token.set(OP_LESS, '<', pos);
			tokens.push_back(token);
			pos++;
			continue;
		case '!':
			if (exp[++pos] == '=')
			{
				token.set(OP_NOT_EQ, "!=", pos);
				tokens.push_back(token);
				pos++;
				continue;
			}
			else
			{
				throw(AlgorithmException(std::string("Помилка! Несподіваний символ '") + exp[pos] + std::string("' на позиції: ") + std::to_string(pos), pos));
			}
		case '>':

			if (exp[++pos] == '=')
			{
				token.set(OP_MORE_EQ, ">=", pos);
				tokens.push_back(token);
				pos++;
				continue;
			}
			else pos--;

			token.set(OP_MORE, '>', pos);
			tokens.push_back(token);
			pos++;
			continue;
		case ']':
			token.set(SQ_RIGHT_BRACKET, ']', pos);
			tokens.push_back(token);
			pos++;
			continue;
		case '{':
			token.set(F_LEFT_BRACKET, '{', pos);
			tokens.push_back(token);
			pos++;
			continue;
		case '}':
			token.set(F_RIGHT_BRACKET, '}', pos);
			tokens.push_back(token);
			pos++;
			continue;
		case '\'':
		{
			std::string buf;

			do
			{
				pos++;
				buf.push_back(exp[pos]);
			} while (exp[pos] != '\'' && pos < size);

			pos++;

			if (pos > size)
				throw(AlgorithmException("Помилка аналізу", WITHOUT_POSITION));

			buf.pop_back();

			token.set(STRING, buf, pos);
			tokens.push_back(token);
			continue;
		}
		case '#':
			do
			{
				pos++;
			} while (exp[pos] != '#' && pos < size);
			pos++;

			if (pos > size)
				throw(AlgorithmException("Помилка аналізу", WITHOUT_POSITION));

			continue;
		case '_':
		{
			std::string buf;

			do
			{
				buf.push_back(exp[pos]);

				if (pos >= size || exp[pos] == EOE)
					break;

				pos++;

			} while (exp[pos] >= 'a' && exp[pos] <= 'z' || exp[pos] >= 'A' && exp[pos] <= 'Z');


			token.set(VAR, buf, pos);
			tokens.push_back(token);
			continue;
		}
		default: 
			if (exp[pos] >= '0' && exp[pos] <= '9')
			{
				std::string buf;
				int isDouble = 0;

				do
				{
					buf.push_back(exp[pos]);

					if (pos >= size || exp[pos] == EOE)
						break;

					if (exp[pos] == '.')
						isDouble++;

					pos++;

				} while (exp[pos] >= '0' && exp[pos] <= '9' || exp[pos] == '.');

				if(isDouble > 1)
					throw(AlgorithmException(std::string("Помилка! Неможливо прочитати число ") + std::string("на позиції: ") + std::to_string(pos), pos));

				token.set(NUMBER, buf, pos);
				tokens.push_back(token);
			}
			else if(exp[pos] >= 'a' && exp[pos] <= 'z' || exp[pos] >= 'A' && exp[pos] <= 'Z') 
			{
				std::string buf;

				do
				{
					buf.push_back(exp[pos]);

					if (pos >= size || exp[pos] == EOE)
						break;

					pos++;
				} while (exp[pos] >= 'a' && exp[pos] <= 'z' || exp[pos] >= 'A' && exp[pos] <= 'Z');

			

				if (mapFunctions[buf] != 0 || buf == "for" || buf == "printf" || buf == "forc" || "if")
				{
					token.set(FUNC, buf, pos);
					tokens.push_back(token);
				}
				else
				{
					throw(AlgorithmException(std::string("Помилка! Функції '" + buf + std::string("()' не знайдено. Позиція: ") + std::to_string(pos)), pos));
				}
			}
			else if (exp[pos] == ' ' || exp[pos] == '\n')
				pos++;
			else
				throw(AlgorithmException(std::string("Помилка! Несподіваний символ '") + exp[pos] + std::string("' на позиції: ") + std::to_string(pos), pos));
		}
	}
	token.set(EOE, ' ', pos);
	tokens.push_back(token);

	return tokens;
}

void parseExp(const std::vector<Token>& tokens)
{
	BufferTokens buffer(tokens);
	Token token = tokens[0];

	while (token.getType() != EOE)
	{
		expr(buffer);
		buffer.isAvaliable();
		token = buffer.next();

		if (token.getType() == F_RIGHT_BRACKET)
		{
			if (buffer.next().getType() == EOE)
				break;
			else 
				buffer.back();
		}	
	}
}

static double expr(BufferTokens& buffer)
{
	Token token = buffer.next();

	if (token.getType() == EOE)
		return 0;
	else 
	{
		buffer.back();
		return plusminus(buffer);
	}
}


static double plusminus(BufferTokens& buffer)
{
	double value = muldiv(buffer);

	while (true)
	{
		Token token = buffer.next();

		switch (token.getType())
		{
		case OP_PLUS:
			value += muldiv(buffer);
			break;

		case OP_MINUS:
			value -= muldiv(buffer);
			break;
		case OP_LESS:
			return value < expr(buffer);
		case OP_MORE:
			return value > expr(buffer);
		case OP_MORE_EQ:
			return value >= expr(buffer);
		case OP_LESS_EQ:
			return value <= expr(buffer);
		case OP_EQ:
			return value == expr(buffer);
		case OP_NOT_EQ:
			return value != expr(buffer);
		case EOE:
		case RIGHT_BRACKET:
		case F_RIGHT_BRACKET:
		case COMMA:
		case EOO:
		case SQ_RIGHT_BRACKET:
			buffer.back();
			return value;
		default:
			throw(AlgorithmException(std::string("Помилка! Несподіваний символ '") + token.getValue() + std::string("' на позиції: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));
		}
	}
}

static double muldiv(BufferTokens& buffer)
{
	double value = factor(buffer);

	while (true)
	{
		Token token = buffer.next();

		switch (token.getType())
		{
		case OP_MUL:
			value *= factor(buffer);
			break;
		case OP_DIV:
			value /= factor(buffer);
			break;
		case EOE:
		case RIGHT_BRACKET:
		case F_RIGHT_BRACKET:
		case OP_PLUS:
		case OP_MINUS:
		case OP_LESS:
		case OP_MORE:
		case OP_MORE_EQ:
		case OP_LESS_EQ:
		case OP_NOT_EQ:
		case OP_EQ:
		case COMMA:
		case EOO:
		case SQ_RIGHT_BRACKET:
			buffer.back();
			return value;
		default:
			throw(AlgorithmException(std::string("Помилка! Несподіваний символ '") + token.getValue() + std::string("' на позиції: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));
		}
	}
}

static double factor(BufferTokens& buffer)
{
	Token token = buffer.next();
	double value = 0;

	switch (token.getType())
	{
	case OP_MINUS:
		value = factor(buffer);
		return -value;
	case NUMBER:
		return std::stod(token.getValue());
	case STRING:
		output_buffer.append(token.getValue());
		token = buffer.next();

		if(token.getType() != COMMA && token.getType() != RIGHT_BRACKET)
			throw(AlgorithmException(std::string("Помилка! Несподіваний символ '") + token.getValue() + std::string("' на позиції: ") + std::to_string(buffer.getPosition()), buffer.getPosition()));

		buffer.back();

		return NAN;
	case VAR:
	{
		std::string name = token.getValue();
		token = buffer.next();
		
		if (token.getType() == SQ_LEFT_BRACKET) 
		{
			token = buffer.next();

			if (token.getType() == SQ_RIGHT_BRACKET)
			{
				if (mapVariables.find(name) != mapVariables.end())
					throw(AlgorithmException(std::string("Змінна '") + name + std::string("' уже об'явлена! Позиція: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));
				
				token = buffer.next();

				if(token.getType() != EQL)
					throw(AlgorithmException(std::string("Невірна ініціалізація масиву '") + name + std::string("[]' на позиції: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));
		
				token = buffer.next();

				if(token.getType() != LEFT_BRACKET)
					throw(AlgorithmException(std::string("Невірна ініціалізація масиву '") + name + std::string("[]' на позиції: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));

				std::vector<double> elems;

				do
				{
					elems.push_back(expr(buffer));
					token = buffer.next();

					if (token.getType() != RIGHT_BRACKET && token.getType() != COMMA)
						throw(AlgorithmException(std::string("Невірна ініціалізація масиву '") + name + std::string("[]' на позиції: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));
				} while (token.getType() == COMMA);

				mapArrays[name] = elems;
				return elems[0];
			}
			else
			{
				if (mapArrays.find(name) == mapArrays.end()) 
					throw(AlgorithmException(std::string("Масив '") + name + std::string("' не об'явлений! Позиція: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));

				int size = mapArrays[name].size();
				buffer.back();
				double index = expr(buffer);

				if(index < 0 || index > size - 1)
					throw(AlgorithmException(std::string("Вихід за границі масиву'") + name + std::string("[]' Позиція: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));

				token = buffer.next();

				if (token.getType() != SQ_RIGHT_BRACKET)
					throw(AlgorithmException(std::string("Несподіваний символ '") + token.getValue() + std::string("' на позиції: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));

				token = buffer.next();

				if (token.getType() != EQL)
				{
					buffer.back();
					return mapArrays[name][index];
				}
				else
				{
					double value = expr(buffer);
					mapArrays[name][index] = value;
					return mapArrays[name][index];
				}
			}
		}

		if (token.getType() == EQL)
		{
			if (mapArrays.find(name) != mapArrays.end())
				throw(AlgorithmException(std::string("Масив '") + name + std::string("' уже об'явлений! Позиція: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));

			mapVariables[name] = expr(buffer);

			token = buffer.next();

			//if (token.getType() != EOO && token.getType() != EOE )
				//throw(AlgorithmException(std::string("Несподіваний символ '") + token.getValue() + std::string("' на позиції: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));


			buffer.back();
			return mapVariables[name];
		}
		else buffer.back();


		if (mapVariables.find(name) == mapVariables.end())
			throw(AlgorithmException(std::string("Змінна '") + name + std::string("' не об'явлена! Позиція: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));
		else
			return mapVariables[name];
	}

	case FUNC:
		buffer.back();
		token = buffer.next();

		if (token.getValue() == "for")
		{
			value = forloop(buffer);
			buffer.next();
			return value;
		}
		if (token.getValue() == "forc")
		{
			value = forloopWithCondition(buffer);
			buffer.back();
			return value;
		}
		if (token.getValue() == "if")
		{
			value = if_statment(buffer);
			//buffer.back();
			return value;
		}
		else if (token.getValue() == "printf")
		{
			output(buffer);
			buffer.next();
			return 0;
		}
		else
		{
			buffer.back();
			return func(buffer);
		}

	case LEFT_BRACKET:
		value = expr(buffer);
		token = buffer.next();

		if (token.getType() != RIGHT_BRACKET)
			throw(AlgorithmException(std::string("Помилка! Очікується символ ')' на позиції ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));
	
		return value;
	default:
		throw(AlgorithmException(std::string("Несподіваний символ '") + token.getValue() + std::string("' на позиції: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));
	}

}

static double func(BufferTokens& buffer)
{
	std::string name = buffer.next().getValue();
	Token token = buffer.next();

	if (token.getType() != LEFT_BRACKET)
		throw(AlgorithmException(std::string("Неправильний виклик функції '" + name + std::string("' на позиції: ") + std::to_string(buffer.getPosition())), buffer.getCharPosition()));


	std::vector<double> args;


	if (token.getType() != RIGHT_BRACKET)
	{
		do
		{
			args.push_back(expr(buffer));
			token = buffer.next();

			if(token.getType() != RIGHT_BRACKET && token.getType() != COMMA)
				throw(AlgorithmException(std::string("Неправильний виклик функції '" + name + std::string("' на позиції: ") + std::to_string(buffer.getPosition())), buffer.getCharPosition()));

		} while (token.getType() == COMMA);
	}

	return mapFunctions[name](args);
}


//for(_var = 5; 10)(_var);
static double forloop(BufferTokens& buffer)
{

	if (buffer.next().getType() != LEFT_BRACKET)
		throw(AlgorithmException(std::string("Неправильний виклик функції 'for' на позиції: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));

	std::string name = buffer.next().getValue();
	buffer.back();

	double down = expr(buffer);
	buffer.next();
	double up = expr(buffer);

	if (buffer.next().getType() != RIGHT_BRACKET)
		throw(AlgorithmException(std::string("Неправильний виклик функції 'for' на позиції: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));

	buffer.next();
	int position = buffer.getPosition();

	int end_exp = 0;
	double sum = 0;

	for (double i = down; i < up; i++)
	{
		mapVariables[name] = i;
		sum += expr(buffer);
		end_exp = buffer.getPosition();
		buffer.setPostion(position);
	}

	buffer.setPostion(end_exp);
	return sum;
}



/*for (_i = 0; _i < 5; _i = _i + 1)
{
	printf(_i, '\n');
}*/

static double forloopWithCondition(BufferTokens& buffer)
{

	Token token = buffer.next();
	if (token.getType() != LEFT_BRACKET)
		throw(AlgorithmException(std::string("Неправильний виклик функції 'forc' на позиції: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));
	
	do
	{
		if (buffer.next().getType() == EOO)
			break;//throw(AlgorithmException(std::string("В циклі повинні бути лише змінні. Позиція: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));
		else
			buffer.back();
	

		expr(buffer);
		token = buffer.next();

		if (token.getType() != EOO && token.getType() != COMMA)
			throw(AlgorithmException(std::string("Неправильний виклик функції 'forc' на позиції: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));


	} while (token.getType() == COMMA);
	
	int postionCondition = buffer.getPosition();
	int begin_loop = 0;
	int end_loop = 0;
	int postion = 0;
	bool status = expr(buffer);
	

	if(status == false)
		return 0;

	buffer.next();

	postion = buffer.getPosition();

	while (token.getType() != F_LEFT_BRACKET)
	{
		
		token = buffer.next();

		if (token.getType() == EOE)
			throw(AlgorithmException(std::string("Не відкрита дужка в функції 'forc'"), WITHOUT_POSITION));

		begin_loop = buffer.getPosition();
	}

	while (status)
	{
		buffer.setPostion(begin_loop - 2);
		token = buffer.next();

		if(token.getType() != RIGHT_BRACKET)
			throw(AlgorithmException(std::string("Неправильний виклик функції 'forc' на позиції: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));
		
		
		if(buffer.next().getType() != F_LEFT_BRACKET)
			throw(AlgorithmException(std::string("Неправильний виклик функції 'forc' на позиції: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));

		do
		{
			if (buffer.next().getType() == F_RIGHT_BRACKET)
			{
				end_loop = buffer.getPosition();
				buffer.back();
				break;
			}
			else buffer.back();

			expr(buffer);

			buffer.isAvaliable();

			token = buffer.next();
			if (token.getType() != EOO && token.getType() != F_RIGHT_BRACKET)
				throw(AlgorithmException(std::string("Помилка! Несподіваний символ '") + token.getValue() + std::string("' на позиції: ") + std::to_string(buffer.getPosition()), buffer.getPosition()));

		} while (token.getType() == EOO || token.getType() == F_RIGHT_BRACKET);

		
		buffer.setPostion(postion);
		token = buffer.next();
		if (token.getType() == RIGHT_BRACKET)
		{
			buffer.setPostion(postionCondition);
		}
		else
		{
			buffer.back();

			do
			{
				//if (buffer.next().getType() != VAR)
					//throw(AlgorithmException(std::string("В циклі повинні бути лише змінні. Позиція: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));
				//buffer.back();


				expr(buffer);
				token = buffer.next();

				if (token.getType() != RIGHT_BRACKET && token.getType() != COMMA)
					throw(AlgorithmException(std::string("Неправильний виклик функції 'forc' на позиції: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));

			} while (token.getType() == COMMA);


		}

		buffer.setPostion(postionCondition);
		status = expr(buffer);
	}

	
	buffer.setPostion(end_loop - 1);

	if (buffer.next().getType() == F_LEFT_BRACKET)
		throw(AlgorithmException(std::string("Не закрита дужка в функції 'forc'") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));

	return 0;
}

static void output(BufferTokens& buffer)
{
	if (buffer.next().getType() != LEFT_BRACKET)
		throw(AlgorithmException(std::string("Помилка! Неправильний виклик функції 'printf' на позиції: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));

	if (buffer.next().getType() == RIGHT_BRACKET)
		throw(AlgorithmException("Помилка! Пуста функція printf()", WITHOUT_POSITION));

	buffer.back();

	Token token;
	double res;
	do
	{
		res = expr(buffer);

		if (!isnan(res))
			output_buffer.append(std::to_string(res));

		token = buffer.next();

		if (token.getType() != RIGHT_BRACKET && token.getType() != COMMA)
			throw(AlgorithmException(std::string("Помилка! Неправильний виклик функції 'printf' на позиції: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));

	} while (token.getType() == COMMA);

	buffer.back();
}


/*if (5 > 3)
{
	printf('hello world\n');
}
printf('bye world')*/

static double if_statment(BufferTokens& buffer)
{
	Token token = buffer.next();

	if (token.getType() != LEFT_BRACKET)
		throw(AlgorithmException(std::string("Помилка! Неправильний виклик функції 'if' на позиції: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));

	bool status = expr(buffer);

	token = buffer.next();

	if (token.getType() != RIGHT_BRACKET)
		throw(AlgorithmException(std::string("Помилка! Неправильний виклик функції 'if' на позиції: ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));
	
	token = buffer.next();

	if(token.getType() != F_LEFT_BRACKET)
		throw(AlgorithmException(std::string("Помилка! Очікується символ '{' на позиції ") + std::to_string(buffer.getPosition()), buffer.getCharPosition()));

	if (status)
	{
		do
		{
			if (buffer.next().getType() == F_RIGHT_BRACKET)
			{
				buffer.back();
				break;
			}
			else buffer.back();

			expr(buffer);

			buffer.isAvaliable();

			token = buffer.next();
			if(token.getType() != EOO && token.getType() != F_RIGHT_BRACKET)
				throw(AlgorithmException(std::string("Помилка! Несподіваний символ '") + token.getValue() + std::string("' на позиції: ") + std::to_string(buffer.getPosition()), buffer.getPosition()));
			
		} while (token.getType() == EOO || token.getType() == F_RIGHT_BRACKET);
	} 
	else
	{
		int count_brackets = 0;
		buffer.back();
		while(true)
		{
			token = buffer.next();

			if (token.getType() == F_RIGHT_BRACKET) count_brackets--;
			if (token.getType() == F_LEFT_BRACKET) count_brackets++;
			if (token.getType() == EOE)
				throw(AlgorithmException(std::string("Не закрита дужка в функції 'if'. Позиція:") + std::to_string(buffer.getPosition()), WITHOUT_POSITION));
			
			if (token.getType() == F_RIGHT_BRACKET && count_brackets == 0)
				break;
		} 

		buffer.back();
	}

	
	return 0;
}


void clear()
{
	mapArrays.clear();
	mapVariables.clear();
	output_buffer.clear();
}

