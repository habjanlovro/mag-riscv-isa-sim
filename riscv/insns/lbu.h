reg_t addr = RS1 + insn.i_imm();
WRITE_RD(MMU.load_uint8(addr));
TAG.load(pc, TAG_DEST, TAG_RS1, addr, sizeof(uint8_t));
