#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include <Windows.h> // text color
#include <algorithm> // transform
#include "Chip.h"
#include "User.h"
#include "PremiumUser.h"
#include "StandardUser.h"
#include "Exception.h"
#include "Color.h"

using namespace std;

// Bonus (in %)
const int BONUS = 12;
// Rounds
int rounds = 0;
// Authorized
bool authorized = false;
User* authorizedUser;
// Chips map
map<int, Chip*> chips;
// Current chip
Chip* currentChip;
// Previous numbers
list<int> prevNumbers;
// Fields
list<string> fields;
// Bets
map<string, double> bets;
double cashSpent = 0;

void changeTextColor(Color color) {
	switch (color) {
		case Color::RED:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
			break;
		case Color::GREEN:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			break;
		case Color::YELLOW:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
			break;
		case Color::ORANGE:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
			break;
		case Color::LIGHT_BLUE:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
			break;
		case Color::WHITE:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			break;
		case Color::STANDARD:
		default:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			break;
	}
}

Color convertStringToEnum(string color) {
	if (color.compare("red") == 0) return Color::RED;
	if (color.compare("green") == 0) return Color::GREEN;
	if (color.compare("yellow") == 0) return Color::YELLOW;
	if (color.compare("orange") == 0) return Color::ORANGE;
	if (color.compare("lightblue") == 0) return Color::LIGHT_BLUE;
	if (color.compare("white") == 0) return Color::WHITE;
	// Default
	return Color::STANDARD;
}

void showSectionName(string name) {
	changeTextColor(Color::YELLOW);
	cout << "[" << name << "]" << endl;
	changeTextColor(Color::STANDARD);
}

/* LOGIN - LOGIC ===============================================*/
void takeUserData(string& email, string& password) {
	cout << "=================" << endl;
	cout << "Email: ";
	cin >> email;
	cout << "Password: ";
	cin >> password;
	cout << "=================" << endl;
}

bool checkUserData(string email, string password) {
	string file = "data/users.txt";
	ifstream usersFile(file, ios::in);
	try {
		if (!usersFile.is_open()) throw Exception(file + " file is not open for reading data!");
		string email_a, name, password_a, premium;
		double balance;
		while (!usersFile.eof()) {
			usersFile >> email_a >> name >> password_a >> balance >> premium;
			if (email_a.compare(email) == 0 && password_a.compare(password) == 0) {
				if (premium.compare("true") == 0) {
					authorizedUser = new PremiumUser(email_a, name, password_a, balance, BONUS);
				}
				else {
					authorizedUser = new StandardUser(email_a, name, password_a, balance);
				}
				return true;
			}
		}
	}
	catch (Exception exception) {
		changeTextColor(Color::ORANGE);
		cout << exception.getMessage() << endl;
		changeTextColor(Color::STANDARD);
	}
	usersFile.close();
	return false;
}

void showLogin() {
	bool correctData = true;
	string email;
	string password;
	do {
		if (correctData) {
			cout << "Welcome! Please login to able to play." << endl;
		}
		else {
			changeTextColor(Color::ORANGE);
			cout << "Incorrect data! Please try again!" << endl;
			changeTextColor(Color::STANDARD);
		}
		takeUserData(email, password);
		correctData = checkUserData(email, password);
	} while (!correctData);
	changeTextColor(Color::GREEN);
	cout << "Successfully logged in!" << endl;
	changeTextColor(Color::STANDARD);
	// Setting authorized to true
	authorized = true;
}
/* =============================================================*/
/* CHIPS - LOGIC ===============================================*/
void loadChips() {
	string file = "data/chips.txt";
	ifstream chipsFile(file, ios::in);
	try {
		if (!chipsFile.is_open()) throw Exception(file + " file is not open for reading data!");
		double value;
		string color;
		int counter = 0;
		while (!chipsFile.eof()) {
			chipsFile >> value >> color;
			Chip* chip = new Chip(value, color);
			chips.insert({ counter, chip });
			counter++;
		}
	}
	catch (Exception exception) {
		changeTextColor(Color::ORANGE);
		cout << exception.getMessage() << endl;
		changeTextColor(Color::STANDARD);
	}
	chipsFile.close();
}

void showChips() {
	cout << "_____________________________________________________________" << endl;
	showSectionName("CHIPS");
	int chipCounter = 1;
	map<int, Chip*>::iterator chipIterator;
	for (chipIterator = chips.begin(); chipIterator != chips.end(); chipIterator++) {
		changeTextColor(convertStringToEnum(chipIterator->second->getColor()));
		cout << "Chip[" << chipCounter++ << "] -> " << chipIterator->second->getColor() << "[ value: " << chipIterator->second->getValue() << " ]" << endl;
	}
	changeTextColor(Color::STANDARD);
	cout << "_____________________________________________________________" << endl;
}
/* =============================================================*/
/* MESSAGES ====================================================*/
void showMaintenanceMessage() {
	changeTextColor(Color::LIGHT_BLUE);
	cout << "[MAINTENANCE] : This action is not supported yet! It will be completed soon." << endl;
	changeTextColor(Color::STANDARD);
}

