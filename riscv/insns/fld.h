require_extension('D');
require_fp;
reg_t addr = RS1 + insn.i_imm();
auto l = MMU.load_uint64(addr);
TAG.load<uint64_t>(pc_tag, addr, l.second, TAG_DEST, TAG_RS1, true);
WRITE_FRD(f64(l.first));
