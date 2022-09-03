require_rv64;
auto l = MMU.load_int64(RS1 + insn.i_imm());
TAG.load<int64_t>(pc_tag, l.second, TAG_DEST, TAG_RS1);
WRITE_RD(l.first);