void showWelcomeMessage() {
	cout << "Welcome, " << authorizedUser->getName() << "! Here is the menu.." << endl;
};
/* =============================================================*/
/* CHOOSING - LOGIC ============================================*/
template <typename T>
bool contains(T chosenOption, list<T> options) {
	for (T option : options) {
		if (chosenOption == option) {
			return true;
		}
	}
	return false;
}

int choose(list<int> options) {
	int option;
	bool valid = false;
	while (!valid) {
		try {
			cout << "I choose: ";
			cin >> option;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(256, '\n');
				throw Exception("Please enter the number!");
			}
			if (!contains(option, options)) {
				throw Exception("Please enter the valid option!");
			}
			valid = true;
		}
		catch (Exception exception) {
			cout << exception.getMessage() << endl;
		}
	}
	cout << endl;
	return option;
}

string choose(list<string> fields) {
	string field;
	bool valid = false;
	while (!valid) {
		cout << "Place chip [" << currentChip->getValue() << "] on: ";
		cin >> field;
		transform(field.begin(), field.end(), field.begin(), tolower);
		try {
			if (!contains(field, fields)) {
				throw Exception("Please enter the valid field!");
			}
			valid = true;
		}
		catch (Exception exception) {
			cout << exception.getMessage() << endl;
		}
	}
	return field;
}

void fillFields() {
	// 0, 1, 2,..., 34, 35, 36
	int number = 0;
	while (number < 37) {
		fields.push_back(to_string(number++));
	}
	// 1st, 2nd, 3rd
	fields.push_back("1st");
	fields.push_back("2nd");
	fields.push_back("3rd");
	// 1st12, 2nd12, 3rd12
	fields.push_back("1st12");
	fields.push_back("2nd12");
	fields.push_back("3rd12");
	// 1-18, 19-36
	fields.push_back("1-18");
	fields.push_back("19-36");
	// even, odd
	fields.push_back("even");
	fields.push_back("odd");
	// red, black
	fields.push_back("red");
	fields.push_back("black");
	// bet
	fields.push_back("bet");
}
/* =============================================================*/
/* PREV NUMBERS - LOGIC ========================================*/
void loadPrevNumbers() {
	string file = "data/prevNumbers.txt";
	ifstream prevNumbersFile(file, ios::in);
	try {
		if (!prevNumbersFile.is_open()) throw Exception(file + " file is not open for reading data!");
		int necessary = 10;
		int number;
		while (!prevNumbersFile.eof() && necessary != 0) {
			prevNumbersFile >> number;
			prevNumbers.push_back(number);
			necessary--;
		}
	}
	catch (Exception exception) {
		changeTextColor(Color::ORANGE);
		cout << exception.getMessage() << endl;
		changeTextColor(Color::STANDARD);
	}
}

void recordPrevNumbers() {
	string file = "data/prevNumbers.txt";
	ofstream prevNumbersFile(file, ios::trunc | ios::out);
	try {
		if (!prevNumbersFile.is_open()) throw Exception(file + " file is not open for data entry!");
		list<int>::iterator numIterator = prevNumbers.begin();
		while (numIterator != prevNumbers.end()) {
			prevNumbersFile << *numIterator << endl;
			numIterator++;
		}
	}
	catch (Exception exception) {
		changeTextColor(Color::ORANGE);
		cout << exception.getMessage() << endl;
		changeTextColor(Color::STANDARD);
	}
	prevNumbersFile.close();
}
/* =============================================================*/
/* SHOW ========================================================*/
void showPrevNumbers() {
	cout << "Last 10 rounds: [";
	for (int number : prevNumbers) {
		cout << number << " ";
	}
	cout << "]" << endl;
}

void showRoundInfo() {
	cout << "_____________________________________________________________" << endl;
	showSectionName("ROUND INFO");
	showPrevNumbers();
	cout << "LAST NUMBER: ";
	changeTextColor(Color::ORANGE);
	cout << prevNumbers.back() << endl;
	changeTextColor(Color::STANDARD);
	cout << "BALANCE: ";
	changeTextColor(Color::GREEN);
	cout << authorizedUser->getBalance() << "$" << endl;
	changeTextColor(Color::STANDARD);
	cout << "Current chip: ";
	changeTextColor(convertStringToEnum(currentChip->getColor()));
	cout << currentChip->getColor() << "[ value: " << currentChip->getValue() << " ]" << endl;
	changeTextColor(Color::STANDARD);
	cout << "_____________________________________________________________" << endl;
}

