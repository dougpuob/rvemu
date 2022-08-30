#pragma once
#include <cstdint>
#include <string>

struct SymbolData {
  std::string Name;
  uint32_t Start = 0;
  uint32_t Size = 0;
  int Index = 0;
  uint32_t Offset = 0;
};

enum class FileFlags {
  O_RDONLY = 0,
  O_WRONLY = 1,
  O_RDWR = 2,
  O_ACCMODE = 3,
};