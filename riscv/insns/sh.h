uint16_t tag = TAG.store<uint16_t>(pc_tag, TAG_RS1, TAG_RS2);
MMU.store_uint16(RS1 + insn.s_imm(), std::make_pair(RS2, tag));
