#pragma once
#include <iostream>
#include <string>
#include "User.h"

using namespace std;

class PremiumUser : public User
{
	double bonus;
public:
	PremiumUser(string email, string name, string password, double balance, double bonus) : User(email, name, password, balance), bonus(bonus) {};

	double getBonus() {
		return this->bonus;
	}

	void setBonus(double bonus) {
		this->bonus = bonus;
	}

	double cashBack(double cash, double reward);
};

