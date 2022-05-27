#pragma once
#include <string>

using namespace std;

class Exception
{
	string message;
public:
	Exception(string message) : message(message) {}

	string getMessage() {
		return "[ERROR] : " + this->message;
	}
};

