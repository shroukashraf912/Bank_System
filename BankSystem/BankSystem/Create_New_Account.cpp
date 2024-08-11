#include <iostream>
#include <cstdlib>
#include <ctime>
#include <set>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <string>
#include <stdexcept>

#include <sstream> // For std::stringstream
#include <vector> // For std::vector
#include <algorithm> // Include this for std::all_of
#include <random>

using namespace std;

#include "Create_New_Account.h"
#include "Admin.h"

// Constructor
Create_New_Account::Create_New_Account()
	: BankAccountId(GenerateBankAccountId()), Password(GeneratePassword(8)), AccountStatus("Active") {}

// Setters
void Create_New_Account::SetFullName(const std::string& name) 
{ 

	// Split the name into words and count them
	stringstream ss(name);
	string word;
	//Stores the words in a vector<string>.
	//Checks if the vector contains at least four words.If not, it prints an erro
	vector<string> words;

	while (ss >> word)
	{
		words.push_back(word);
	}

	// Check if there are at least four words
	if (words.size() < 4)
	{
		cout << "Error: You must enter at least four names. Please try again." << endl;
		return;
	}

	FullName = name;
}
void Create_New_Account::SetFullAddress(const std::string& address) { FullAddress = address; }
void Create_New_Account::SetNationalId(const std::string& NID) 
{
	if (NID.length() != 14 || !all_of(NID.begin(), NID.end(), ::isdigit))
	{
		cout << "Error: National ID must consist of exactly 14 digits. Please try again." << endl;
		return;
	}
	NationalId = NID;
}
void Create_New_Account::SetAge(short AGE) { Age = AGE; }
void Create_New_Account::SetGuardian(const std::string& Guard) 
{ 
	// Split the name into words and count them
	stringstream ssg(Guard);
	string name;
	//Stores the words in a vector<string>.
	//Checks if the vector contains at least four words.If not, it prints an erro
	vector<string> names;

	while (ssg >> name)
	{
		names.push_back(name);
	}

	// Check if there are at least four words
	if (names.size() < 4)
	{
		cout << "Error: You must enter at least four names. Please try again." << endl;
		return;
	}

	Guardian = Guard;
}
void Create_New_Account::SetGuardianNationalId(const std::string& GNID) 
{
	if (GNID.length() != 14 || !all_of(GNID.begin(), GNID.end(), ::isdigit))
	{
		cout << "Error: Guardian National ID must consist of exactly 14 digits. Please try again." << endl;
		return;
	}
	GuardianNationalId = GNID;
}
void Create_New_Account::SetBalance(double Bal) { Balance = Bal; }
void Create_New_Account::SetAccountStatus(const std::string& status) { AccountStatus = status; }

// Getters
std::string Create_New_Account::GetFullName() const { return FullName; }
std::string Create_New_Account::GetFullAddress() const { return FullAddress; }
std::string Create_New_Account::GetNationalId() const { return NationalId; }
short Create_New_Account::GetAge() const { return Age; }
std::string Create_New_Account::GetGuardian() const { return Guardian; }
std::string Create_New_Account::GetGuardianNationalId() const { return GuardianNationalId; }
double Create_New_Account::GetBalance() const { return Balance; }
std::string Create_New_Account::GetAccountStatus() const { return AccountStatus; }
int Create_New_Account::GetBankAccountId() const { return BankAccountId; }
std::string Create_New_Account::GetPassword() const { return Password; }

// Static method for inserting data
bool Create_New_Account::executeInsert(SQLHANDLE SQLStatementHandle, const std::string& name, const std::string& address,
	const std::string& NId, short age, const std::string& Guard, const std::string& GNId,
	double bal, int BankId, const std::string& pass, const std::string& Status) 
{
	// Implement insertion logic
	SQLCHAR SQLQuery[512]; // Increased the size to accommodate the full query.
	Admin admin;
	string AdminUser, AdminPass;
	snprintf((char*)SQLQuery, sizeof(SQLQuery),
		"INSERT INTO Bank.accounts ([Full Name], [Full Address], [National ID], [Age], [Guardian], [Guardian National ID], [Balance], [Bank Account ID], [Password], [Account Status]) "
		"VALUES ('%s', '%s', '%s', %d, '%s', '%s', %lf, %d, '%s', '%s')",
		name.c_str(), address.c_str(), NId.c_str(), age, Guard.c_str(), GNId.c_str(), bal, BankId, pass.c_str(), Status.c_str());


	if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, SQLQuery, SQL_NTS))
	{
		showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		return false;
	}
	else
	{
		admin.ADMIN_PAGE(AdminUser, AdminPass);
	}
	return true;
}

// Other methods
void Create_New_Account::CreateNewAccount() 
{
	std::string fullName, fullAddress, nationalId, guardianNationalId, guardian;
	short age;
	double balance;
	std::cout << "Enter Full Name: ";
	cin.ignore(); // to clear the newline character left in the buffer by previous input
	std::getline(std::cin, fullName);
	SetFullName(fullName);

	std::cout << "Enter Full Address: ";
	std::getline(std::cin, fullAddress);
	SetFullAddress(fullAddress);

	std::cout << "Enter Age: ";
	std::cin >> age;
	SetAge(age);

	std::cout << "Enter National ID: ";
	std::cin >> nationalId;
	SetNationalId(nationalId);

	if (age < 21) {
		std::cout << "Enter Guardian: ";
		cin.ignore(); // to clear the newline character left in the buffer by previous input
		getline(cin, guardian); // use getline to handle multi-word input
		SetGuardian(guardian);
		std::cout << "Enter Guardian National ID: ";
		std::cin >> guardianNationalId;
		SetGuardianNationalId(guardianNationalId);
	}

	std::cout << "Enter Balance: ";
	std::cin >> balance;
	SetBalance(balance);

	std::cout << "Create the account.....";
	Sleep(2000); // Delay for 2000 milliseconds (2 seconds)
	
}

unsigned int Create_New_Account::GenerateBankAccountId() {
	// Create a random number generator with a seed based on the current time
	std::random_device rd;
	std::mt19937 gen(rd());

	// Define a uniform distribution for 10-digit numbers
	std::uniform_int_distribution<unsigned int> dis(1000000000, 9999999999);

	// Generate a random number in the range [1000000000, 9999999999]
	unsigned int BankId = dis(gen);

	BankAccountId = BankId;
	return BankId;
}

std::string Create_New_Account::GeneratePassword(int length) 
{
	const std::string characters = "abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789"
		"!@#$%^&*()_-+=<>?";

	srand(static_cast<unsigned int>(time(0)));

	std::string password;
	password.reserve(length); // Reserve memory for the string

	for (int i = 0; i < length; ++i) {
		password += characters[rand() % characters.length()];
	}
	Password = password;
	return password;
}