void showGuide() {
	showSectionName("GUIDE");
	cout << "Here is the guide. Let's take a look! " << endl;
	cout << "1. If you want to bet on a certain number, just enter its value, then enter the number of chips you want to bet. " << endl;	
	cout << "2. If you want to bet on EVEN numbers, just type \"even\" (or \"EVEN\"). The same way is for the ODD numbers. " << endl;
	cout << "3. If you want to bet on 1st 12, just type \"1st12\" (or \"1ST12\"). The same thing is for the other fields of the table. " << endl;
	cout << "4. If you want to start the round, type the word \"bet\". " << endl;
}

void showTable() {
	cout << "   ______________________________________________________________________________________________" << endl;
	cout << "  |     |     |     |     |      |      |      |      |      |      |      |      |      |       |" << endl;
	cout << "  |     |  3  |  6  |  9  |  12  |  15  |  18  |  21  |  24  |  27  |  30  |  33  |  36  |  3rd  |" << endl;
	cout << "  |     |_____|_____|_____|______|______|______|______|______|______|______|______|______|_______|" << endl;
	cout << "  |     |     |     |     |      |      |      |      |      |      |      |      |      |       |" << endl;
	cout << "  |  0  |  2  |  5  |  8  |  11  |  14  |  17  |  20  |  23  |  26  |  29  |  32  |  35  |  2nd  |" << endl;
	cout << "  |     |_____|_____|_____|______|______|______|______|______|______|______|______|______|_______|" << endl;
	cout << "  |     |     |     |     |      |      |      |      |      |      |      |      |      |       |" << endl;
	cout << "  |     |  1  |  4  |  7  |  10  |  13  |  16  |  19  |  22  |  25  |  28  |  31  |  34  |  1st  |" << endl;
	cout << "  |_____|_____|_____|_____|______|______|______|______|______|______|______|______|______|_______|" << endl;
	cout << "        |                        |                           |                           |" << endl;
	cout << "        |         1st 12         |           2nd 12          |           3rd 12          |" << endl;
	cout << "        |________________________|___________________________|___________________________|" << endl;
	cout << "        |           |            |             |             |             |             |" << endl;
	cout << "        |    1-18   |    EVEN    |     RED     |    BLACK    |     ODD     |    19-36    |" << endl;
	cout << "        |___________|____________|_____________|_____________|_____________|_____________|" << endl;
}

void showMenu(int level) {
	cout << "_____________________________________________________________" << endl;
	if (level == 0) {
		showSectionName("MENU");
		cout << "[1] play the game    [2] guide    [3] end game" << endl;
	}
	if (level == 1) {
		showSectionName("OPTIONS");
		cout << "[1] set the chips    [2] change current chip    [3] skip round    [4] back to menu" << endl;
	}
	cout << "_____________________________________________________________" << endl;
}
/* =============================================================*/

void recordNewBalance() {
	string file = "data/users.txt";
	ifstream usersFileInput(file, ios::in);
	string record;
	try {
		if (!usersFileInput.is_open()) throw Exception(file + " file is not open for reading data!");
		string email, name, password, premium;
		double balance;
		while (!usersFileInput.eof()) {
			usersFileInput >> email >> name >> password >> balance >> premium;
			if (email.compare(authorizedUser->getEmail()) == 0) {
				record.append(email + " " + name + " " + password + " " + to_string(authorizedUser->getBalance()) + " " + premium + "\n");
			}
			else {
				record.append(email + " " + name + " " + password + " " + to_string(balance) + " " + premium);
				if (!usersFileInput.eof()) {
					record.append("\n");
				}
			}
		}
	}
	catch (Exception exception) {
		changeTextColor(Color::ORANGE);
		cout << exception.getMessage() << endl;
		changeTextColor(Color::STANDARD);
	}
	usersFileInput.close();
	ofstream usersFileOutput(file, ios::trunc | ios::out);
	try {
		if (!usersFileOutput.is_open()) throw Exception(file + " file is not open for data entry!");
		usersFileOutput << record.substr(0, record.length() - 1);
	}
	catch (Exception exception) {
		changeTextColor(Color::ORANGE);
		cout << exception.getMessage() << endl;
		changeTextColor(Color::STANDARD);
	}
	usersFileOutput.close();
}

