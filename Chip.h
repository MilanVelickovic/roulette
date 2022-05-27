#pragma once
#include <string>

using namespace std;

class Chip
{
	double value;
	string color;
public:
	Chip(double value, string color) : value(value), color(color) {}
	~Chip();

	double getValue() {
		return this->value;
	}

	string getColor() {
		return this->color;
	}
};