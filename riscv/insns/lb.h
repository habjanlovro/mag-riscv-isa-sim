reg_t addr = RS1 + insn.i_imm();
auto l = MMU.load_int8(addr);
WRITE_RD(l.first);
// TAG.load(pc, TAG_DEST, TAG_RS1, addr, sizeof(int8_t));
