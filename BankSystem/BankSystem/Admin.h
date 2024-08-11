#pragma once
#include "Database.h"
class Admin :
    public Database
{
    private:
        std::string userAdmin;
        std::string adminPassword;
        std::string query_user;
        std::string query_pass;
        bool user_found = FALSE;
        bool correct_pass = FALSE;
    public:
        void Enter_username(std::string user);
        std::string Get_username(void);
        void Enter_password(std::string pass);
        std::string Get_password(void);
        void ADMIN_PAGE(std::string user, std::string pass);
};

