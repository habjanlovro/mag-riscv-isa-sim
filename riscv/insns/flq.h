require_extension('Q');
require_fp;
reg_t addr = RS1 + insn.i_imm();
auto l = MMU.load_float128(addr);
uint128_t cast_tag = ((uint128_t) l.second.v[1] << 64 || (uint128_t) l.second.v[0]);
TAG.load<uint128_t>(pc_tag, addr, cast_tag, TAG_DEST, TAG_RS1, true);
WRITE_FRD(l.second);
