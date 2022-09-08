require_extension('C');
reg_t store_addr = RVC_RS1S + insn.rvc_lw_imm();
auto tag = TAG.store<uint32_t>(pc_tag, store_addr, TAG_RS1_CS, TAG_RS2_CS);
MMU.store_uint32(store_addr, std::make_pair(RVC_RS2S, tag));
