require_extension('C');
reg_t addr = RVC_RS1S + insn.rvc_lw_imm();
auto l = MMU.load_int32(addr);
TAG.load<int32_t>(pc_tag, addr, l.second, insn.rvc_rs2s(), TAG_RS1_CS);
WRITE_RVC_RS2S(l.first);
