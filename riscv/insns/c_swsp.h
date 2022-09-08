require_extension('C');
reg_t store_addr = RVC_SP + insn.rvc_swsp_imm();
uint32_t tag = TAG.store<uint32_t>(pc_tag, store_addr, TAG_C_SP, TAG_RS2_C);
MMU.store_uint32(store_addr, std::make_pair(RVC_RS2, tag));
