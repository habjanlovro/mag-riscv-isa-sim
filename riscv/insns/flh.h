require_extension(EXT_ZFHMIN);
require_fp;
auto l = MMU.load_uint16(RS1 + insn.i_imm());
TAG.load<uint16_t>(pc_tag, l.second, TAG_RS1, true);
WRITE_FRD(f16(l.first));
