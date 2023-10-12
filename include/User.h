#include <nlohmann/json.hpp>
#include "TranslationOutput.h"

class User{

private:
  std::string name;
  std::string profession;
  std::vector<TranslationOutput> translations;

public:
  User(std::string name, std::string profession);
  User();
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(User, name, profession, translations)

};