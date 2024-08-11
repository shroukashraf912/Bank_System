#pragma once
class Database
{
    public:
        static SQLHANDLE SQLEnvHandle;
        static SQLHANDLE SQLConnectionHandle ;
        static SQLHANDLE SQLStatementHandle;
        static SQLRETURN retCode;

	   static void showSQLError(unsigned int handleType, const SQLHANDLE& handle);
       static void PrintSQLError(SQLHANDLE handle, SQLSMALLINT handleType);
       static void main_menu(void);
       bool FetchAccountData(const std::string& BankId);
};

