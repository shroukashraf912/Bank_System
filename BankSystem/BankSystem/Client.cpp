#include <iostream>

#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <string>
#include <stdexcept>
#include <algorithm>
using namespace std;
#include "Open_Existing_Account.h"
#include "Client.h"

// Function to verify the old password
bool Client::VerifyOldPassword(const std::string& bankId, const std::string& oldPassword)
{
    SQLHANDLE State_Handle = Database::SQLStatementHandle;
    SQLRETURN retcode;
    SQLCHAR dbPassword[256];
    SQLLEN indicator;

    // Query to check if the password is correct for the given user
    std::string query_pass = "SELECT [Password] FROM Bank.accounts WHERE [Bank Account ID] = '" + bankId + "' AND [Password] = '" + oldPassword + "'";

    // Execute the query
    retcode = SQLExecDirect(State_Handle, (SQLCHAR*)query_pass.c_str(), SQL_NTS);

    if (SQL_SUCCESS != retcode) {
        Database::showSQLError(SQL_HANDLE_STMT, State_Handle);
        return false; // Return false if the query fails
    }

    // Bind the column in the result set to a buffer (assuming Password is of type VARCHAR)
    retcode = SQLBindCol(State_Handle, 1, SQL_C_CHAR, dbPassword, sizeof(dbPassword), &indicator);

    // Fetch the result
    retcode = SQLFetch(State_Handle);

    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        OldPasswordAccount = oldPassword;
        SQLFreeStmt(State_Handle, SQL_CLOSE);
        SQLCloseCursor(State_Handle);
        return true;
    }
    else {
        // If no rows are returned, the password is incorrect
        std::cerr << "Incorrect password. Please try again." << std::endl;
        SQLFreeStmt(State_Handle, SQL_CLOSE);
        SQLCloseCursor(State_Handle);
        return false;
    }
}
// Mock function to update the password in the database
void Client::UpdatePasswordInDatabase(const std::string& bankId, const std::string& newPassword) {
    // In real implementation, execute an SQL update statement
    bool CorrectPass = VerifyOldPassword(bankId, OldPasswordAccount);
    if (CorrectPass)
    {
        // If a row is returned, the password is correct
        std::cout << "Password verified successfully." << std::endl;
        // If the old password is correct, proceed with updating the password
        std::string updateQuery = "UPDATE Bank.accounts SET [Password] = '" + newPassword + "' WHERE [Bank Account ID] = '" + bankId + "'";

        SQLHANDLE State_Handle = Database::SQLStatementHandle;
        SQLRETURN retcode;

        // Execute the update query
        retcode = SQLExecDirect(State_Handle, (SQLCHAR*)updateQuery.c_str(), SQL_NTS);

        if (SQL_SUCCESS != retcode) {
            // Handle errors if the update fails
            Database::showSQLError(SQL_HANDLE_STMT, State_Handle);
            std::cerr << "Failed to update the password for account ID: " << bankId << std::endl;
        }
        else {
            // If the update is successful, notify the user
            std::cout << "Password updated successfully for account ID: " << bankId << std::endl;
        }

        // Clean up
        SQLFreeStmt(State_Handle, SQL_CLOSE);
        SQLCloseCursor(State_Handle);
    }
    else {
        // If the old password is incorrect, notify the user
        std::cerr << "Old password is incorrect. Password update failed." << std::endl;
    }
}

void Client::BankId_Query(std::string id)
{
    SQLHANDLE State_Handle = Database::SQLStatementHandle;
    string bankQuery = "SELECT [Bank Account ID] FROM Bank.accounts WHERE [Bank Account ID] = '" + id + "'";

    // Execute the query
    if (SQL_SUCCESS != SQLExecDirect(State_Handle, (SQLCHAR*)bankQuery.c_str(), SQL_NTS)) {
        Database::showSQLError(SQL_HANDLE_STMT, State_Handle);
    }
    else {
        if (SQLFetch(State_Handle) == SQL_SUCCESS) {
            // User found
            //std::cout << "User " << user << " found in admins table." << std::endl;
            IdClient = id;
            id_found = TRUE;
            //std::cout << userAdmin << std::endl;
        }
        else {
            // User not found
            //std::cout << "User " << user << " not found." << std::endl;
            id_found = FALSE;
        }
        // Close the cursor after fetching the data
        SQLCloseCursor(State_Handle);
    }
}

