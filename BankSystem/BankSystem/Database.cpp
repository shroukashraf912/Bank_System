#include <iostream>

#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

using namespace std;

#include "Database.h"
#include "Admin.h"
#include "Client.h"


// Define static members
SQLHANDLE Database::SQLEnvHandle = NULL;
SQLHANDLE Database::SQLConnectionHandle = NULL;
SQLHANDLE Database::SQLStatementHandle = NULL;
SQLRETURN Database::retCode = 0;

void Database::showSQLError(unsigned int handleType, const SQLHANDLE& handle)
{
    SQLCHAR SQLState[1024];
    SQLCHAR Message[1024];
    if (SQL_SUCCESS == SQLGetDiagRec(handleType, handle, 1, SQLState, NULL, Message, 1024, NULL))
        cout << "SQL driver message" << Message << "\nSQL state: " << SQLState << "." << endl;

}

void Database::PrintSQLError(SQLHANDLE handle, SQLSMALLINT handleType)
{
    SQLCHAR sqlState[1024];
    SQLCHAR message[1024];
    SQLINTEGER nativeError;
    SQLSMALLINT messageLength;

    if (SQLGetDiagRec(handleType, handle, 1, sqlState, &nativeError, message, sizeof(message), &messageLength) == SQL_SUCCESS) {
        std::cerr << "SQL State: " << sqlState << std::endl;
        std::cerr << "Native Error Code: " << nativeError << std::endl;
        std::cerr << "Message: " << message << std::endl;
    }
    else {
        std::cerr << "Failed to retrieve SQL error information." << std::endl;
    }
}

bool Database::FetchAccountData(const std::string& BankId) {
    // SQL query to fetch the account data based on Bank Account ID
    std::string query = "SELECT [Full Name], [Full Address], [National ID], [Age], [Guardian], [Guardian National ID], [Balance], [Bank Account ID], [Password], [Account Status] FROM Bank.accounts WHERE [Bank Account ID] = ?";

    // Prepare the SQL statement
    if (SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle) == SQL_SUCCESS) {

        // Bind the BankId to the query
        SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, BankId.length(), 0, (SQLPOINTER)BankId.c_str(), 0, NULL);

        // Execute the query
        if (SQLExecDirect(SQLStatementHandle, (SQLCHAR*)query.c_str(), SQL_NTS) == SQL_SUCCESS) {
            SQLCHAR FullName[50], FullAddress[100], NationalId[15], Guardian[50], GuardianNationalId[15], Password[50], AccountStatus[20];
            SQLCHAR BankAccountId[11];
            SQLCHAR Age[4];
            SQLDOUBLE Balance;

            // Fetch the data from the result set
            while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS)
            {
                SQLGetData(SQLStatementHandle, 1, SQL_C_CHAR, FullName, sizeof(FullName), NULL);
                SQLGetData(SQLStatementHandle, 2, SQL_C_CHAR, FullAddress, sizeof(FullAddress), NULL);
                SQLGetData(SQLStatementHandle, 3, SQL_C_CHAR, NationalId, sizeof(NationalId), NULL);
                SQLGetData(SQLStatementHandle, 4, SQL_C_CHAR, Age, sizeof(Age), NULL);
                SQLGetData(SQLStatementHandle, 5, SQL_C_CHAR, Guardian, sizeof(Guardian), NULL);
                SQLGetData(SQLStatementHandle, 6, SQL_C_CHAR, GuardianNationalId, sizeof(GuardianNationalId), NULL);
                SQLGetData(SQLStatementHandle, 7, SQL_C_DOUBLE, &Balance, 0, NULL);
                SQLGetData(SQLStatementHandle, 8, SQL_C_CHAR, BankAccountId, sizeof(BankAccountId), NULL);
                SQLGetData(SQLStatementHandle, 9, SQL_C_CHAR, Password, sizeof(Password), NULL);
                SQLGetData(SQLStatementHandle, 10, SQL_C_CHAR, AccountStatus, sizeof(AccountStatus), NULL);


                // Now you can store or print the fetched data
                std::cout << "Full Name: " << FullName << std::endl;
                std::cout << "Full Address: " << FullAddress << std::endl;
                std::cout << "National ID: " << NationalId << std::endl;
                std::cout << "Age: " << Age << std::endl;
                std::cout << "Guardian: " << Guardian << std::endl;
                std::cout << "Guardian National ID: " << GuardianNationalId << std::endl;
                std::cout << "Balance: " << Balance << std::endl;
                std::cout << "Bank Account ID: " << BankAccountId << std::endl;
                std::cout << "Password: " << Password << std::endl;
                std::cout << "Account Status: " << AccountStatus << std::endl;
            }
            return true;
        }
    }
    return false; // If query execution fails or no data is found
}

void Database::main_menu(void)
{
    Admin ADMIN;
    Client USER;
    char choiceOfWindow;
    string AdminUser, AdminPass;
    string ClientId, ClientPass;
    system("cls");
    std::cout << "\n\t\t\t\t\t ******    Bank System   ******" << std::endl;
    cout << "\n\t\t\t\t\t\xB2\xB2\xB2 Welcome To The Main Menu \xB2\xB2\xB2" << endl;
    cout << "\n\nplease choose the window :" << endl;
    cout << "1- Admin." << endl;
    cout << "2- User." << endl;
    cout << "\n\nEnter your choice :\t";
    cin >> choiceOfWindow;
    switch (choiceOfWindow)
    {
        case '1':
            ADMIN.ADMIN_PAGE(AdminUser, AdminPass);
            break;
        case '2':
            USER.CLIENT_PAGE(ClientId, ClientPass);
            break;
        default:
            break;
    }
}
