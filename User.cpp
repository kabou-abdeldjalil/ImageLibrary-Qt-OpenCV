#include "User.h"


User::User() : code_(""), level_(UserLevel::NONE) {}

User::User(std::string code, UserLevel level)
    : code_(std::move(code)), level_(level) {}

const std::string& User::getCode() const { return code_; }
UserLevel User::getLevel() const { return level_; }


