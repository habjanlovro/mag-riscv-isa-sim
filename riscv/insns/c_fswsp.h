require_extension('C');
if (xlen == 32) {
  require_extension('F');
  require_fp;
  uint32_t tag = TAG.store<uint32_t>(pc_tag, TAG_C_SP, TAG_RS2_FC);
  MMU.store_uint32(RVC_SP + insn.rvc_swsp_imm(), std::make_pair(RVC_FRS2.v[0], tag));
} else { // c.sdsp
  uint64_t tag = TAG.store<uint64_t>(pc_tag, TAG_C_SP, TAG_RS2_C);
  MMU.store_uint64(RVC_SP + insn.rvc_sdsp_imm(), std::make_pair(RVC_RS2, tag));
}
