require_extension('F');
require_fp;
auto l = MMU.load_uint32(RS1 + insn.i_imm());
TAG.load<uint32_t>(pc_tag, l.second, TAG_DEST, TAG_RS1, true);
WRITE_FRD(f32(l.first));
