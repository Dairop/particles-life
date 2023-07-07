#pragma once
#include "utils.h"


std::string generate_rand_str_expression(short int max_depth);

class expression {
public:
	enum class expression_type { CONSTANT, VARIABLE, DISTANCE, ADDITION, SUBSTRACTION, MULTIPLICATION, DIVISION, SIN, COS, TAN, TANH, SQRT, RAND, NRAND, POW, ABS };
private:	
	expression_type _type;
	std::vector<std::shared_ptr<expression>> _children;
	float _constValue;
	unsigned int _varIndex;
public:
	expression_type stringToExpressionType(std::string& type);
	void updateExpression(const std::vector<std::string>& tokens, unsigned int start, unsigned int end);
	void updateExpression(const std::string& representation);
	expression(const std::string& representation);
	expression(const std::vector<std::string>& tokens, unsigned int start, unsigned int end);
	float applyFunction(float dist, const std::array<float, 5>& parameters) const;
};
