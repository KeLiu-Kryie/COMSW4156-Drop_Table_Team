#include <nlohmann/json.hpp>

class TranslationOutput{

private:
  std::string to;
  std::string from;
  std::string inputText;
  std::string outputText;

public:
  TranslationOutput(std::string to, std::string from, std::string inputText, std::string outputText);
  TranslationOutput();
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(TranslationOutput, to, from, inputText, outputText)
};