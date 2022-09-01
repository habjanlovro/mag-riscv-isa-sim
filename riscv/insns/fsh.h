require_extension(EXT_ZFHMIN);
require_fp;
MMU.store_uint16(RS1 + insn.s_imm(), std::make_pair(FRS2.v[0], 0));
