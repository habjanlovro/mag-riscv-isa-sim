require_extension('C');
MMU.store_uint32(RVC_RS1S + insn.rvc_lw_imm(), std::make_pair(RVC_RS2S, 0));
