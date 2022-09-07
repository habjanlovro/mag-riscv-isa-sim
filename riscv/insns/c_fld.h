require_extension('C');
require_extension('D');
require_fp;
auto l = MMU.load_uint64(RVC_RS1S + insn.rvc_ld_imm());
TAG.load<uint64_t>(pc_tag, l.second, insn.rvc_rs2s(), TAG_RS1_CS, true);
WRITE_RVC_FRS2S(f64(l.first));
