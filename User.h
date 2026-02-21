#ifndef USER_H
#define USER_H

#include <string>

enum class UserLevel { NONE = 0, LEVEL1 = 1, LEVEL2 = 2 };

class User {
private:
    std::string code_;
    UserLevel level_;

public:
    User(); // niveau 0
    User(std::string code, UserLevel level);

    const std::string& getCode() const;
    UserLevel getLevel() const;





    bool canConsult() const;
    bool canManage() const;
    bool canProcess() const;
};

#endif
