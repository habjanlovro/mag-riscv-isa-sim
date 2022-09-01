require_rv64;
auto l = MMU.load_int64(RS1 + insn.i_imm());
WRITE_RD(l.first);
// TAG.load(pc, TAG_DEST, TAG_RS1, addr, sizeof(int64_t));
