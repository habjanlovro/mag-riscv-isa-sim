require_extension('C');
require(insn.rvc_rd() != 0);
auto l = MMU.load_int32(RVC_SP + insn.rvc_lwsp_imm());
TAG.load<int32_t>(pc_tag, l.second, TAG_DEST, TAG_C_SP);
WRITE_RD(l.first);
