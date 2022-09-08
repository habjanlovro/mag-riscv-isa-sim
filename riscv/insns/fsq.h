require_extension('Q');
require_fp;
reg_t store_addr = RS1 + insn.s_imm();
auto tag = TAG.store<uint128_t>(pc_tag, store_addr, TAG_RS1, TAG_RS2_F);
float128_t cast_tag = (float128_t){(uint64_t) (tag >> 64), (uint64_t) tag};
MMU.store_float128(store_addr, std::make_pair(FRS2, cast_tag));
