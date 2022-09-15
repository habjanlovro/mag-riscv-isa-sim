reg_t addr = RS1 + insn.i_imm();
auto l = MMU.load_int16(addr);
TAG.load<int16_t>(pc_tag, addr, l.second, TAG_DEST, TAG_RS1);
WRITE_RD(l.first);
