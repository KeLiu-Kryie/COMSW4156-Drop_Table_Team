#ifndef INCLUDE_TRANSLATIONS_H_
#define INCLUDE_TRANSLATIONS_H_

#include "TranslationOutput.h"
#include <vector>
#include <nlohmann/json.hpp>

class Translations{
 private:
  std::vector<TranslationOutput> translationData;

 public:
  Translations();
  void AddTranslation(TranslationOutput newTranslation);
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Translations, translationData)
};

#endif  // INCLUDE_TRANSLATIONS_H_
