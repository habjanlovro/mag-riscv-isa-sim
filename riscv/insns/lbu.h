auto l = MMU.load_uint8(RS1 + insn.i_imm());
TAG.load<uint8_t>(pc_tag, l.second, TAG_DEST, TAG_RS1);
WRITE_RD(l.first);
