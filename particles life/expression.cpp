#include "expression.h"

bool letterIn(char letter, std::vector<char> letters) { // this function checks if a char is present in a set of chars
	for (unsigned int i = 0; i < letters.size(); i++) {
		if (letter == letters.at(i)) return true;
	}
	return false;
}

float operatorPriority(const std::string& op) { // some operators are treated before others, thanks to this function
	if (op == "+") return 0.3;
	if (op == "-") return 0.3;
	if (op == "*") return 0.6;
}

void expression::updateExpression(const std::vector<std::string> & tokens, unsigned int start, unsigned int end) { // this recursive function finds the less priority operator if it exists and creates a new node into the tree
	_children.clear();
	std::string token;
	float depth = 0;
	float priority;
	std::string highestToken; 
	float highestDepth = tokens.size()+1;
	unsigned int highestTokenIndex = 0;

	// we search the highest operator here
	for (unsigned int i = start; i <= end; i++) {
		token = tokens.at(i);
		if (token == "(") depth++;
		else if (token == ")") depth--;
		else if (token.size() == 1 && letterIn(token.at(0), { '+', '-', '*' })) {
			if (operatorPriority(token) + depth <= highestDepth) {
				highestDepth = operatorPriority(token) + depth;
				highestToken = token;
				highestTokenIndex = i;
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
				} else if (token == "&") {
					_type = expression_type::DISTANCE;
				} else {
					_type = expression_type::CONSTANT;
					_constValue = std::stof(token);
				}
				break;
			}
		}
	} else {// if a highest operator is found, we create children for the current expression
		if (highestToken == "+") _type = expression_type::ADDITION; 
		else if (highestToken == "-") _type = expression_type::SUBSTRACTION;
		else if (highestToken == "*") _type = expression_type::MULTIPLICATION;
		_children.push_back(std::make_shared<expression>(tokens, start, highestTokenIndex-1));
		_children.push_back(std::make_shared<expression>(tokens, highestTokenIndex +1, end));
	}
}

void expression::updateExpression(const std::string& representation) { // cut a string into a set of lexical units (tokens), and update the expression thanks to it
	std::vector<std::string> tokens;
	std::string word;
	char letter;
	for (unsigned int i = 0; i < representation.size(); i++) {
		letter = representation.at(i);
		if (letterIn(letter, {'0','1','2','3','4','5','6','7','8','9','.', '$'})) word += letter;
		else {
			if (!word.empty()) {
				tokens.push_back(word);
				word.clear();
			}
			if (letterIn(letter, { '(', ')', '+', '-', '*', '&'})) tokens.push_back(std::string(1, letter));
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
	else if (_type == expression_type::SUBSTRACTION) return _children.at(0)->applyFunction(dist, parameters) - _children.at(1)->applyFunction(dist, parameters);
	else if (_type == expression_type::MULTIPLICATION) return _children.at(0)->applyFunction(dist, parameters) * _children.at(1)->applyFunction(dist, parameters);
}