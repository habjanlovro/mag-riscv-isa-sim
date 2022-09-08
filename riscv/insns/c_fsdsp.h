require_extension('C');
require_extension('D');
require_fp;
reg_t store_addr = RVC_SP + insn.rvc_sdsp_imm();
uint64_t tag = TAG.store<uint64_t>(pc_tag, store_addr, TAG_C_SP, TAG_RS2_FC);
MMU.store_uint64(store_addr, std::make_pair(RVC_FRS2.v[0], tag));
