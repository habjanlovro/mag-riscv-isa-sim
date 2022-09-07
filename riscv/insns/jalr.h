reg_t jmp_addr = (RS1 + insn.i_imm()) & ~reg_t(1);
reg_t tmp = npc;
TAG.jump(pc_tag, jmp_addr, TAG_DEST, TAG_RS1);
set_pc(jmp_addr);
WRITE_RD(tmp);
