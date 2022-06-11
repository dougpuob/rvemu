#include <string>

class ConfigSingleton
{
private:
    // Disallow creating an instance of this object
    ConfigSingleton() {}

public:
    static ConfigSingleton& getInst();

    bool opt_trace;
    std::string opt_prog_name;
};