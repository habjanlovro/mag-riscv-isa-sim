require_rv64;
auto l = MMU.load_uint32(RS1 + insn.i_imm());
TAG.load<uint32_t>(pc_tag, l.second, TAG_DEST, TAG_RS1);
WRITE_RD(l.first);
