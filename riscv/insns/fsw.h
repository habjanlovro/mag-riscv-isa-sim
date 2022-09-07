require_extension('F');
require_fp;
uint32_t tag = TAG.store<uint32_t>(pc_tag, TAG_RS1, TAG_RS2_F);
MMU.store_uint32(RS1 + insn.s_imm(), std::make_pair(FRS2.v[0], tag));
