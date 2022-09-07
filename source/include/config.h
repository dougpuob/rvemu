#pragma once

#include <string>

class ConfigSingleton {
private:
  // Disallow creating an instance of this object
  ConfigSingleton() {}

public:
  static ConfigSingleton &getInst();

  bool opt_trace = false;
  bool opt_tracelog = false;
  bool opt_unittest = false;
  std::string opt_prog_name = "";
};

using Config = ConfigSingleton;