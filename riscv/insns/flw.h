require_extension('F');
require_fp;
reg_t addr = RS1 + insn.i_imm();
auto l = MMU.load_uint32(addr);
TAG.load<uint32_t>(pc_tag, addr, l.second, TAG_DEST, TAG_RS1, true);
WRITE_FRD(f32(l.first));
