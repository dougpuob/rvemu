#pragma once

#include <cstdint>

namespace rvemu {
// clang-format off
  enum {
      //                    1          0
      //               54321098'76543210
      INST16_00_00 = 0b00000000'00000001,
      INST16_01_00 = 0b00000000'00000011,
      INST16_02_00 = 0b00000000'00000111,
      INST16_02_02 = 0b00000000'00000100,
      INST16_03_02 = 0b00000000'00001100,
      INST16_04_00 = 0b00000000'00011111,
      INST16_04_02 = 0b00000000'00011100,
      INST16_04_03 = 0b00000000'00011000,
      INST16_04_04 = 0b00000000'00010000,
      INST16_05_02 = 0b00000000'00111100,      
      INST16_05_03 = 0b00000000'00111000,      
      INST16_05_05 = 0b00000000'00100000,      
      INST16_06_02 = 0b00000000'01111100,
      INST16_06_04 = 0b00000000'01110000,
      INST16_06_05 = 0b00000000'01100000,
      INST16_06_06 = 0b00000000'01000000,
      INST16_07_06 = 0b00000000'1100000,
      INST16_07_07 = 0b00000000'10000000,
      INST16_08_07 = 0b00000001'10000000,
      INST16_08_08 = 0b00000001'00000000,
      INST16_09_07 = 0b00000011'10000000,
      INST16_09_09 = 0b00000010'00000000,
      INST16_10_07 = 0b00000111'10000000,
      INST16_10_09 = 0b00000110'00000000,
      INST16_10_10 = 0b00000100'00000000,
      INST16_11_07 = 0b00001111'10000000,
      INST16_11_10 = 0b00001100'00000000,
      INST16_11_11 = 0b00001000'00000000,
      INST16_12_09 = 0b00011110'00000000,
      INST16_12_10 = 0b00011100'00000000,
      INST16_12_11 = 0b00011000'00000000,
      INST16_12_12 = 0b00010000'00000000,
      INST16_15_13 = 0b11100000'00000000,

      //                3          2          1          0
      //               10987654'32109876'54321098'76543210
      INST32_04_00 = 0b00000000'00010000'00000000'00011111,
      INST32_04_02 = 0b00000000'00010000'00000000'00011100,
      INST32_06_02 = 0b00000000'00010000'00000000'01111100,
      INST32_06_05 = 0b00000000'00010000'00000000'01100000,
      INST32_07_07 = 0b00000000'00010000'00000000'10000000,
      INST32_11_00 = 0b00000000'00000000'00001111'11111111,
      INST32_11_07 = 0b00000000'00000000'00001111'10000000,
      INST32_11_08 = 0b00000000'00000000'00001111'00000000,
      INST32_19_12 = 0b00000000'00001111'11110000'00000000,
      INST32_24_20 = 0b00000001'11110000'00000000'00000000,
      INST32_20_20 = 0b00000000'00010000'00000000'00000000,
      INST32_30_20 = 0b01111111'11110000'00000000'00000000,
      INST32_31_12 = 0b11111111'11111111'11110000'00000000,
      INST32_31_21 = 0b01111111'11100000'00000000'00000000,
      INST32_31_31 = 0b10000000'00000000'00000000'00000000,
      INST32_31_20 = 0b11111111'11110000'00000000'00000000,
      INST32_14_12 = 0b00000000'00000000'01110000'00000000,
      INST32_19_15 = 0b00000000'00001111'10000000'00000000,
      INST32_30_21 = 0b01111111'11100000'00000000'00000000,
      INST32_30_25 = 0b01111110'00000000'00000000'00000000,
      INST32_30_30 = 0b01000000'00000000'00000000'00000000,
      INST32_31_25 = 0b11111110'00000000'00000000'00000000,
  };
// clang-format on

struct DecodeInstruction {
  bool CheckAligned2Bytes(uint32_t Pc) { return (0 != Pc % 2); }
  bool CheckAligned4Bytes(uint32_t Pc) { return (0 != Pc % 4); }
};

struct DecodeInstruction16 : DecodeInstruction {

  inline uint16_t Fetch_15_13(uint32_t Inst) {
    return (INST16_15_13 & Inst) >> 13;
  }

  inline uint16_t Fetch_00_00(uint16_t Inst) {
    return (INST16_00_00 & Inst) >> 0;
  }

  inline uint16_t Fetch_02_01(uint16_t Inst) {
    return (INST16_02_00 & Inst) >> 0;
  }

