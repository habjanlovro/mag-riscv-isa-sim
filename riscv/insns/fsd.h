require_extension('D');
require_fp;
MMU.store_uint64(RS1 + insn.s_imm(), std::make_pair(FRS2.v[0], 0));
