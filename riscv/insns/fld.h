require_extension('D');
require_fp;
auto l = MMU.load_uint64(RS1 + insn.i_imm());
TAG.load<uint64_t>(pc_tag, l.second, TAG_DEST, TAG_RS1, true);
WRITE_FRD(f64(l.first));
