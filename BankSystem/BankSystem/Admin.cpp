#include <iostream>

#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <string>
#include <stdexcept>
#include <algorithm>
#include "Admin.h"
#include "Create_New_Account.h"
#include "Open_Existing_Account.h"

using namespace std;

void Admin::Enter_username(std::string user)
{
	SQLHANDLE State_Handle = Database::SQLStatementHandle;
	query_user = "SELECT Username FROM Bank.admins WHERE Username = '" + user + "'";

	// Execute the query
	if (SQL_SUCCESS != SQLExecDirect(State_Handle, (SQLCHAR*)query_user.c_str(), SQL_NTS)) {
		Database::showSQLError(SQL_HANDLE_STMT, State_Handle);
	}
	else {
		if (SQLFetch(State_Handle) == SQL_SUCCESS) {
			// User found
			//std::cout << "User " << user << " found in admins table." << std::endl;
			userAdmin = user;
            user_found = TRUE;
			//std::cout << userAdmin << std::endl;
		}
		else {
			// User not found
			//std::cout << "User " << user << " not found." << std::endl;
            user_found = FALSE;
		}
        // Close the cursor after fetching the data
        SQLCloseCursor(State_Handle);
	}
}

std::string Admin::Get_username(void)
{
	return userAdmin;
}

void Admin::Enter_password(std::string pass)
{
    // Ensure that a user has already been set before checking the password
    if (userAdmin.empty()) {
        std::cout << "No user has been set. Please set the username first." << std::endl;
        return;
    }

    SQLHANDLE State_Handle = Database::SQLStatementHandle;
    SQLFreeStmt(State_Handle, SQL_CLOSE);
    
    // Query to check if the password is correct for the given user
    std::string query_pass = "SELECT Password FROM Bank.admins WHERE Username = '" + userAdmin + "' AND Password = '" + pass + "'";

    // Execute the query
    if (SQL_SUCCESS != SQLExecDirect(State_Handle, (SQLCHAR*)query_pass.c_str(), SQL_NTS)) {
        Database::showSQLError(SQL_HANDLE_STMT, State_Handle);
    }
    else {
        if (SQLFetch(State_Handle) == SQL_SUCCESS) {
            // Password is correct
            //std::cout << "Password is correct for user " << userAdmin << "." << std::endl;
            adminPassword = pass;
            correct_pass = TRUE;
        }
        else {
            // Password is incorrect
            //std::cout << "Password is incorrect for user " << userAdmin << "." << std::endl;
            correct_pass = FALSE;
        }
        // Close the cursor after fetching the data
        SQLCloseCursor(State_Handle);
    }
}

std::string Admin::Get_password(void)
{
	return adminPassword;
}

