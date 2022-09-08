require_extension('D');
require_fp;
reg_t store_addr = RS1 + insn.s_imm();
auto tag = TAG.store<uint64_t>(pc_tag, store_addr, TAG_RS1, TAG_RS2_F);
MMU.store_uint64(store_addr, std::make_pair(FRS2.v[0], tag));
