require_extension('C');
auto tag = TAG.store<uint32_t>(pc_tag, TAG_RS1_CS, TAG_RS2_CS);
MMU.store_uint32(RVC_RS1S + insn.rvc_lw_imm(), std::make_pair(RVC_RS2S, tag));
