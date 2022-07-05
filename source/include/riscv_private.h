#include <climits>

#include "riscv.h"

#define RV_NUM_REGS 32

struct riscv_t {
    bool halt;

    // io interface
    struct riscv_io_t io;

    // integer registers
    riscv_word_t X[RV_NUM_REGS];
    riscv_word_t PC;

    // user provided data
    riscv_user_t userdata;

    // csr registers
    uint64_t csr_cycle;
    uint32_t csr_mstatus;
    uint32_t csr_mtvec;
    uint32_t csr_misa;
    uint32_t csr_mtval;
    uint32_t csr_mcause;
    uint32_t csr_mscratch;
    uint32_t csr_mepc;
    uint32_t csr_mip;
    uint32_t csr_mbadaddr;
    
    // current instruction length
    uint8_t inst_len;
};