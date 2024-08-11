#include <iostream>

#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <string>
#include <stdexcept>

using namespace std;
#include "Open_Existing_Account.h"


void Open_Existing_Account::DepositToAccount(const std::string& BankId, double amount)
{
    // Prepare the SQL query to update the account balance
    std::string query = "UPDATE Bank.accounts SET [Balance] = [Balance] + ? WHERE [Bank Account ID] = ?";

    if (SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle) != SQL_SUCCESS) {
        std::cout << "Failed to allocate SQL statement handle." << std::endl;
        return;
    }

    // Bind the parameters to the query
    SQLRETURN retCode;

    // Bind the amount parameter (1st parameter in query)
    retCode = SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &amount, 0, NULL);
    if (retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO) {
        std::cout << "Failed to bind amount parameter." << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
        return;
    }

    // Bind the BankId parameter (2nd parameter in query)
    retCode = SQLBindParameter(SQLStatementHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, BankId.length(), 0, (SQLPOINTER)BankId.c_str(), 0, NULL);
    if (retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO) {
        std::cout << "Failed to bind BankId parameter." << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
        return;
    }

    // Execute the update query
    retCode = SQLExecDirect(SQLStatementHandle, (SQLCHAR*)query.c_str(), SQL_NTS);
    if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {
        std::cout << "Deposit successful. Amount: $" << amount << std::endl;
    }
    else {
        std::cout << "Failed to deposit amount." << std::endl;
        // Print detailed error information
        SQLCHAR sqlState[1024];
        SQLCHAR message[1024];
        SQLINTEGER nativeError;
        SQLSMALLINT messageLength;
        while (SQLGetDiagRec(SQL_HANDLE_STMT, SQLStatementHandle, 1, sqlState, &nativeError, message, sizeof(message), &messageLength) == SQL_SUCCESS) {
            std::cout << "SQLSTATE: " << sqlState << std::endl;
            std::cout << "Message: " << message << std::endl;
            std::cout << "Native Error Code: " << nativeError << std::endl;
        }
    }

    // Free the SQL statement handle
    SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
}

void Open_Existing_Account::UpdateAccountStatus(const std::string& BankId, const std::string& newStatus)
{
    // Prepare the SQL query to update the account status
    std::string query = "UPDATE Bank.accounts SET [Account Status] = ? WHERE [Bank Account ID] = ?";

    // Allocate and prepare SQL statement handle
    
    if (SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle) != SQL_SUCCESS) {
        std::cout << "Failed to allocate SQL statement handle." << std::endl;
        return;
    }

    // Bind the parameters to the query
    SQLRETURN retCode;

    // Bind the newStatus parameter (1st parameter in query)
    retCode = SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, newStatus.length(), 0, (SQLPOINTER)newStatus.c_str(), 0, NULL);
    if (retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO) {
        std::cout << "Failed to bind newStatus parameter." << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
        return;
    }

    // Bind the BankId parameter (2nd parameter in query)
    retCode = SQLBindParameter(SQLStatementHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, BankId.length(), 0, (SQLPOINTER)BankId.c_str(), 0, NULL);
    if (retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO) {
        std::cout << "Failed to bind BankId parameter." << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
        return;
    }

    // Execute the update query
    retCode = SQLExecDirect(SQLStatementHandle, (SQLCHAR*)query.c_str(), SQL_NTS);
    if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {
        std::cout << "Account status updated successfully to: " << newStatus << std::endl;
        
    }
    else {
        std::cout << "Failed to update account status." << std::endl;
        // Print detailed error information
        SQLCHAR sqlState[1024];
        SQLCHAR message[1024];
        SQLINTEGER nativeError;
        SQLSMALLINT messageLength;
        while (SQLGetDiagRec(SQL_HANDLE_STMT, SQLStatementHandle, 1, sqlState, &nativeError, message, sizeof(message), &messageLength) == SQL_SUCCESS) {
            std::cout << "SQLSTATE: " << sqlState << std::endl;
            std::cout << "Message: " << message << std::endl;
            std::cout << "Native Error Code: " << nativeError << std::endl;
        }
    }

    // Free the SQL statement handle
    SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
}

