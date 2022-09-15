reg_t addr = RS1 + insn.i_imm();
auto l = MMU.load_uint8(addr);
TAG.load<uint8_t>(pc_tag, addr, l.second, TAG_DEST, TAG_RS1);
WRITE_RD(l.first);
