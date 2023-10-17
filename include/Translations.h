#include <nlohmann/json.hpp>
#include "TranslationOutput.h"

class Translations{

private:
  std::vector<TranslationOutput> translationData;

public:
  Translations();
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Translations, translationData)

};