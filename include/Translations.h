#ifndef TRANSLATIONS_H
#define TRANSLATIONS_H
#include <nlohmann/json.hpp>
#include "TranslationOutput.h"

class Translations{

private:
  std::vector<TranslationOutput> translationData;

public:
  Translations();
  void AddTranslation(TranslationOutput newTranslation);
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Translations, translationData)

};

#endif