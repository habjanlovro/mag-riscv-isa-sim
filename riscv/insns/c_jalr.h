require_extension('C');
require(insn.rvc_rs1() != 0);
reg_t tmp = npc;
reg_t jmp_addr = RVC_RS1 & ~reg_t(1);
TAG.jump(pc_tag, jmp_addr, X_RA, TAG_RS1_C);
set_pc(jmp_addr);
WRITE_REG(X_RA, tmp);
