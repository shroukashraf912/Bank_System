#pragma once

#include <string>
#include "Database.h"

class Create_New_Account : public Database {
private:
    std::string FullName;
    std::string FullAddress;
    std::string NationalId;
    short Age;
    std::string Guardian;
    std::string GuardianNationalId;
    double Balance;
    std::string AccountStatus; // Active, Restricted, Closed
    unsigned int BankAccountId;
    std::string Password;

    unsigned int GenerateBankAccountId();
    std::string GeneratePassword(int length);

public:
    // Constructor
    Create_New_Account();

    // Setters
    void SetFullName(const std::string& name);
    void SetFullAddress(const std::string& address);
    void SetNationalId(const std::string& NID);
    void SetAge(short AGE);
    void SetGuardian(const std::string& Guard);
    void SetGuardianNationalId(const std::string& GNID);
    void SetBalance(double Bal);
    void SetAccountStatus(const std::string& status);

    // Getters
    std::string GetFullName() const;
    std::string GetFullAddress() const;
    std::string GetNationalId() const;
    short GetAge() const;
    std::string GetGuardian() const;
    std::string GetGuardianNationalId() const;
    double GetBalance() const;
    std::string GetAccountStatus() const;
    int GetBankAccountId() const;
    std::string GetPassword() const;

    // Static method for inserting data
    static bool executeInsert(SQLHANDLE SQLStatementHandle, const std::string& name, const std::string& address,
        const std::string& NId, short age, const std::string& Guard, const std::string& GNId,
        double bal, int BankId, const std::string& pass, const std::string& Status);

    // Other methods
    void CreateNewAccount();
    void SaveToDatabase(Database& db);


    bool FetchAccountData(const std::string& BankId);
};



