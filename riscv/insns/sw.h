reg_t store_addr = RS1 + insn.s_imm();
uint32_t tag = TAG.store<uint32_t>(pc_tag, store_addr, TAG_RS1, TAG_RS2);
MMU.store_uint32(store_addr, std::make_pair(RS2, tag));
