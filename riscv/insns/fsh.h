require_extension(EXT_ZFHMIN);
require_fp;
reg_t store_addr = RS1 + insn.s_imm();
auto tag = TAG.store<uint16_t>(pc_tag, store_addr, TAG_RS1, TAG_RS2_F);
MMU.store_uint16(store_addr, std::make_pair(FRS2.v[0], tag));
