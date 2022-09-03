require_rv64;
uint64_t tag = TAG.store<uint64_t>(pc_tag, TAG_RS1, TAG_RS2);
MMU.store_uint64(RS1 + insn.s_imm(), std::make_pair(RS2, tag));
