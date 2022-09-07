require_extension('C');
require_extension('D');
require_fp;
auto l = MMU.load_uint64(RVC_SP + insn.rvc_ldsp_imm());
TAG.load<uint64_t>(pc_tag, l.second, TAG_DEST, TAG_C_SP, true);
WRITE_FRD(f64(l.first));
