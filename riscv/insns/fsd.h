require_extension('D');
require_fp;
auto tag = TAG.store<uint64_t>(pc_tag, TAG_RS1, TAG_RS2_F);
MMU.store_uint64(RS1 + insn.s_imm(), std::make_pair(FRS2.v[0], tag));
