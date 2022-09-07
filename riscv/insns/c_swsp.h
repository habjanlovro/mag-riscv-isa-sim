require_extension('C');
uint32_t tag = TAG.store<uint32_t>(pc_tag, TAG_C_SP, TAG_RS2_C);
MMU.store_uint32(RVC_SP + insn.rvc_swsp_imm(), std::make_pair(RVC_RS2, tag));
