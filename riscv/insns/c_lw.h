require_extension('C');
auto l = MMU.load_int32(RVC_RS1S + insn.rvc_lw_imm());
TAG.load<int32_t>(pc_tag, l.second, insn.rvc_rs2s(), TAG_RS1_CS);
WRITE_RVC_RS2S(l.first);
