#include "include/config.h"

ConfigSingleton &ConfigSingleton::getInst() {
  static ConfigSingleton INSTANCE;
  return INSTANCE;
}
