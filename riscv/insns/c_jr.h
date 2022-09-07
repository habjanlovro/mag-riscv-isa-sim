require_extension('C');
require(insn.rvc_rs1() != 0);
reg_t jmp_addr = RVC_RS1 & ~reg_t(1);
TAG.propagate_branch(pc_tag, jmp_addr, TAG_RS1_C);
set_pc(jmp_addr);
