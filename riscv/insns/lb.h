reg_t addr = RS1 + insn.i_imm();
auto l = MMU.load_int8(addr);
TAG.load<int8_t>(pc_tag, l.second, TAG_DEST, TAG_RS1);
WRITE_RD(l.first);
