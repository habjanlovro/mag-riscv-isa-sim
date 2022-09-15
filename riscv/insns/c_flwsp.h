require_extension('C');
if (xlen == 32) {
  require_extension('F');
  require_fp;
  reg_t addr = RVC_SP + insn.rvc_lwsp_imm();
  auto l = MMU.load_uint32(addr);
  TAG.load<uint32_t>(pc_tag, addr, l.second, TAG_DEST, TAG_C_SP, true);
  WRITE_FRD(f32(l.first));
} else { // c.ldsp
  require(insn.rvc_rd() != 0);
  reg_t addr = RVC_SP + insn.rvc_ldsp_imm();
  auto l = MMU.load_int64(addr);
  TAG.load<int64_t>(pc_tag, addr, l.second, TAG_DEST, TAG_C_SP);
  WRITE_RD(l.first);
}
