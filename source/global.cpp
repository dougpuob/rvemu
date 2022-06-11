#include "include/global.h"

ConfigSingleton& ConfigSingleton::getInst()
{
    static ConfigSingleton INSTANCE;
    return INSTANCE;
}
