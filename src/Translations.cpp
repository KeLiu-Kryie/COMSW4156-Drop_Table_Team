#include "../include/Translations.h"

void Translations::AddTranslation(TranslationOutput newTranslation){
  while (translationData.size() >= 10){
    translationData.erase(translationData.begin());
  }
  translationData.push_back(newTranslation);
}

Translations::Translations(){}
