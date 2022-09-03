uint32_t tag = TAG.store<uint32_t>(pc_tag, TAG_RS1, TAG_RS2);
MMU.store_uint32(RS1 + insn.s_imm(), std::make_pair(RS2, tag));
