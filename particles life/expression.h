#pragma once
#include <memory>
#include <iostream>
#include <array>
#include <vector>
#include <string>

class expression {
	enum class expression_type { CONSTANT, VARIABLE, DISTANCE, ADDITION, SUBSTRACTION, MULTIPLICATION, DIVISION, SIN, COS, TAN, TANH, SQRT };
	expression_type _type;
	std::vector<std::shared_ptr<expression>> _children;
	float _constValue;
	unsigned int _varIndex;
public:
	void updateExpression(const std::vector<std::string>& tokens, unsigned int start, unsigned int end);
	void updateExpression(const std::string& representation);
	expression(const std::string& representation);
	expression(const std::vector<std::string>& tokens, unsigned int start, unsigned int end);
	float applyFunction(float dist, const std::array<float, 5>& parameters) const;
};
