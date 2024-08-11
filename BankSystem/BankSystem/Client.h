#pragma once
#include "Database.h"
class Client :
    public Database
{
    private:
         bool oldPass;
         string OldPasswordAccount;
         string IdClient;
         string clientPassword;
         bool id_found;
         bool correct_pass;

    public:
        bool VerifyOldPassword(const std::string& bankId, const std::string& oldPassword);
        void UpdatePasswordInDatabase(const std::string& bankId, const std::string& newPassword);
        void BankId_Query(std::string id);
        string Get_BankID(void);
        void  Enter_password(std::string pass);
        string Get_password(void);
        void CLIENT_PAGE( std::string bankId,std::string pass);
};

