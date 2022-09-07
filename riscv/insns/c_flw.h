require_extension('C');
if (xlen == 32) {
  require_extension('F');
  require_fp;
  auto l = MMU.load_uint32(RVC_RS1S + insn.rvc_lw_imm());
  TAG.load<uint32_t>(pc_tag, l.second, insn.rvc_rs2s(), TAG_RS1_CS, true);
  WRITE_RVC_FRS2S(f32(l.first));
} else { // c.ld
  auto l = MMU.load_int64(RVC_RS1S + insn.rvc_ld_imm());
  TAG.load<int64_t>(pc_tag, l.second, insn.rvc_rs2s(), TAG_RS1_CS);
  WRITE_RVC_RS2S(l.first);
}