string Client::Get_BankID(void)
{
    return IdClient;
}

void Client::Enter_password(std::string pass)
{
    // Ensure that a user has already been set before checking the password
    if (IdClient.empty()) {
        std::cout << "No user has been set. Please set the Bank Account ID first." << std::endl;
        return;
    }

    SQLHANDLE State_Handle = Database::SQLStatementHandle;
    SQLFreeStmt(State_Handle, SQL_CLOSE);

    // Query to check if the password is correct for the given user
    std::string query_pass = "SELECT Password FROM Bank.accounts WHERE [Bank Account ID] = '" + IdClient + "' AND Password = '" + pass + "'";

    // Execute the query
    if (SQL_SUCCESS != SQLExecDirect(State_Handle, (SQLCHAR*)query_pass.c_str(), SQL_NTS)) {
        Database::showSQLError(SQL_HANDLE_STMT, State_Handle);
    }
    else {
        if (SQLFetch(State_Handle) == SQL_SUCCESS) {
            // Password is correct
            //std::cout << "Password is correct for user " << userAdmin << "." << std::endl;
            clientPassword = pass;
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

string Client::Get_password(void)
{
    return clientPassword;
}

void Client::CLIENT_PAGE(std::string bankId,std::string pass)
{
    int counter = 0;
    char choiceOfClient;
    double DepositeAmount, getcash, AmountTransfer;
    std::string OutPage;
    std::string NEWSTATUS, destBankId;
    std::string oldPassword, newPassword, confirmPassword;
    system("cls");
    std::cout << "\n\t\t\t\t\t ****** IMT Bank System ******" << std::endl;
    std::cout << "\n\t\t\t\t\t\xB2\xB2\xB2 Welcome To Client Window \xB2\xB2\xB2" << std::endl;
    do
    {
        std::cout << "\nBank Account ID:\t";
        std::cin >> bankId;

        try
        {
            // Call the function to check the username in the database
            BankId_Query(bankId);

            if (!id_found)
            {
                counter++;
                std::cout << "Bank Account ID " << bankId << " not found. Try again." << std::endl;
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
    } while (!id_found);

    // Loop to validate password
    do
    {
        if (id_found)
        {

            std::cout << "\nPassword:\t";
            std::cin >> pass;

            try
            {
                // Call the function to check the password in the database
                Enter_password(pass);

                if (!correct_pass)
                {
                    std::cout << "Password is incorrect for ID " << bankId << ". Try again." << std::endl;
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


    if (id_found && correct_pass)
    {
        system("cls");
        std::cout << "\n\t\t\t\t\t *********** IMT Bank System ***********" << std::endl;
        std::cout << "\n\t\t\t\t\t\t\xB2\xB2\xB2  Welcome "  << " \t\xB2\xB2\xB2" << std::endl;
        std::cout << "\n\nplease choose the window :" << std::endl;
        std::cout << "1- Make Transaction." << std::endl;
        std::cout << "2- Change Account Passsword." << std::endl;
        std::cout << "3- Get Cash." << std::endl;
        std::cout << "4- Deposit in Account." << std::endl;
        std::cout << "5- Return to main Menu." << std::endl;
        std::cout << "\n\nEnter your choice :\t";
        std::cin >> choiceOfClient;
        switch (choiceOfClient)
        {
            case '1':
                system("cls");
                std::cout << "\n\t\t\t\t\t *********** IMT Bank System ***********" << std::endl;
                std::cout << "\n\t\t\t\t\t\t\xB2\xB2\xB2  Welcome  \t\xB2\xB2\xB2" << std::endl;
                std::cout << "Enter the Bank Account ID you want to transfer money to: ";
                std::cin.ignore(); // to clear the newline character left in the buffer by previous input
                std::getline(std::cin, destBankId);
                std::cout << "Enter amount of money to transfer it to this account: ";
                std::cin >> AmountTransfer;
                std::cout << "\n" << std::endl;
                Open_Existing_Account::MakeTransaction(bankId, destBankId, AmountTransfer);
                std::cout << "\n" << std::endl;
                std::cout << "**ACCOUNTS**" << std::endl;
                FetchAccountData(bankId);
                std::cout << "Do you want back to client window? (TRUE or FALSE): ";
                std::cin.ignore(); // to clear the newline character left in the buffer by previous input
                std::getline(std::cin, OutPage);

                // Convert OutPage to uppercase for comparison
                std::transform(OutPage.begin(), OutPage.end(), OutPage.begin(), ::toupper);

                if (OutPage == "TRUE")
                {
                    // Reset statement handle parameters before calling another function
                    SQLFreeStmt(SQLStatementHandle, SQL_CLOSE);
                    SQLFreeStmt(SQLStatementHandle, SQL_RESET_PARAMS);
                    SQLCloseCursor(SQLStatementHandle);
                    CLIENT_PAGE(bankId, pass);
                }
                else
                {
                    // Reset statement handle parameters before calling another function
                    SQLFreeStmt(SQLStatementHandle, SQL_CLOSE);
                    SQLFreeStmt(SQLStatementHandle, SQL_RESET_PARAMS);
                    SQLCloseCursor(SQLStatementHandle);
                    // Exit the program
                    main_menu();
                }
                break;
            case '2':
                system("cls");
                std::cout << "\n\t\t\t\t\t *********** IMT Bank System ***********" << std::endl;
                std::cout << "\n\t\t\t\t\t\xB2\xB2\xB2  Welcome    \xB2\xB2\xB2" << std::endl;
                std::cout << "Enter your old password: ";
                std::cin.ignore(); // Clear the buffer
                std::getline(std::cin, oldPassword);

                // Verify the old password
                if (!VerifyOldPassword(bankId, oldPassword)) {
                    std::cerr << "The old password is incorrect. Please try again." << std::endl;
                    return;
                }

                std::cout << "Enter your new password: ";
                std::getline(std::cin, newPassword);

                std::cout << "Confirm your new password: ";
                std::getline(std::cin, confirmPassword);

                // Check if new password matches the confirmation
                if (newPassword != confirmPassword) {
                    std::cerr << "Passwords do not match. Please try again." << std::endl;
                    return;
                }
                // Update the password in the database
                try {
                    UpdatePasswordInDatabase(bankId, newPassword);
                    std::cout << "Password has been successfully changed!" << std::endl;
                }
                catch (const std::runtime_error& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
                std::cout << "\n" << std::endl;
                std::cout << "**ACCOUNT**" << std::endl;
                FetchAccountData(bankId);
                std::cout << "Do you want back to client window? (TRUE or FALSE): ";
                std::getline(std::cin, OutPage);

                // Convert OutPage to uppercase for comparison
                std::transform(OutPage.begin(), OutPage.end(), OutPage.begin(), ::toupper);

                if (OutPage == "TRUE")
                {
                    // Reset statement handle parameters before calling another function
                    SQLFreeStmt(SQLStatementHandle, SQL_CLOSE);
                    SQLFreeStmt(SQLStatementHandle, SQL_RESET_PARAMS);
                    SQLCloseCursor(SQLStatementHandle);
                    CLIENT_PAGE(bankId, pass);
                }
                else
                {
                    // Reset statement handle parameters before calling another function
                    SQLFreeStmt(SQLStatementHandle, SQL_CLOSE);
                    SQLFreeStmt(SQLStatementHandle, SQL_RESET_PARAMS);
                    SQLCloseCursor(SQLStatementHandle);
                    // Exit the program
                    main_menu();
                }
                break;
            case '3':
                system("cls");
                std::cout << "\n\t\t\t\t\t *********** IMT Bank System ***********" << std::endl;
                std::cout << "\n\t\t\t\t\t\xB2\xB2\xB2  Welcome   \xB2\xB2\xB2" << std::endl;
                std::cout << "Enter the cash you want: ";
                std::cin.ignore(); // to clear the newline character left in the buffer by previous input
                std::cin >> getcash;
                std::cout << "\n" << std::endl;
                Open_Existing_Account::GetCashFromAccount(bankId, getcash);
                std::cout << "\n" << std::endl;
                std::cout << "**ACCOUNT**" << std::endl;
                FetchAccountData(bankId);
                std::cout << "Do you want back to client window? (TRUE or FALSE): ";
                std::cin.ignore(); // to clear the newline character left in the buffer by previous input
                std::getline(std::cin, OutPage);

                // Convert OutPage to uppercase for comparison
                std::transform(OutPage.begin(), OutPage.end(), OutPage.begin(), ::toupper);

                if (OutPage == "TRUE")
                {
                    // Reset statement handle parameters before calling another function
                    SQLFreeStmt(SQLStatementHandle, SQL_CLOSE);
                    SQLFreeStmt(SQLStatementHandle, SQL_RESET_PARAMS);
                    SQLCloseCursor(SQLStatementHandle);
                    CLIENT_PAGE(bankId, pass);
                }
                else
                {
                    // Reset statement handle parameters before calling another function
                    SQLFreeStmt(SQLStatementHandle, SQL_CLOSE);
                    SQLFreeStmt(SQLStatementHandle, SQL_RESET_PARAMS);
                    SQLCloseCursor(SQLStatementHandle);
                    // Exit the program
                    main_menu();
                }
                break;
            case '4':
                system("cls");
                std::cout << "\n\t\t\t\t\t *********** IMT Bank System ***********" << std::endl;
                std::cout << "\n\t\t\t\t\t\xB2\xB2\xB2  Welcome    \xB2\xB2\xB2" << std::endl;
                std::cout << "Enter amount you want to deposit: ";
                std::cin.ignore(); // to clear the newline character left in the buffer by previous input
                std::cin >> DepositeAmount;
                Open_Existing_Account::DepositToAccount(bankId, DepositeAmount);
                std::cout << "\n" << std::endl;
                std::cout << "**ACCOUNT**" << std::endl;
                FetchAccountData(bankId);
                std::cout << "Do you want back to client window? (TRUE or FALSE): ";
                std::cin.ignore(); // to clear the newline character left in the buffer by previous input
                std::getline(std::cin, OutPage);

                // Convert OutPage to uppercase for comparison
                std::transform(OutPage.begin(), OutPage.end(), OutPage.begin(), ::toupper);

                if (OutPage == "TRUE")
                {
                    // Reset statement handle parameters before calling another function
                    SQLFreeStmt(SQLStatementHandle, SQL_CLOSE);
                    SQLFreeStmt(SQLStatementHandle, SQL_RESET_PARAMS);
                    SQLCloseCursor(SQLStatementHandle);
                    CLIENT_PAGE(bankId, pass);
                }
                else
                {
                    // Reset statement handle parameters before calling another function
                    SQLFreeStmt(SQLStatementHandle, SQL_CLOSE);
                    SQLFreeStmt(SQLStatementHandle, SQL_RESET_PARAMS);
                    SQLCloseCursor(SQLStatementHandle);
                    // Exit the program
                    main_menu();
                }
                break;
            case '5':
                main_menu();
                break;
        }
    }

}

