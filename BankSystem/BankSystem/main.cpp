/*
*************************************************************************
    Author:          Shrouk Ashraf Abdelfattah Ahmed
    Date of Started: 1-8-2024
    Project:         Bank System by using C++ and SQL Database
    Version:         V.1
*************************************************************************
*/

#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <string>

using namespace std;

#include "Database.h"
#include "Admin.h"
#include "Create_New_Account.h"
#include "Client.h"


// Function to print SQL errors
/*void PrintSQLError(SQLHANDLE handle, SQLSMALLINT handleType) {
    SQLCHAR sqlState[1024];
    SQLCHAR message[1024];
    SQLINTEGER nativeError;
    SQLSMALLINT messageLength;

    if (SQLGetDiagRec(handleType, handle, 1, sqlState, &nativeError, message, sizeof(message), &messageLength) == SQL_SUCCESS) {
        std::cerr << "SQL Error: " << message << std::endl;
    }
}


// Function to execute a SQL query and check its success
bool ExecuteQuery(SQLHANDLE& stmtHandle, const std::string& query) {
    if (SQLExecDirect(stmtHandle, (SQLCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
        std::cerr << "SQL Error: ";
        PrintSQLError(stmtHandle, SQL_HANDLE_STMT);
        return false;
    }
    return true;
}
// Function to print SQL errors
void PrintSQLError(SQLHANDLE handle, SQLSMALLINT handleType) {
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
}*/







int main(void) {
    Database DB;
    do {
        if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &Database::SQLEnvHandle))
            break;

        if (SQL_SUCCESS != SQLSetEnvAttr(Database::SQLEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
            break;

        if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, Database::SQLEnvHandle, &Database::SQLConnectionHandle))
            break;

        if (SQL_SUCCESS != SQLSetConnectAttr(Database::SQLConnectionHandle, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0))
            break;

        SQLCHAR retConString[1024];
        switch (SQLDriverConnect(Database::SQLConnectionHandle, NULL, (SQLCHAR*)"DRIVER={SQL Server}; SERVER=localhost,1433;DATABASE=BankSystem;Trusted=true;", SQL_NTS, retConString, 1024, NULL, SQL_DRIVER_NOPROMPT))
        {
        case SQL_SUCCESS:
            cout << "Success connected" << endl;
            break;
        case SQL_SUCCESS_WITH_INFO:
            cout << "Success connected" << endl;
            break;
        case SQL_NO_DATA_FOUND:
        case SQL_INVALID_HANDLE:
        case SQL_ERROR:
            DB.showSQLError(SQL_HANDLE_DBC, Database::SQLConnectionHandle);
            Database::retCode = -1;
            break;
        default:
            break;
        }
        if (Database::retCode == -1)
            break;

        if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, Database::SQLConnectionHandle, &Database::SQLStatementHandle))
            break;

        Database::main_menu();
    } while (FALSE);

    SQLFreeHandle(SQL_HANDLE_STMT, Database::SQLStatementHandle);
    SQLDisconnect(Database::SQLConnectionHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, Database::SQLConnectionHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, Database::SQLEnvHandle);
}



