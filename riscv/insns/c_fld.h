require_extension('C');
require_extension('D');
require_fp;
reg_t addr = RVC_RS1S + insn.rvc_ld_imm();
auto l = MMU.load_uint64(addr);
TAG.load<uint64_t>(pc_tag, addr, l.second, insn.rvc_rs2s(), TAG_RS1_CS, true);
WRITE_RVC_FRS2S(f64(l.first));