  inline uint16_t Fetch_02_02(uint16_t Inst) {
    return (INST16_02_02 & Inst) >> 2;
  }

  inline uint16_t Fetch_03_02(uint16_t Inst) {
    return (INST16_03_02 & Inst) >> 2;
  }

  inline uint16_t Fetch_05_02(uint16_t Inst) {
    return (INST16_05_02 & Inst) >> 2;
  }

  inline uint16_t Fetch_05_03(uint16_t Inst) {
    return (INST16_05_03 & Inst) >> 3;
  }

  inline uint16_t Fetch_04_04(uint16_t Inst) {
    return (INST16_04_04 & Inst) >> 4;
  }

  inline uint16_t Fetch_05_05(uint16_t Inst) {
    return (INST16_05_05 & Inst) >> 5;
  }

  inline uint16_t Fetch_06_02(uint16_t Inst) {
    return (INST16_06_02 & Inst) >> 2;
  }

  inline uint16_t Fetch_06_04(uint16_t Inst) {
    return (INST16_06_04 & Inst) >> 4;
  }

  inline uint16_t Fetch_06_05(uint16_t Inst) {
    return (INST16_06_05 & Inst) >> 5;
  }

  inline uint16_t Fetch_06_06(uint16_t Inst) {
    return (INST16_06_06 & Inst) >> 6;
  }

  inline uint16_t Fetch_07_07(uint16_t Inst) {
    return (INST16_07_07 & Inst) >> 7;
  }

  inline uint16_t Fetch_08_07(uint16_t Inst) {
    return (INST16_08_07 & Inst) >> 7;
  }

  inline uint16_t Fetch_09_09(uint16_t Inst) {
    return (INST16_09_09 & Inst) >> 9;
  }

  inline uint16_t Fetch_10_07(uint16_t Inst) {
    return (INST16_10_07 & Inst) >> 9;
  }

  inline uint16_t Fetch_10_09(uint16_t Inst) {
    return (INST16_10_09 & Inst) >> 9;
  }

  inline uint16_t Fetch_10_10(uint16_t Inst) {
    return (INST16_10_10 & Inst) >> 10;
  }

  inline uint16_t Fetch_11_10(uint16_t Inst) {
    return (INST16_11_10 & Inst) >> 10;
  }

  inline uint16_t Fetch_11_07(uint16_t Inst) {
    return (INST16_11_07 & Inst) >> 7;
  }

  inline uint16_t Fetch_11_11(uint16_t Inst) {
    return (INST16_11_11 & Inst) >> 11;
  }

  inline uint16_t Fetch_12_09(uint16_t Inst) {
    return (INST16_12_09 & Inst) >> 9;
  }

  inline uint16_t Fetch_12_10(uint16_t Inst) {
    return (INST16_12_10 & Inst) >> 10;
  }

  inline uint16_t Fetch_12_11(uint16_t Inst) {
    return (INST16_12_11 & Inst) >> 11;
  }

  inline uint16_t Fetch_12_12(uint16_t Inst) {
    return (INST16_12_12 & Inst) >> 12;
  }

  inline uint16_t Fetch_04_03(uint16_t Inst) {
    return (INST16_04_03 & Inst) >> 3;
  }

  inline uint16_t Fetch_04_00(uint16_t Inst) {
    return (INST16_04_00 & Inst) >> 0;
  }

  inline uint16_t Fetch_07_06(uint16_t Inst) {
    return (INST16_07_06 & Inst) >> 6;
  }

  inline uint16_t Fetch_08_08(uint16_t Inst) {
    return (INST16_08_08 & Inst) >> 8;
  }

  inline uint16_t Fetch_01_00(uint16_t Inst) {
    return (INST16_01_00 & Inst) >> 0;
  }

  inline uint16_t Fetch_09_07(uint16_t Inst) {
    return (INST16_09_07 & Inst) >> 7;
  }

  inline uint16_t Fetch_04_02(uint16_t Inst) {
    return (INST16_04_02 & Inst) >> 2;
  }

  // sign extend a 16 bit value
  inline uint32_t SignExtend16(uint32_t x) { return (int32_t)((int16_t)x); }

  // sign extend an 8 bit value
  inline uint32_t SignExtend8(uint32_t x) { return (int32_t)((int8_t)x); }

