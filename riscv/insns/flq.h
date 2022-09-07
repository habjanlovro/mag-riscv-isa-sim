require_extension('Q');
require_fp;
auto l = MMU.load_float128(RS1 + insn.i_imm());
uint128_t cast_tag = ((uint128_t) l.second.v[1] << 64 || (uint128_t) l.second.v[0]);
TAG.load<uint128_t>(pc_tag, cast_tag, TAG_DEST, TAG_RS1, true);
WRITE_FRD(l.second);
