#include "expression.h"



std::string generate_rand_str_expression(short int max_depth, bool prev_was_minus_sign) { 
	std::vector<std::string> unary_functions = { "abs", "sin", "sqrt", "tanh", "tan" };
	std::vector<std::string> two_var_functions = { "nrand", "pow"};
	std::vector<std::string> n_var_functions = { "rand" };

	std::vector<std::string> unary_operators = { "-" };
	std::vector<char> two_var_operators = { '+', '-', '*', '/'};
	
	//some appear multiple times to be chosen more often
	std::vector<std::string> variables = 
		{ "$0", "$1", "$2", "$3", "$4", "&", "&*0.1", "&*0.01" ,
		  "&*&/40000" , "&*&/40000", "&*&/40000", "&*&/-40000" , "&*&/-40000", "cos(&*&/40000)",
		  "1", "1.61803", "2", "2,71828", "3", "3.14159",
		  "-1", "-1.61803", "-2", "-2.71828", "-3", "-3.14159"
		};



	//probability of the type of sub-expression
	float prob_unary_functions = 0.3f;
	float prob_two_var_functions = 0.1f;
	float prob_n_var_functions = 0.05f;
	float prob_unary_operators = 0.1f * !prev_was_minus_sign;
	float prob_two_var_operators = 1.5f;
	float prob_variables = 0.4f;

	float total_probs = prob_unary_functions + prob_two_var_functions + prob_n_var_functions + prob_unary_operators + prob_two_var_operators + prob_variables;

	//don't go above the depth limit (except for the "&*&/40000" variable and some other composed ones)
	if (max_depth <= 1) {
		return '(' + variables.at(rand() % variables.size()) + ')';
	}

	//choose which string we will return
	float score = randFloat() * total_probs;



	if (score < prob_unary_functions) {
		/* eg:  abs( ... )    */
		return unary_functions.at(rand() % unary_functions.size()) 
			+ "(" + 
				generate_rand_str_expression(max_depth-1, false) 
			+ ")";
	} else {
		score -= prob_unary_functions;
	}

	if (score < prob_two_var_functions) {
		/* eg:  nrand( ... , ... )    */
		return two_var_functions.at(rand() % two_var_functions.size())
			+ "(" +
				generate_rand_str_expression(max_depth - 1, false) + "," + generate_rand_str_expression(max_depth - 1, false)
			+ ")";
	}
	else {
		score -= prob_two_var_functions;
	}

	if (score < prob_n_var_functions) {
		/* eg:  rand( ... , ... , ..., ... )    */
		std::string tmp_str = n_var_functions.at(rand() % n_var_functions.size()) + "(";

		const short MAX_NB_OF_PARAMETERS = 6;
		for (unsigned int i = 0; i < MAX_NB_OF_PARAMETERS - 1; i++) {
			tmp_str += generate_rand_str_expression(max_depth - 1, false) + ",";
		}
		tmp_str += generate_rand_str_expression(max_depth - 1, false) + ")";

		return  tmp_str;
	}
	else {
		score -= prob_n_var_functions;
	}

	if (score < prob_unary_operators) {
		/* eg:  -...    */
		return unary_operators.at(rand() % unary_operators.size()) + '(' + generate_rand_str_expression(max_depth - 1, false) +')';
	}
	else {
		score -= prob_unary_operators;
	}

	if (score < prob_two_var_operators) {
		/* eg:  ... + ...    */
		return	generate_rand_str_expression(max_depth - 1, false) +
					two_var_operators.at(rand() % two_var_operators.size()) + 
				generate_rand_str_expression(max_depth - 1, false);
	}
	else {
		score -= prob_two_var_operators;
	}

	if (score < prob_variables) {
		/* eg: ...    */
		return variables.at(rand() % variables.size());
	}
	else {
		score -= prob_variables;
	}


	std::cout << "problem";
	return ""; // ???

}








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
	std::string lowestToken;
	float lowestDepth = tokens.size()+1;
	unsigned int lowestTokenIndex = 0;
	std::vector<unsigned int> commas;
	float commasDepth = 0;
	bool commasCollection = false;
	int firstTokenIndex = -1;

	// we search the lowest operator here
	for (unsigned int i = start; i <= end; i++) {
		token = tokens.at(i);
		if (token == "(") depth++;
		else if (token == ")") {
			depth--;
			if (depth < commasDepth) commasCollection = false;
		}
		else if (token == ",") {
			if (commasCollection && depth == commasDepth) commas.push_back(i);
		} else {
			if (firstTokenIndex == -1) firstTokenIndex = i;
			if (tokenIn(token, { "+", "-", "*", "/", "sin", "cos", "tan", "tanh", "sqrt", "rand", "nrand", "pow", "abs" })) {
				if (operatorPriority(token) + depth <= lowestDepth) {
					lowestDepth = operatorPriority(token) + depth;
					lowestToken = token;
					lowestTokenIndex = i;
					commas.clear();
					commasDepth = depth + 1;
					commasCollection = (i < tokens.size() - 1 && tokens.at(i + 1) == "(");					
				}
			}
		}
	}

	// if no lowest operator is found, the expression takes the value
	if (lowestToken.empty()) {
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
	} else {// if a lowest operator is found, we create children for the current expression
		_type = stringToExpressionType(lowestToken);		
		if (firstTokenIndex != -1 && firstTokenIndex < lowestTokenIndex) _children.push_back(std::make_shared<expression>(tokens, start, lowestTokenIndex-1));
		unsigned int lastComma = lowestTokenIndex;
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
		if (letterIn(letter, { '(', ')', '+', '-', '*', '/', '&', ',' })) {
			if (!word.empty()) {
				tokens.push_back(word);
				word.clear();
			}
			tokens.push_back(std::string(1, letter));
		}
		else word += letter;
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
	if (_type == expression_type::CONSTANT) return _constValue;
	if (_type == expression_type::DISTANCE) return dist;
	if (_type == expression_type::ADDITION) return _children.at(0)->applyFunction(dist, parameters) + _children.at(1)->applyFunction(dist, parameters);
	if (_type == expression_type::SUBSTRACTION) {
		if (_children.size() == 1) return -_children.at(0)->applyFunction(dist, parameters);
		return _children.at(0)->applyFunction(dist, parameters) - _children.at(1)->applyFunction(dist, parameters);
	}
	if (_type == expression_type::MULTIPLICATION) return _children.at(0)->applyFunction(dist, parameters) * _children.at(1)->applyFunction(dist, parameters);
	if (_type == expression_type::DIVISION) {
		float a = _children.at(0)->applyFunction(dist, parameters);
		float b = _children.at(1)->applyFunction(dist, parameters);
		if (b == 0) return a;
		return a / b;
	}
	if (_type == expression_type::SIN) return sin(_children.at(0)->applyFunction(dist, parameters));
	if (_type == expression_type::COS) return cos(_children.at(0)->applyFunction(dist, parameters));
	if (_type == expression_type::TAN) return tan(_children.at(0)->applyFunction(dist, parameters));
	if (_type == expression_type::TANH) return tanh(_children.at(0)->applyFunction(dist, parameters));
	if (_type == expression_type::SQRT) return sqrt(abs(_children.at(0)->applyFunction(dist, parameters)));
	if (_type == expression_type::RAND) return _children.at(_varIndex)->applyFunction(dist, parameters);
	if (_type == expression_type::NRAND) return valueInInterval(_children.at(0)->applyFunction(dist, parameters), _children.at(1)->applyFunction(dist, parameters), _constValue);
	if (_type == expression_type::POW) return pow(_children.at(0)->applyFunction(dist, parameters), _children.at(1)->applyFunction(dist, parameters));
	if (_type == expression_type::ABS) return abs(_children.at(0)->applyFunction(dist, parameters));
}