  inline int16_t FetchImmCiFmt_54276(uint32_t Inst) {
    // INST[06:02]
    const uint16_t inst_07_06 = Fetch_03_02(Inst);
    const uint16_t inst_04_02 = Fetch_06_04(Inst);
    // INST[12:12]
    const uint16_t inst_05_05 = Fetch_12_12(Inst);

    // Combine them
    const uint16_t uimm = (inst_07_06 << 6) | //
                          (inst_04_02 << 2) | //
                          (inst_05_05 << 5);

    // Cast to sign integer
    const int16_t imm = ((int16_t)(uimm << 8)) >> 8;

    return imm;
  }

  inline int16_t FetchImmCiFmt_54386(uint32_t Inst) {
    // INST[06:02]
    const uint16_t inst_08_06 = Fetch_03_02(Inst);
    const uint16_t inst_04_03 = Fetch_06_04(Inst);
    // INST[12:12]
    const uint16_t inst_05_05 = Fetch_12_12(Inst);

    // Combine them
    const uint16_t uimm = (inst_08_06 << 6) | //
                          (inst_04_03 << 3) | //
                          (inst_05_05 << 5);

    // Cast to sign integer
    const int16_t imm = ((int16_t)(uimm << 7)) >> 7;

    return imm;
  }

  inline int16_t FetchImmCiFmt_5496(uint32_t Inst) {
    // INST[06:02]
    const uint16_t inst_09_06 = Fetch_05_02(Inst);
    const uint16_t inst_04_04 = Fetch_06_06(Inst);
    // INST[12:12]
    const uint16_t inst_05_05 = Fetch_12_12(Inst);

    // Combine them
    const uint16_t uimm = (inst_09_06 << 6) | //
                          (inst_04_04 << 4) | //
                          (inst_05_05 << 5);

    // Cast to sign integer
    const int16_t imm = ((int16_t)(uimm << 6)) >> 6;

    return imm;
  }

  inline int16_t FetchImmCiFmt_540(uint32_t Inst) {
    // INST[06:02]
    const uint16_t inst_04_00 = Fetch_06_02(Inst);
    // INST[12:12]
    const uint16_t inst_05_05 = Fetch_12_12(Inst);

    // Combine them
    const uint16_t uimm = (inst_04_00 << 0) | //
                          (inst_05_05 << 5);

    // Cast to sign integer
    const int16_t imm = ((int16_t)(uimm << 10)) >> 10;

    return imm;
  }

  inline int16_t FetchImmCiFmt_546875_qq(uint32_t Inst) {
    // INST[06:02]
    const uint16_t inst_05_05 = Fetch_02_02(Inst);
    const uint16_t inst_08_07 = Fetch_04_03(Inst);
    const uint16_t inst_06_06 = Fetch_05_05(Inst);
    const uint16_t inst_04_04 = Fetch_06_06(Inst);
    // INST[12:12]
    const uint16_t inst_09_09 = Fetch_12_12(Inst);

    // Combine them
    const uint16_t uimm = (inst_05_05 << 5) | //
                          (inst_08_07 << 7) | //
                          (inst_06_06 << 6) | //
                          (inst_04_04 << 4) | //
                          (inst_09_09 << 9);

    // Cast to sign integer ??
    const int16_t imm = ((int16_t)(uimm << 6)) >> 6;

    return imm;
  }

  inline int32_t FetchImmCiFmt_171612_qq(uint32_t Inst) {
    // INST[06:02]
    const uint32_t inst_16_12 = Fetch_06_02(Inst);
    // INST[12:12]
    const uint32_t inst_17_17 = Fetch_12_12(Inst);

    // Combine them
    const uint16_t uimm = (inst_16_12 << 12) | //
                          (inst_17_17 << 17);

    // Cast to sign integer ????
    const int16_t imm = ((int16_t)(uimm << 10)) >> 10;

    return imm;
  }

  inline int16_t FetchImmCssFmt_5276(uint32_t Inst) {
    // INST[12:07]
    const uint16_t inst_07_06 = Fetch_08_07(Inst);
    const uint16_t inst_05_02 = Fetch_12_09(Inst);

    // Combine them
    const uint16_t uimm = (inst_07_06 << 6) | //
                          (inst_05_02 << 2);

    // Cast to sign integer
    const int16_t imm = ((int16_t)(uimm << 8)) >> 8;

    return imm;
  }

  inline int16_t FetchImmCssFmt_5386(uint32_t Inst) {
    // INST[12:07]
    const uint16_t inst_08_06 = Fetch_09_07(Inst);
    const uint16_t inst_05_03 = Fetch_12_10(Inst);

    // Combine them
    const uint16_t uimm = (inst_08_06 << 6) | //
                          (inst_05_03 << 3);

    // Cast to sign integer
    const int16_t imm = ((int16_t)(uimm << 7)) >> 7;

    return imm;
  }

