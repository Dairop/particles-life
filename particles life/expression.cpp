#include "expression.h"

bool letterIn(char letter, std::vector<char> letters) { // this function checks if a char is present in a set of chars
	for (unsigned int i = 0; i < letters.size(); i++) {
		if (letter == letters.at(i)) return true;
	}
	return false;
}

bool tokenIn(std::string token, std::vector<std::string> tokens) { // this function checks if a char is present in a set of chars
	for (unsigned int i = 0; i < tokens.size(); i++) {
		if (token == tokens.at(i)) return true;
	}
	return false;
}

expression::expression_type expression::stringToExpressionType(std::string & type) {
	if (type == "+") return expression_type::ADDITION;
	if (type == "-") return expression_type::SUBSTRACTION;
	if (type == "*") return expression_type::MULTIPLICATION;
	if (type == "/") return expression_type::DIVISION;
	if (type == "sin") return expression_type::SIN;
	if (type == "cos") return expression_type::COS;
	if (type == "tan") return expression_type::TAN;
	if (type == "tanh") return expression_type::TANH;
	if (type == "sqrt") return expression_type::SQRT;
	if (type == "rand") return expression_type::RAND;
	if (type == "nrand") return expression_type::NRAND;
	if (type == "pow") return expression_type::POW;
	if (type == "abs") return expression_type::ABS;
}


float operatorPriority(const std::string& op) { // some operators are treated before others, thanks to this function
	if (op == "+" || op == "-") return 0.2;
	if (op == "*" || op == "/") return 0.4;
	return 0.5;
}

float valueInInterval(float a, float b, float cursor) {
	return a+(b-a)*cursor;
}

void expression::updateExpression(const std::vector<std::string> & tokens, unsigned int start, unsigned int end) { // this recursive function finds the less priority operator if it exists and creates a new node into the tree
	_children.clear();
	std::string token;
	float depth = 0;
	float priority;
	std::string highestToken; 
	float highestDepth = tokens.size()+1;
	unsigned int highestTokenIndex = 0;
	std::vector<unsigned int> commas;
	float commasDepth = 0;
	bool commasCollection = false;

	// we search the highest operator here
	for (unsigned int i = start; i <= end; i++) {
		token = tokens.at(i);
		if (token == "(") depth++;
		else if (token == ")") depth--;
		else if (token == ",") {
			if (commasCollection && depth == commasDepth) commas.push_back(i);
		} else if (tokenIn(token, { "+", "-", "*", "/", "sin", "cos", "tan", "tanh", "sqrt", "rand", "nrand", "pow", "abs"})) {
			if (operatorPriority(token) + depth <= highestDepth) {
				highestDepth = operatorPriority(token) + depth;
				highestToken = token;
				highestTokenIndex = i;
				commas.clear();
				commasCollection = (i<tokens.size()-1 && tokens.at(i+1)=="(");
				commasDepth = depth + 1;
			}
		}
	}

	// if no highest operator is found, the expression takes the value
	if (highestToken.empty()) {
		for (unsigned int i = start; i <= end; i++) {
			token = tokens.at(i);
			if (token != "(") {
				if (token.at(0) == '$') {
					_type = expression_type::VARIABLE;
					_varIndex = std::stoi(token.substr(1, token.size()));
				}
				else if (token == "&") {
					_type = expression_type::DISTANCE;
				} else if (token != ",") {
					_type = expression_type::CONSTANT;
					_constValue = std::stof(token);
				}
				break;
			}
		}
	} else {// if a highest operator is found, we create children for the current expression
		_type = stringToExpressionType(highestToken);		
		if (highestTokenIndex > start) _children.push_back(std::make_shared<expression>(tokens, start, highestTokenIndex-1));
		unsigned int lastComma = highestTokenIndex;
		commas.push_back(end+1);
		for (unsigned int i = 0; i < commas.size(); i++) {
			_children.push_back(std::make_shared<expression>(tokens, lastComma+1, commas.at(i)-1));
			lastComma = commas.at(i);
		}
		
		if (_type == expression_type::NRAND) _constValue = (rand() % 1000) / 1000.0;
		else if (_type == expression_type::RAND) _varIndex = rand() % _children.size();
	}
}

void expression::updateExpression(const std::string& representation) { // cut a string into a set of lexical units (tokens), and update the expression thanks to it
	std::vector<std::string> tokens;
	std::string word;
	char letter;
	for (unsigned int i = 0; i < representation.size(); i++) {
		letter = representation.at(i);
		if (! letterIn(letter, { '(', ')', '+', '-', '*', '/', '&', ','})) word += letter;
		else {
			if (!word.empty()) {
				tokens.push_back(word);
				word.clear();
			}
			tokens.push_back(std::string(1, letter));
		}
	}
	if (!word.empty()) tokens.push_back(word);

	updateExpression(tokens, 0, tokens.size()-1);
}

expression::expression(const std::string& representation) {
	updateExpression(representation);
}

expression::expression(const std::vector<std::string>& tokens, unsigned int start, unsigned int end) {
	updateExpression(tokens, start, end);
}

float expression::applyFunction(float dist, const std::array<float, 5> & parameters) const { //calculate the expression with the given parameters
	if (_type == expression_type::VARIABLE) return parameters.at(_varIndex);
	else if (_type == expression_type::CONSTANT) return _constValue;
	else if (_type == expression_type::DISTANCE) return dist;
	else if (_type == expression_type::ADDITION) return _children.at(0)->applyFunction(dist, parameters) + _children.at(1)->applyFunction(dist, parameters);
	else if (_type == expression_type::SUBSTRACTION) {
		if (_children.size() == 1) return -_children.at(0)->applyFunction(dist, parameters);
		return _children.at(0)->applyFunction(dist, parameters) - _children.at(1)->applyFunction(dist, parameters);
	}
	else if (_type == expression_type::MULTIPLICATION) return _children.at(0)->applyFunction(dist, parameters) * _children.at(1)->applyFunction(dist, parameters);
	else if (_type == expression_type::DIVISION) {
		float a = _children.at(0)->applyFunction(dist, parameters);
		float b = _children.at(1)->applyFunction(dist, parameters);
		if (b == 0) return a;
		return a / b;
	}
	else if (_type == expression_type::SIN) return sin(_children.at(0)->applyFunction(dist, parameters));
	else if (_type == expression_type::COS) return cos(_children.at(0)->applyFunction(dist, parameters));
	else if (_type == expression_type::TAN) return tan(_children.at(0)->applyFunction(dist, parameters));
	else if (_type == expression_type::TANH) return tanh(_children.at(0)->applyFunction(dist, parameters));
	else if (_type == expression_type::SQRT) return sqrt(abs(_children.at(0)->applyFunction(dist, parameters)));
	else if (_type == expression_type::RAND) return _children.at(_varIndex)->applyFunction(dist, parameters);
	else if (_type == expression_type::NRAND) return valueInInterval(_children.at(0)->applyFunction(dist, parameters), _children.at(1)->applyFunction(dist, parameters), _constValue);
	else if (_type == expression_type::POW) return pow(_children.at(0)->applyFunction(dist, parameters), _children.at(1)->applyFunction(dist, parameters));
	else if (_type == expression_type::ABS) return abs(_children.at(0)->applyFunction(dist, parameters));
}