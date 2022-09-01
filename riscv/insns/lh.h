auto l = MMU.load_int16(RS1 + insn.i_imm());
WRITE_RD(l.first);
// TAG.load(pc, TAG_DEST, TAG_RS1, addr, sizeof(int16_t));