void setTheChips() {
	bool setting = true;
	while (setting) {
		string field = choose(fields);
		bool userHasMoney = false;
		if (field.compare(fields.back()) != 0) {
			do {
				try {
					int bet = 1;
					cout << "How many chips? ";
					cin >> bet;
					cout << endl;
					double betMoney = bet * currentChip->getValue();
					if (betMoney <= authorizedUser->getBalance()) {
						userHasMoney = true;
						authorizedUser->setBalance(authorizedUser->getBalance() - betMoney);
						cashSpent += betMoney;
						bets.insert({ field, betMoney });
					}
					else {
						throw Exception("You don't have enough money! [bet: " + to_string(betMoney) + " < balance: " + to_string(authorizedUser->getBalance()) + "]");
					}
				}
				catch (Exception exception) {
					cout << exception.getMessage() << endl;
				}
			} while (!userHasMoney);
		}
		else {
			setting = false;
		}
	}
}

void changeCurrentChip() {
	showChips();
	int chosenOption = choose({ 1, 2, 3, 4, 5});
	currentChip = chips.at(chosenOption - 1);
}

int row(int number) {
	// 0 - 0, 1 - first row, 2 - second row, 3 - third row
	list<int> firstRowNumbers, secondRowNumbers, thirdRowNumbers;

	for (int i = 1; i <= 12; i++) {
		firstRowNumbers.push_back(i);
		firstRowNumbers.push_back(i + 1);
		firstRowNumbers.push_back(i + 2);
	}

	if (contains(number, firstRowNumbers)) return 1;
	if (contains(number, secondRowNumbers)) return 2;
	if (contains(number, thirdRowNumbers)) return 3;
	return 0;
}

double reward(int number) {
	bool premiumUser = typeid(*authorizedUser) == typeid(PremiumUser);
	double reward = 0;
	map<string, double>::iterator betIterator;
	list<string>::iterator fieldIterator;
	for (betIterator = bets.begin(); betIterator != bets.end(); betIterator++) {
		if ((betIterator->first.compare("1st12") == 0 && number <= 12) ||
			(betIterator->first.compare("2nd12") == 0 && (number > 12 && number <= 24)) ||
			(betIterator->first.compare("3rd12") == 0 && number > 24) ||
			(betIterator->first.compare("1st") == 0 && row(number) == 1) ||
			(betIterator->first.compare("2nd") == 0 && row(number) == 2) ||
			(betIterator->first.compare("3rd") == 0 && row(number) == 3)) {
			reward += betIterator->second * 3;
		} else if ((betIterator->first.compare("even") == 0 && number % 2 == 0) ||
			(betIterator->first.compare("red") == 0 && number % 2 == 0) ||
			(betIterator->first.compare("odd") == 0 && number % 2 != 0) ||
			(betIterator->first.compare("black") == 0 && number % 2 != 0) ||
			(betIterator->first.compare("1-18") == 0 && (number >= 1 && number <= 18)) ||
			(betIterator->first.compare("19-36") == 0 && number >= 19)) {
			reward += betIterator->second * 2;
		} else {
			if (betIterator->first.compare(to_string(number)) == 0) {
				reward += betIterator->second * 36;
			}
		}	
	}
	if (premiumUser && rounds != 0 && rounds % 10 == 0) {
		return reward + (reward * (BONUS / 100));
	}
	else {
		return reward;
	}
}

void bringItOn() {
	srand(time(NULL));
	int randomNumber = rand() % 37;
	if (prevNumbers.size() == 10) {
		prevNumbers.pop_front();
	}
	prevNumbers.push_back(randomNumber);
	double rewardValue = reward(randomNumber);
	rewardValue += authorizedUser->cashBack(cashSpent, rewardValue);
	authorizedUser->setBalance(authorizedUser->getBalance() + rewardValue);
	bets.clear();
	rounds++;
	cashSpent = 0;
}

void playTheGame() {
	bool playing = true;
	currentChip = chips.at(0);
	showChips();
	showRoundInfo();
	showTable();
	fillFields();
	while (playing) {
		showMenu(1);
		int chosenOption = choose({ 1, 2, 3, 4 });

		switch (chosenOption) {
		case 1:
			setTheChips();
			bringItOn();
			break;
		case 2:
			changeCurrentChip();
			break;
		case 3:
			// Skip round;
			bringItOn();
			break;
		case 4:
		default:
			playing = false;
			break;
		}
		showRoundInfo();
		showTable();
	}
}

/* MAIN ========================================================*/
int main() {
	showLogin();
	if (authorized) {
		loadChips();
		loadPrevNumbers();
		bool playing = true;
		showWelcomeMessage();
		while (playing) {
			showMenu(0);
			int choosenOption = choose({ 1, 2, 3 });
			
			switch (choosenOption) {
			case 1:
				playTheGame();
				break;
			case 2:
				showGuide();
				break;
			case 3:
			default:
				playing = false;
				recordPrevNumbers();
				recordNewBalance();
				break;
			}
		}
	}
	return 0;
}