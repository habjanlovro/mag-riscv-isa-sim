auto l = MMU.load_int16(RS1 + insn.i_imm());
TAG.load<int16_t>(pc_tag, l.second, TAG_DEST, TAG_RS1);
WRITE_RD(l.first);
