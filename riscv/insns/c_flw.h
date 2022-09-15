require_extension('C');
if (xlen == 32) {
  require_extension('F');
  require_fp;
  reg_t addr = RVC_RS1S + insn.rvc_lw_imm();
  auto l = MMU.load_uint32(addr);
  TAG.load<uint32_t>(pc_tag, addr, l.second, insn.rvc_rs2s(), TAG_RS1_CS, true);
  WRITE_RVC_FRS2S(f32(l.first));
} else { // c.ld
  reg_t addr = RVC_RS1S + insn.rvc_ld_imm();
  auto l = MMU.load_int64(addr);
  TAG.load<int64_t>(pc_tag, addr, l.second, insn.rvc_rs2s(), TAG_RS1_CS);
  WRITE_RVC_RS2S(l.first);
}