  inline int16_t FetchImmCssFmt_5496(uint32_t Inst) {
    // INST[12:07]
    const uint16_t inst_09_06 = Fetch_10_07(Inst);
    const uint16_t inst_05_04 = Fetch_12_10(Inst);

    // Combine them
    const uint16_t uimm = (inst_09_06 << 6) | //
                          (inst_05_04 << 4);

    // Cast to sign integer
    const int16_t imm = ((int16_t)(uimm << 6)) >> 6;

    return imm;
  }

  inline int16_t FetchImmCiwFmt_549623(uint32_t Inst) {
    // INST[12:05]
    const uint16_t inst_03_03 = Fetch_05_05(Inst);
    const uint16_t inst_02_02 = Fetch_06_06(Inst);
    const uint16_t inst_09_06 = Fetch_10_07(Inst);
    const uint16_t inst_05_04 = Fetch_12_11(Inst);

    // Combine them
    const uint16_t uimm = (inst_03_03 << 3) | //
                          (inst_02_02 << 2) | //
                          (inst_09_06 << 6) | //
                          (inst_05_04 << 4);

    // Cast to sign integer ??
    const int16_t imm = ((int16_t)(uimm << 6)) >> 6;

    return imm;
  }

  inline int16_t FetchImmClFmt_5326(uint32_t Inst) {
    // INST[06:05]
    const uint16_t inst_06_06 = Fetch_05_05(Inst);
    const uint16_t inst_02_02 = Fetch_06_06(Inst);
    // INST[12:10]
    const uint16_t inst_05_03 = Fetch_12_10(Inst);

    // Combine them
    const uint16_t uimm = (inst_06_06 << 6) | //
                          (inst_02_02 << 2) | //
                          (inst_05_03 << 3);

    // Cast to sign integer
    const int16_t imm = ((int16_t)(uimm << 9)) >> 9;

    return imm;
  }

  inline int16_t FetchImmClFmt_5376(uint32_t Inst) {
    // INST[06:05]
    const uint16_t inst_07_06 = Fetch_06_05(Inst);
    // INST[12:10]
    const uint16_t inst_05_03 = Fetch_12_10(Inst);

    // Combine them
    const uint16_t uimm = (inst_07_06 << 6) | //
                          (inst_05_03 << 3);

    // Cast to sign integer
    const int16_t imm = ((int16_t)(uimm << 8)) >> 8;

    return imm;
  }

  inline int16_t FetchImmClFmt_54876(uint32_t Inst) {
    // INST[06:05]
    const uint16_t inst_07_06 = Fetch_06_05(Inst);
    // INST[12:10]
    const uint16_t inst_05_05 = Fetch_10_10(Inst);
    const uint16_t inst_04_04 = Fetch_11_11(Inst);
    const uint16_t inst_08_08 = Fetch_12_12(Inst);

    // Combine them
    const uint16_t uimm = (inst_07_06 << 6) | //
                          (inst_05_05 << 5) | //
                          (inst_04_04 << 4) | //
                          (inst_08_08 << 8);

    // Cast to sign integer
    const int16_t imm = ((int16_t)(uimm << 7)) >> 7;

    return imm;
  }

  inline int16_t FetchImmCbFmt_84376215(uint32_t Inst) {
    // INST[06:02]
    const uint16_t inst_05_05 = Fetch_02_02(Inst);
    const uint16_t inst_02_01 = Fetch_04_03(Inst);
    const uint16_t inst_07_06 = Fetch_06_05(Inst);
    // INST[12:10]
    const uint16_t inst_04_03 = Fetch_11_10(Inst);
    const uint16_t inst_08_08 = Fetch_12_12(Inst);

    // Combine them
    const uint16_t uimm = (inst_05_05 << 5) | //
                          (inst_02_01 << 1) | //
                          (inst_07_06 << 6) | //
                          (inst_04_03 << 2) | //
                          (inst_08_08 << 8);

    // Cast to sign integer
    const int16_t imm = ((int16_t)(uimm << 7)) >> 7;

    return imm;
  }

  inline uint16_t FetchImmCbFmt_540(uint32_t Inst) {
    // INST[06:02]
    const uint16_t inst_04_00 = Fetch_06_02(Inst);
    // INST[12:10]
    const uint16_t inst_05_05 = Fetch_12_12(Inst);

    // Combine them
    const uint16_t shamt = (inst_05_05 << 5) | //
                           (inst_04_00 << 0);
    return shamt;
  }

