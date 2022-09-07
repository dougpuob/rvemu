#pragma once

//#include "common.h"
#include "elf.h"
#include "riscv_define.h"

#include <cstdarg>
#include <string.h>
#include <string>
#include <vector>

namespace rvemu {

enum class OpResult { Failed = 0, InvalidBypass, InvalidArgument, Executed };

struct RecordInst {
  RecordInst() = default;
  RecordInst(uint32_t Pc, uint32_t Inst, InstLen Len, const char *Name)
      : Pc(Pc), Inst(Inst), Len(Len), Name(Name) {}

  void AddLog(std::string Log) { Logs.push_back(Log); }
  int AddLog(const char *Fmt, ...) {
    char szBuf[128];
    memset(szBuf, 0, sizeof(szBuf));

    std::va_list Args;
    va_start(Args, Fmt);
    int Ret = vsnprintf(szBuf, sizeof(szBuf), Fmt, Args);
    va_end(Args);

    Logs.push_back(szBuf);
    return Ret;
  }

  uint32_t Pc = 0;
  uint32_t Inst = 0;
  InstLen Len = InstLen::INST_UNKNOWN;
  std::string Name;
  SymbolData ElfSym;
  std::vector<std::string> Logs;
  OpResult Result = OpResult::Failed;
};

} // namespace rvemu