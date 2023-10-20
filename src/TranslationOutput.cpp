#include "../include/TranslationOutput.h"

TranslationOutput::TranslationOutput(std::string to, std::string from, std::string inputText, std::string outputText){
  this->to = to;
  this->from = from;
  this->inputText = inputText;
  this->outputText = outputText;
}
TranslationOutput::TranslationOutput(){}
