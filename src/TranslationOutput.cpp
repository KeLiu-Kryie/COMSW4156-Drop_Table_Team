#include "../include/TranslationOutput.h"

TranslationOutput::TranslationOutput(const std::string& to, const std::string& from, const std::string& inputText, const std::string& outputText){
  this->to = to;
  this->from = from;
  this->inputText = inputText;
  this->outputText = outputText;
}
TranslationOutput::TranslationOutput(){}
