require_extension('C');
require_extension('D');
require_fp;
MMU.store_uint64(RVC_RS1S + insn.rvc_ld_imm(), std::make_pair(RVC_FRS2S.v[0], 0));
