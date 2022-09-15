reg_t addr = RS1 + insn.i_imm();
auto l = MMU.load_uint16(addr);
TAG.load<uint16_t>(pc_tag, addr, l.second, TAG_DEST, TAG_RS1);
WRITE_RD(l.first);
