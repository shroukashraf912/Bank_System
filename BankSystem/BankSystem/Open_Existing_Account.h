#pragma once
#include "Database.h"
class Open_Existing_Account :
    public Database
{
    public:
        static void MakeTransaction(const std::string& sourceBankId, const std::string& destBankId, double amount);
        static void UpdateAccountStatus(const std::string& BankId, const std::string& newStatus);
        static void GetCashFromAccount(const std::string& BankId, double amount);
        static void DepositToAccount(const std::string& BankId, double amount);

};

