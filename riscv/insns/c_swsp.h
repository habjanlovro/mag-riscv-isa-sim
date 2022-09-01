require_extension('C');
MMU.store_uint32(RVC_SP + insn.rvc_swsp_imm(), std::make_pair(RVC_RS2, 0));
