/*
Copyright (c) <2012>, <Georgia Institute of Technology> All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted 
provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions 
and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice, this list of 
conditions and the following disclaimer in the documentation and/or other materials provided 
with the distribution.

Neither the name of the <Georgia Institue of Technology> nor the names of its contributors 
may be used to endorse or promote products derived from this software without specific prior 
written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef TRACE_GENERATOR_H
#define TRACE_GENERATOR_H
#include <inttypes.h>
#include "data_types.h"

#include <stdio.h>
#include <zlib.h>
#include <ostream>
extern "C" {
#include "xed-interface.h"
}

#define MAX_SRC_NUM 9
#define MAX_DST_NUM 6
#define USE_MAP 0
#define MAX_THREADS 1000

/**
 * Instruction Information
 */

struct Inst_info {
  uint8_t  num_read_regs;       // 3-bits
  uint8_t  num_dest_regs;       // 3-bits
  uint8_t  src[MAX_SRC_NUM];    // 6-bits * 4
  uint8_t  dst[MAX_DST_NUM];    // 6-bits * 4
  uint8_t  cf_type;             // 4 bits
  bool     has_immediate;       // 1bits
  uint8_t  opcode;              // 6 bits
  bool     has_st;              // 1 bit
  bool     is_fp;               // 1bit
  bool     write_flg;           // 1bit
  uint8_t  num_ld;              // 2bit
  uint8_t  size;                // 5 bit
  // **** dynamic ****
  uint64_t ld_vaddr1;           // 4 bytes
  uint64_t ld_vaddr2;           // 4 bytes
  uint64_t st_vaddr;            // 4 bytes
  uint64_t instruction_addr;    // 4 bytes
  uint64_t branch_target;       // not the dynamic info. static info  // 4 bytes
  uint8_t  mem_read_size;       // 8 bit
  uint8_t  mem_write_size;      // 8 bit
  bool     rep_dir;             // 1 bit
  bool     actually_taken;      // 1 ibt
};

#define BUF_SIZE (10000 * sizeof(struct Inst_info))

struct Trace_info {
  gzFile    trace_stream;
  char      trace_buf[BUF_SIZE];
  int       bytes_accumulated;
  std::ofstream* debug_stream;
};

typedef struct{
	uint64_t addr;
	uint8_t size;
	bool is_read;	
} child_memop;

enum CPU_OPCODE_enum {
  TR_MUL = XED_CATEGORY_LAST ,
  TR_DIV,
  TR_FMUL,
  TR_FDIV,
  TR_NOP,
  PREFETCH_NTA,
  PREFETCH_T0,
  PREFETCH_T1,
  PREFETCH_T2,
  GPU_EN,
  CPU_MEM_EXT_OP,   // To denote that the op is a continuation of the same parent op but has more mem accesses.
  CPU_OPCODE_LAST,
} CPU_OPCODE;


enum CF_TYPE_enum {
  NOT_CF,   // not a control flow instruction
  CF_BR,   // an unconditional branch
  CF_CBR,   // a conditional branch
  CF_CALL,  // a call
  // below this point are indirect cfs
  CF_IBR,   // an indirect branch
  CF_ICALL,   // an indirect call
  CF_ICO,   // an indirect jump to co-routine
  CF_RET,   // a return
  CF_SYS,
  CF_ICBR     // an indirect conditional branch
} CF_TYPE;

std::string tr_cf_names[15] = {
  "NOT_CF",   // not a control flow instruction
  "CF_BR",    // an unconditional branch
  "CF_CBR",   // a conditional branch
  "CF_CALL",  // a call
  "CF_IBR",   // an indirect branch
  "CF_ICALL", // an indirect call
  "CF_ICO",   // an indirect jump to co-routine
  "CF_RET",   // a return
  "CF_SYS",
  "CF_ICBR"
};

std::string tr_opcode_names[67] = {
  "INVALID",
  "3DNOW",
  "AES",
  "AVX",
  "AVX2", // new
  "AVX2GATHER", // new
  "BDW", // new
  "BINARY",
  "BITBYTE",
  "BMI1", // new
  "BMI2", // new
  "BROADCAST",
  "CALL",
  "CMOV",
  "COND_BR",
  "CONVERT",
  "DATAXFER",
  "DECIMAL",
  "FCMOV",
  "FLAGOP",
  "FMA4", // new
  "INTERRUPT",
  "IO",
  "IOSTRINGOP",
  "LOGICAL",
  "LZCNT", // new
  "MISC",
  "MMX",
  "NOP",
  "PCLMULQDQ",
  "POP",
  "PREFETCH",
  "PUSH",
  "RDRAND", // new
  "RDSEED", // new
  "RDWRFSGS", // new
  "RET",
  "ROTATE",
  "SEGOP",
  "SEMAPHORE",
  "SHIFT",
  "SSE",
  "STRINGOP",
  "STTNI",
  "SYSCALL",
  "SYSRET",
  "SYSTEM",
  "TBM", // new
  "UNCOND_BR",
  "VFMA", // new
  "VTX",
  "WIDENOP",
  "X87_ALU",
  "XOP",
  "XSAVE",
  "XSAVEOPT",
  "TR_MUL",
  "TR_DIV",
  "TR_FMUL",
  "TR_FDIV",
  "TR_NOP",
  "PREFETCH_NTA",
  "PREFETCH_T0",
  "PREFETCH_T1",
  "PREFETCH_T2",
  "GPU_EN",
  "CPU_MEM_EXT_OP"
};

typedef UINT64 ADDRINT;
typedef UINT32 THREADID;
typedef std::pair<ADDRINT, ADDRINT> va_pa_pair;

#endif // TRACE_GENERATOR_H
