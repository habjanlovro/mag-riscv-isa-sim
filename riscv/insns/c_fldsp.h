require_extension('C');
require_extension('D');
require_fp;
reg_t addr = RVC_SP + insn.rvc_ldsp_imm();
auto l = MMU.load_uint64(addr);
TAG.load<uint64_t>(pc_tag, addr, l.second, TAG_DEST, TAG_C_SP, true);
WRITE_FRD(f64(l.first));
