require_extension(EXT_ZFHMIN);
require_fp;
auto tag = TAG.store<uint16_t>(pc_tag, TAG_RS1, TAG_RS2_F);
MMU.store_uint16(RS1 + insn.s_imm(), std::make_pair(FRS2.v[0], tag));
