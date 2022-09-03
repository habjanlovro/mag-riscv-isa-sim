uint8_t tag = TAG.store<uint8_t>(pc_tag, TAG_RS1, TAG_RS2);
MMU.store_uint8(RS1 + insn.s_imm(), std::make_pair(RS2, tag));
