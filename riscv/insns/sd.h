require_rv64;
MMU.store_uint64(RS1 + insn.s_imm(), std::make_pair(RS2, 0));
// TAG.store(pc, TAG_RS1, TAG_RS2, addr, sizeof(uint64_t));
