#pragma once
#include <iostream>
#include <string>
#include "User.h"

using namespace std;

class StandardUser : public User
{
public:
	StandardUser(string email, string name, string password, double balance) : User(email, name, password, balance) {};

	double cashBack(double cash, double reward);
};

