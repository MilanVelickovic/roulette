#pragma once
#include <iostream>
#include <string>

using namespace std;

class User
{
protected:
	string email;
	string name;
	string password;
	double balance;
public:
	User(string email, string name, string password, double balance) : email(email), name(name), password(password), balance(balance) {};
	~User();

	string getEmail() {
		return this->email;
	}

	string getName() {
		return this->name;
	}

	string getPassword() {
		return this->password;
	}

	double getBalance() {
		return balance;
	}

	void setBalance(double balance) {
		this->balance = balance;
	}

	virtual double cashBack(double cash, double reward) = 0;
};