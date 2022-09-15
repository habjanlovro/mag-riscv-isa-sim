require_extension('C');
require(insn.rvc_rd() != 0);
reg_t addr = RVC_SP + insn.rvc_lwsp_imm();
auto l = MMU.load_int32(addr);
TAG.load<int32_t>(pc_tag, addr, l.second, TAG_DEST, TAG_C_SP);
WRITE_RD(l.first);
