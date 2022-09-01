require_extension('C');
if (xlen == 32) {
  require_extension('F');
  require_fp;
  MMU.store_uint32(RVC_RS1S + insn.rvc_lw_imm(), std::make_pair(RVC_FRS2S.v[0], 0));
} else { // c.sd
  MMU.store_uint64(RVC_RS1S + insn.rvc_ld_imm(), std::make_pair(RVC_RS2S, 0));
}