  inline int16_t FetchImmCjFmt_114981067315(uint32_t Inst) {
    // INST[12:02]
    const uint16_t inst_05_05 = Fetch_02_02(Inst);
    const uint16_t inst_03_01 = Fetch_05_03(Inst);
    const uint16_t inst_07_07 = Fetch_06_06(Inst);
    const uint16_t inst_06_06 = Fetch_07_07(Inst);
    const uint16_t inst_10_10 = Fetch_08_08(Inst);
    const uint16_t inst_09_08 = Fetch_10_09(Inst);
    const uint16_t inst_04_04 = Fetch_11_11(Inst);
    const uint16_t inst_11_11 = Fetch_12_12(Inst);

    // Combine them
    const uint16_t uimm = (inst_05_05 << 5) |  //
                          (inst_03_01 << 1) |  //
                          (inst_07_07 << 7) |  //
                          (inst_06_06 << 6) |  //
                          (inst_10_10 << 10) | //
                          (inst_09_08 << 8) |  //
                          (inst_04_04 << 4) |  //
                          (inst_11_11 << 11);

    // Cast to sign integer
    const int16_t imm = ((int16_t)(uimm << 4)) >> 4;

    return imm;
  }

  inline int16_t FetchImmCsFmt_5326(uint32_t Inst) {
    // INST[06:05]
    const uint16_t inst_06_06 = Fetch_05_05(Inst);
    const uint16_t inst_07_07 = Fetch_06_06(Inst);
    // INST[12:10]
    const uint16_t inst_05_03 = Fetch_12_10(Inst);

    // Combine them
    const uint16_t uimm = (inst_06_06 << 6) | //
                          (inst_07_07 << 7) | //
                          (inst_05_03 << 3);

    // Cast to sign integer
    const int16_t imm = ((int16_t)(uimm << 8)) >> 8;

    return imm;
  }

  inline int16_t FetchImmCsFmt_5376(uint32_t Inst) {
    // INST[06:05]
    const uint16_t inst_07_06 = Fetch_06_05(Inst);
    // INST[12:10]
    const uint16_t inst_05_03 = Fetch_12_10(Inst);

    // Combine them
    const uint16_t uimm = (inst_07_06 << 6) | //
                          (inst_05_03 << 3);

    // Cast to sign integer
    const int16_t imm = ((int16_t)(uimm << 8)) >> 8;

    return imm;
  }

  inline int16_t FetchImmCsFmt_54876(uint32_t Inst) {
    // INST[06:05]
    const uint16_t inst_07_06 = Fetch_06_05(Inst);
    // INST[12:10]
    const uint16_t inst_08_08 = Fetch_10_10(Inst);
    const uint16_t inst_04_04 = Fetch_11_11(Inst);
    const uint16_t inst_05_05 = Fetch_12_12(Inst);

    // Combine them
    const uint16_t uimm = (inst_07_06 << 6) | //
                          (inst_08_08 << 8) | //
                          (inst_04_04 << 4) | //
                          (inst_05_05 << 5);

    // Cast to sign integer
    const int16_t imm = ((int16_t)(uimm << 7)) >> 7;

    return imm;
  }
};

struct DecodeInstruction32 : DecodeInstruction {

  inline uint32_t Fetch_04_00(uint32_t Inst) {
    return (INST32_04_00 & Inst) >> 0;
  }

  inline uint32_t Fetch_04_02(uint32_t Inst) {
    return (INST32_04_02 & Inst) >> 2;
  }

  inline uint32_t Fetch_06_02(uint32_t Inst) {
    return (INST32_06_02 & Inst) >> 2;
  }

  inline uint32_t Fetch_06_05(uint32_t Inst) {
    return (INST32_06_05 & Inst) >> 5;
  }

  inline uint32_t Fetch_07_07(uint32_t Inst) {
    return (INST32_07_07 & Inst) >> 7;
  }

  inline uint32_t Fetch_11_00(uint32_t Inst) {
    return (INST32_11_00 & Inst) >> 0;
  }

  inline uint32_t Fetch_11_07(uint32_t Inst) {
    return (INST32_11_07 & Inst) >> 7;
  }

  inline uint32_t Fetch_11_08(uint32_t Inst) {
    return (INST32_11_08 & Inst) >> 8;
  }

  inline uint32_t Fetch_14_12(uint32_t Inst) {
    return (INST32_14_12 & Inst) >> 12;
  }

  inline uint32_t Fetch_20_20(uint32_t Inst) {
    return (INST32_20_20 & Inst) >> 20;
  }