void Open_Existing_Account::GetCashFromAccount(const std::string& BankId, double amount) {
    // First, check if the account has sufficient balance
    double currentBalance = 0.0;
    std::string query = "SELECT [Balance] FROM Bank.accounts WHERE [Bank Account ID] = ?";

    // Allocate and prepare SQL statement handle
    if (SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle) != SQL_SUCCESS) {
        std::cerr << "Failed to allocate SQL statement handle." << std::endl;
        return;
    }

    // Bind parameters to fetch the current balance
    SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, BankId.length(), 0, (SQLPOINTER)BankId.c_str(), 0, NULL);

    // Execute the query to fetch the current balance
    if (SQLExecDirect(SQLStatementHandle, (SQLCHAR*)query.c_str(), SQL_NTS) == SQL_SUCCESS) {
        if (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
            SQLGetData(SQLStatementHandle, 1, SQL_C_DOUBLE, &currentBalance, 0, NULL);
        }
        else {
            std::cerr << "Failed to fetch balance." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
            return;
        }
    }
    else {
        std::cerr << "Failed to execute query for balance check." << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
        return;
    }

    // Check if the account has sufficient balance
    if (amount > currentBalance) {
        std::cerr << "Insufficient balance. Cannot withdraw " << amount << "." << std::endl;
    }
    else {
        // Update the account balance to withdraw the cash
        query = "UPDATE Bank.accounts SET [Balance] = [Balance] - ? WHERE [Bank Account ID] = ?";
        SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);

        if (SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle) != SQL_SUCCESS) {
            std::cerr << "Failed to allocate SQL statement handle for update." << std::endl;
            return;
        }

        // Bind parameters for updating the balance
        SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &amount, 0, NULL);
        SQLBindParameter(SQLStatementHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, BankId.length(), 0, (SQLPOINTER)BankId.c_str(), 0, NULL);

        // Execute the update query
        if (SQLExecDirect(SQLStatementHandle, (SQLCHAR*)query.c_str(), SQL_NTS) == SQL_SUCCESS) {
            std::cout << "Withdrawal successful. Amount: $" << amount << std::endl;
        }
        else {
            std::cerr << "Failed to withdraw amount." << std::endl; \
        }
        SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
    }
}
// Function to make a transaction
void Open_Existing_Account::MakeTransaction(const std::string& sourceBankId, const std::string& destBankId, double amount) 
{
    double sourceBalance = 0.0;

    // Fetch the source account balance
    char query1[] = "SELECT [Balance] FROM Bank.accounts WHERE [Bank Account ID] = ? AND [Account Status] = 'Active'";
    SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, sourceBankId.length(), 0, (SQLPOINTER)sourceBankId.c_str(), 0, NULL);

    if (SQLExecDirect(SQLStatementHandle, (SQLCHAR*)query1, SQL_NTS) == SQL_SUCCESS && SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
        SQLGetData(SQLStatementHandle, 1, SQL_C_DOUBLE, &sourceBalance, 0, NULL);
        std::cout << "Source account balance: $" << sourceBalance << std::endl;
    }
    else {
        std::cerr << "Failed to fetch source account balance." << std::endl;
        PrintSQLError(SQLStatementHandle, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
        return;
    }

    // Close the cursor and free statement handle
    SQLFreeStmt(SQLStatementHandle, SQL_CLOSE);

    // Deduct amount from source account
    char query2[] = "UPDATE Bank.accounts SET [Balance] = [Balance] - ? WHERE [Bank Account ID] = ?";
    SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &amount, 0, NULL);
    SQLBindParameter(SQLStatementHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, sourceBankId.length(), 0, (SQLPOINTER)sourceBankId.c_str(), 0, NULL);

    if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)query2, SQL_NTS)) {
        PrintSQLError(SQLStatementHandle, SQL_HANDLE_STMT);
    }
    else {
        std::cout << "Amount deducted from source account: $" << amount << std::endl;
    }

    // Close the cursor and free statement handle
    SQLFreeStmt(SQLStatementHandle, SQL_CLOSE);

    // Add amount to destination account
    char query3[] = "UPDATE Bank.accounts SET [Balance] = [Balance] + ? WHERE [Bank Account ID] = ?";
    SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &amount, 0, NULL);
    SQLBindParameter(SQLStatementHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, destBankId.length(), 0, (SQLPOINTER)destBankId.c_str(), 0, NULL);

    if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)query3, SQL_NTS)) {
        PrintSQLError(SQLStatementHandle, SQL_HANDLE_STMT);
    }
    else {
        std::cout << "Transaction successful. Amount transferred: $" << amount << std::endl;
    }

    SQLFreeStmt(SQLStatementHandle, SQL_CLOSE);
    SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
}

