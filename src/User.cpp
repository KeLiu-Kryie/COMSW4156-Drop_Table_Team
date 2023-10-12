#include "../include/User.h"


User::User(std::string name, std::string profession){
  this->name = name;
  this->profession = profession;
  this->translations = {TranslationOutput("a", "b", "c", "d")};
}

User::User(){}