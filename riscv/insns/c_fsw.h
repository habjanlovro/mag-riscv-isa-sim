require_extension('C');
if (xlen == 32) {
  require_extension('F');
  require_fp;
  uint32_t tag = TAG.store<uint32_t>(pc_tag, TAG_RS1_CS, TAG_RS2_FCS);
  MMU.store_uint32(RVC_RS1S + insn.rvc_lw_imm(), std::make_pair(RVC_FRS2S.v[0], tag));
} else { // c.sd
  uint64_t tag = TAG.store<uint64_t>(pc_tag, TAG_RS1_CS, TAG_RS2_FCS);
  MMU.store_uint64(RVC_RS1S + insn.rvc_ld_imm(), std::make_pair(RVC_RS2S, tag));
}
