require_extension('F');
require_fp;
MMU.store_uint32(RS1 + insn.s_imm(), std::make_pair(FRS2.v[0], 0));
