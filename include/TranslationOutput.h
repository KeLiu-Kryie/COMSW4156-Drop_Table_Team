#ifndef INCLUDE_TRANSLATIONOUTPUT_H_
#define INCLUDE_TRANSLATIONOUTPUT_H_
#include <string>
#include <nlohmann/json.hpp>

class TranslationOutput
{
 public:
  TranslationOutput(std::string to, std::string from, std::string inputText, std::string outputText);
  TranslationOutput();
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(TranslationOutput, to, from, inputText, outputText)

 private:
  std::string to;
  std::string from;
  std::string inputText;
  std::string outputText;
};

#endif  // INCLUDE_TRANSLATIONOUTPUT_H_
