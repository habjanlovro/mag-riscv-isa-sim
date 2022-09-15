require_extension(EXT_ZFHMIN);
require_fp;
reg_t addr = RS1 + insn.i_imm();
auto l = MMU.load_uint16(addr);
TAG.load<uint16_t>(pc_tag, addr, l.second, TAG_RS1, true);
WRITE_FRD(f16(l.first));
