require_extension('C');
require_extension('D');
require_fp;
uint64_t tag = TAG.store<uint64_t>(pc_tag, TAG_C_SP, TAG_RS2_FC);
MMU.store_uint64(RVC_SP + insn.rvc_sdsp_imm(), std::make_pair(RVC_FRS2.v[0], tag));