  inline uint32_t Fetch_19_12(uint32_t Inst) {
    return (INST32_19_12 & Inst) >> 12;
  }

  inline uint32_t Fetch_19_15(uint32_t Inst) {
    return (INST32_19_15 & Inst) >> 15;
  }

  inline uint32_t Fetch_24_20(uint32_t Inst) {
    return (INST32_24_20 & Inst) >> 20;
  }

  inline uint32_t Fetch_30_21(uint32_t Inst) {
    return (INST32_30_21 & Inst) >> 21;
  }
  inline uint32_t Fetch_30_25(uint32_t Inst) {
    return (INST32_30_25 & Inst) >> 25;
  }

  inline uint32_t Fetch_30_30(uint32_t Inst) {
    return (INST32_30_30 & Inst) >> 25;
  }

  inline uint32_t Fetch_31_12(uint32_t Inst) {
    return (INST32_31_12 & Inst) >> 12;
  }
  inline uint32_t Fetch_31_20(uint32_t Inst) {
    return (INST32_31_20 & Inst) >> 20;
  }
  inline uint32_t Fetch_31_25(uint32_t Inst) {
    return (INST32_31_25 & Inst) >> 25;
  }
  inline uint32_t Fetch_31_31(uint32_t Inst) {
    return (INST32_31_31 & Inst) >> 31;
  }

  inline uint32_t SignExtW(uint32_t Val) {
    const int32_t new_val = (int32_t)((int32_t)Val);
    return new_val;
  }

  inline uint32_t SignExtH(uint32_t Val) {
    const int32_t new_val = (int32_t)((int16_t)Val);
    return new_val;
  }

  inline uint32_t SignExtB(uint32_t Val) {
    const int32_t new_val = (int32_t)((int8_t)Val);
    return new_val;
  }

  inline int32_t FetchImmIType(uint32_t Inst) {
    // INST[11:00]
    const uint32_t inst_11_00 = Fetch_31_20(Inst);

    // Combine them
    const uint32_t uimm = inst_11_00;

    // Cast to sign integer
    const int32_t imm = ((int32_t)(uimm << 20)) >> 20;
    return imm;
  }

  inline int32_t FetchImmSType(uint32_t Inst) {
    // INST[11:07]
    const uint32_t inst_04_00 = Fetch_11_07(Inst);
    // INST[31:25]
    const uint32_t inst_11_05 = Fetch_31_25(Inst);

    // Combine them
    const uint32_t uimm = (inst_11_05 << 5) | //
                          (inst_04_00 << 0);

    // Cast to sign integer
    const int32_t imm = ((int32_t)(uimm << 20)) >> 20;
    return imm;
  }

  inline int32_t FetchImmBType(uint32_t Inst) {
    // INST[11:07]
    const uint32_t inst_11_11 = Fetch_07_07(Inst);
    const uint32_t inst_04_01 = Fetch_11_08(Inst);
    // INST[31:25]
    const uint32_t inst_10_05 = Fetch_30_25(Inst);
    const uint32_t inst_12_12 = Fetch_31_31(Inst);

    // Combine them
    const uint32_t uimm = (inst_11_11 << 11) | //
                          (inst_04_01 << 1) |  //
                          (inst_10_05 << 5) |  //
                          (inst_12_12 << 12);

    // Cast to sign integer
    const int32_t imm = ((int32_t)(uimm << 19)) >> 19;
    return imm;
  }

  inline uint32_t FetchImmUType(uint32_t Inst) {
    // INST[31:12]
    const uint32_t inst_31_12 = Fetch_31_12(Inst);

    // Combine them
    const uint32_t uimm = (inst_31_12 << 12);

    // Cast to sign integer
    return uimm;
  }

  inline int32_t FetchImmJType(uint32_t Inst) {
    // INST[31:12]
    const uint32_t inst_19_12 = Fetch_19_12(Inst);
    const uint32_t inst_11_11 = Fetch_20_20(Inst);
    const uint32_t inst_10_01 = Fetch_30_21(Inst);
    const uint32_t inst_20_20 = Fetch_31_31(Inst);

    // Combine them
    const uint32_t uimm = (inst_19_12 << 12) | //
                          (inst_11_11 << 11) | //
                          (inst_10_01 << 1) |  //
                          (inst_20_20 << 20);

    // Cast to sign integer
    const int32_t imm = ((int32_t)(uimm << 11)) >> 11;
    return imm;
  }
};
} // namespace rvemu