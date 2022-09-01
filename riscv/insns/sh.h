MMU.store_uint16(RS1 + insn.s_imm(), std::make_pair(RS2, 0));
// TAG.store(pc, TAG_RS1, TAG_RS2, addr, sizeof(uint16_t));
