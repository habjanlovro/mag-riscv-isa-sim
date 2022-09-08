require_extension('F');
require_fp;
reg_t store_addr = RS1 + insn.s_imm();
uint32_t tag = TAG.store<uint32_t>(pc_tag, store_addr, TAG_RS1, TAG_RS2_F);
MMU.store_uint32(store_addr, std::make_pair(FRS2.v[0], tag));
