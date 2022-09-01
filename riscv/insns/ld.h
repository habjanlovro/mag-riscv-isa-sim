require_rv64;
reg_t addr = RS1 + insn.i_imm();
WRITE_RD(MMU.load_int64(addr));
TAG.load(pc, TAG_DEST, TAG_RS1, addr, sizeof(int64_t));