void Admin::ADMIN_PAGE(std::string user, std::string pass)
{
    int counter = 0;
    char choiceOfAdmin;
    char choiceOfExisting;
    char SQLQuery_accounts[] = "SELECT * FROM Bank.accounts";
    Create_New_Account account;
    std::string fullName, fullAddress, nationalId, guardianNationalId, guardian, AccountStatus, AccountPass;
    short age;
    double balance;
    int BankID;
    double DepositeAmount, getcash, AmountTransfer;
    std::string OutPage;
    std::string BankId; // Example BankId you want to fetch data for
    std::string NEWSTATUS, destBankId;
    system("cls");
    std::cout << "\n\t\t\t\t\t ****** IMT Bank System ******" << std::endl;
    std::cout << "\n\t\t\t\t\t\xB2\xB2\xB2 Welcome To Admin Window \xB2\xB2\xB2" << std::endl;
    // Loop to find user
    do
    {
        std::cout << "\nUsername:\t";
        std::cin >> user;

        try
        {
            // Call the function to check the username in the database
            Enter_username(user);

            if (!user_found)
            {
                counter++;
                std::cout << "User " << user << " not found. Try again." << std::endl;
                if (4 == counter)
                {
                    std::cout << "Please contact the system administrator." << std::endl;
                    counter = 0;
                    break;
                }
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "SQL error: " << e.what() << "\n";
            std::cerr << "Please contact the system administrator.\n";
            return;
        }
    } while (!user_found);

    // Loop to validate password
    do
    {
        if (user_found)
        {

            std::cout << "\nPassword:\t";
            std::cin >> pass;

            try
            {
                // Call the function to check the password in the database
                Enter_password(pass);

                if (!correct_pass)
                {
                    std::cout << "Password is incorrect for user " << user << ". Try again." << std::endl;
                    counter++;
                    if (4 == counter)
                    {
                        std::cout << "Please contact the system administrator." << std::endl;
                        counter = 0;
                        break;
                    }
                }
            }
            catch (const std::exception& e)
            {
                std::cerr << "SQL error: " << e.what() << "\n";
                std::cerr << "Please contact the system administrator.\n";
                return;
            }
        }
        else {
            break;
        }
    } while (!correct_pass);

    // If the code reaches here, the user is authenticated

    if (user_found && correct_pass)
    {
        system("cls");
        std::cout << "\n\t\t\t\t\t *********** IMT Bank System ***********" << std::endl;
        std::cout << "\n\t\t\t\t\t\t\xB2\xB2\xB2  Welcome, " << user << " \t\xB2\xB2\xB2" << std::endl;
        std::cout << "\n\nplease choose the window :" << std::endl;
        std::cout << "1- Create New Account." << std::endl;
        std::cout << "2- Open Existing Account." << std::endl;
        std::cout << "3- Exist System." << std::endl;
        std::cout << "\n\nEnter your choice :\t";
        std::cin >> choiceOfAdmin;
        switch (choiceOfAdmin)
        {
            case '1':
                //Create New Account
                system("cls");
                std::cout << "\n\t\t\t\t\t *********** IMT Bank System ***********" << std::endl;
                std::cout << "\n\t\t\t\t\xB2\xB2\xB2  Welcome Create Account Page, " << user << "  \xB2\xB2\xB2" << std::endl;
                account.CreateNewAccount();

                fullName = account.GetFullName();
                fullAddress = account.GetFullAddress();
                nationalId = account.GetNationalId();
                guardian = account.GetGuardian();
                guardianNationalId = account.GetGuardianNationalId();
                age = account.GetAge();
                balance = account.GetBalance();
                AccountStatus = account.GetAccountStatus();
                AccountPass = account.GetPassword();
                BankID = account.GetBankAccountId();

                // Insert data of new accounts in the acouunts table
                if (!Create_New_Account::executeInsert(Database::SQLStatementHandle, fullName, fullAddress, nationalId, age, guardian, guardianNationalId, balance, BankID, AccountPass, AccountStatus))
                {
                    cout << "Failed to insert data" << endl;
                    break;
                }
                if (SQL_SUCCESS != SQLExecDirect(Database::SQLStatementHandle, (SQLCHAR*)SQLQuery_accounts, SQL_NTS))
                {
                    Database::showSQLError(SQL_HANDLE_STMT, Database::SQLStatementHandle);
                }
                break;
            case '2':
                //Open Existing Account
                system("cls");
                std::cout << "\n\t\t\t\t\t *********** IMT Bank System ***********" << std::endl;
                std::cout << "\n\t\t\t\t\xB2\xB2\xB2  Welcome  Existing Account Page, " << user << "  \xB2\xB2\xB2" << std::endl;
                std::cout << "Enter Bank Account ID: ";
                std::cin.ignore(); // to clear the newline character left in the buffer by previous input
                std::getline(std::cin, BankId);
                std::cout << "\n" << std::endl;
                //account data <--- <-----
                std::cout << "**ACCOUNT**" << std::endl;
                FetchAccountData(BankId);
                SQLFreeStmt(SQLStatementHandle, SQL_CLOSE);
                std::cout << "\n" << std::endl;
                std::cout << "Choose between the following features: " << std::endl;
                std::cout << "1- Make Transaction." << std::endl;
                std::cout << "2- Change Account Status." << std::endl;
                std::cout << "3- Get Cash." << std::endl;
                std::cout << "4- Deposit in Account." << std::endl;
                std::cout << "5- Return to main Menu." << std::endl;
                std::cout << "\n\nEnter your choice :\t";
                std::cin >> choiceOfExisting;
                switch (choiceOfExisting)
                {
                    case '1':
                        system("cls");
                        std::cout << "\n\t\t\t\t\t *********** IMT Bank System ***********" << std::endl;
                        std::cout << "\n\t\t\t\t\t\xB2\xB2\xB2  Welcome Existing Account Page, " << user << "  \xB2\xB2\xB2" << std::endl;
                        std::cout << "Enter the Bank Account ID you want to transfer money to: ";
                        std::cin.ignore(); // to clear the newline character left in the buffer by previous input
                        std::getline(std::cin, destBankId);
                        std::cout << "Enter amount of money to transfer it to this account: ";
                        std::cin >> AmountTransfer;
                        std::cout << "\n" << std::endl;
                        Open_Existing_Account::MakeTransaction(BankId, destBankId, AmountTransfer);
                        //Open_Existing_Account::MakeTransaction("1378853914", "1192814015", 500);
                        std::cout << "\n" << std::endl;
                        std::cout << "**ACCOUNTS**" << std::endl;
                        FetchAccountData(BankId);
                        std::cout << "\n" << std::endl;
                        FetchAccountData(destBankId);
                        std::cout << "Do you want back to admin window? (TRUE or FALSE): ";
                        std::cin.ignore(); // to clear the newline character left in the buffer by previous input
                        std::getline(std::cin, OutPage);

                        // Convert OutPage to uppercase for comparison
                        std::transform(OutPage.begin(), OutPage.end(), OutPage.begin(), ::toupper);

                        if (OutPage == "TRUE")
                        {
                            // Reset statement handle parameters before calling another function
                            SQLFreeStmt(SQLStatementHandle, SQL_CLOSE);
                            SQLFreeStmt(SQLStatementHandle, SQL_RESET_PARAMS);
                            ADMIN_PAGE(user, pass);
                        }
                        else
                        {
                            // Reset statement handle parameters before calling another function
                            SQLFreeStmt(SQLStatementHandle, SQL_CLOSE);
                            SQLFreeStmt(SQLStatementHandle, SQL_RESET_PARAMS);
                            // Exit the program
                            main_menu();
                        }
                        break;
                    case '2':
                        system("cls");
                        std::cout << "\n\t\t\t\t\t *********** IMT Bank System ***********" << std::endl;
                        std::cout << "\n\t\t\t\t\t\xB2\xB2\xB2  Welcome Existing Account Page, " << user << "  \xB2\xB2\xB2" << std::endl;
                        std::cout << "Enter the Status of the Account: ";
                        std::cin.ignore(); // to clear the newline character left in the buffer by previous input
                        std::getline(std::cin, NEWSTATUS);
                        std::cout << "\n" << std::endl;
                        Open_Existing_Account::UpdateAccountStatus(BankId,NEWSTATUS);
                        std::cout << "\n" << std::endl;
                        std::cout << "**ACCOUNT**" << std::endl;
                        FetchAccountData(BankId);
                        std::cout << "Do you want back to admin window? (TRUE or FALSE): ";
                        std::cin.ignore(); // to clear the newline character left in the buffer by previous input
                        std::getline(std::cin, OutPage);

                        // Convert OutPage to uppercase for comparison
                        std::transform(OutPage.begin(), OutPage.end(), OutPage.begin(), ::toupper);

                        if (OutPage == "TRUE")
                        {
                            // Reset statement handle parameters before calling another function
                            SQLFreeStmt(SQLStatementHandle, SQL_CLOSE);
                            SQLFreeStmt(SQLStatementHandle, SQL_RESET_PARAMS);
                            ADMIN_PAGE(user, pass);
                        }
                        else
                        {
                            // Reset statement handle parameters before calling another function
                            SQLFreeStmt(SQLStatementHandle, SQL_CLOSE);
                            SQLFreeStmt(SQLStatementHandle, SQL_RESET_PARAMS);
                            // Exit the program
                            main_menu();
                        }
                        break;
                    case '3':
                        system("cls");
                        std::cout << "\n\t\t\t\t\t *********** IMT Bank System ***********" << std::endl;
                        std::cout << "\n\t\t\t\t\t\xB2\xB2\xB2  Welcome Existing Account Page, " << user << "  \xB2\xB2\xB2" << std::endl;
                        std::cout << "Enter the cash you want: ";
                        std::cin.ignore(); // to clear the newline character left in the buffer by previous input
                        std::cin>> getcash;
                        std::cout << "\n" << std::endl;
                        Open_Existing_Account::GetCashFromAccount(BankId, getcash);
                        std::cout << "\n" << std::endl;
                        std::cout << "**ACCOUNT**" << std::endl;
                        FetchAccountData(BankId);
                        std::cout << "Do you want back to admin window? (TRUE or FALSE): ";
                        std::cin.ignore(); // to clear the newline character left in the buffer by previous input
                        std::getline(std::cin, OutPage);

                        // Convert OutPage to uppercase for comparison
                        std::transform(OutPage.begin(), OutPage.end(), OutPage.begin(), ::toupper);

                        if (OutPage == "TRUE")
                        {
                            // Reset statement handle parameters before calling another function
                            SQLFreeStmt(SQLStatementHandle, SQL_CLOSE);
                            SQLFreeStmt(SQLStatementHandle, SQL_RESET_PARAMS);
                            ADMIN_PAGE(user, pass);
                        }
                        else
                        {
                            // Reset statement handle parameters before calling another function
                            SQLFreeStmt(SQLStatementHandle, SQL_CLOSE);
                            SQLFreeStmt(SQLStatementHandle, SQL_RESET_PARAMS);
                            // Exit the program
                            main_menu();
                        }
                        break;
                    case '4':
                        system("cls");
                        std::cout << "\n\t\t\t\t\t *********** IMT Bank System ***********" << std::endl;
                        std::cout << "\n\t\t\t\t\t\xB2\xB2\xB2  Welcome Existing Account Page, " << user << "  \xB2\xB2\xB2" << std::endl;
                        std::cout << "Enter amount you want to deposit: ";
                        std::cin.ignore(); // to clear the newline character left in the buffer by previous input
                        std::cin >> DepositeAmount;
                        Open_Existing_Account::DepositToAccount(BankId, DepositeAmount);
                        std::cout << "\n" << std::endl;
                        std::cout << "**ACCOUNT**" << std::endl;
                        FetchAccountData(BankId);
                        std::cout << "Do you want back to admin window? (TRUE or FALSE): ";
                        std::cin.ignore(); // to clear the newline character left in the buffer by previous input
                        std::getline(std::cin, OutPage);

                        // Convert OutPage to uppercase for comparison
                        std::transform(OutPage.begin(), OutPage.end(), OutPage.begin(), ::toupper);

                        if (OutPage == "TRUE")
                        {
                            // Reset statement handle parameters before calling another function
                            SQLFreeStmt(SQLStatementHandle, SQL_CLOSE);
                            SQLFreeStmt(SQLStatementHandle, SQL_RESET_PARAMS);
                            ADMIN_PAGE(user, pass);
                        }
                        else
                        {
                            // Reset statement handle parameters before calling another function
                            SQLFreeStmt(SQLStatementHandle, SQL_CLOSE);
                            SQLFreeStmt(SQLStatementHandle, SQL_RESET_PARAMS);
                            // Exit the program
                            main_menu();
                        }
                        break;
                    case '5':
                        ADMIN_PAGE(user,pass);
                        break;
                }
                break;
            case '3':
                // Exit the program
                main_menu();
                break;
        }
    }
}

