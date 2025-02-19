require_extension('C');
if (xlen == 32) {
  require_extension('F');
  require_fp;
  reg_t store_addr = RVC_SP + insn.rvc_swsp_imm();
  uint32_t tag = TAG.store<uint32_t>(pc_tag, store_addr, TAG_C_SP, TAG_RS2_FC);
  MMU.store_uint32(store_addr, std::make_pair(RVC_FRS2.v[0], tag));
} else { // c.sdsp
  reg_t store_addr = RVC_SP + insn.rvc_sdsp_imm();
  uint64_t tag = TAG.store<uint64_t>(pc_tag, store_addr, TAG_C_SP, TAG_RS2_C);
  MMU.store_uint64(store_addr, std::make_pair(RVC_RS2, tag));
}
