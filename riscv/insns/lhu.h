reg_t addr = RS1 + insn.i_imm();
WRITE_RD(MMU.load_uint16(addr));
TAG.load(pc, TAG_DEST, TAG_RS1, addr, sizeof(uint16_t));
