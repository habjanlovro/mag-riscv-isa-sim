reg_t tmp = npc;
reg_t jmp_addr = JUMP_TARGET;
TAG.jump(pc_tag, jmp_addr, TAG_DEST);
set_pc(jmp_addr);
WRITE_RD(tmp);
