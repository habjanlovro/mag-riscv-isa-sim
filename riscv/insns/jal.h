reg_t tmp = npc;
TAG.jump(pc, TAG_DEST, JUMP_TARGET);
set_pc(JUMP_TARGET);
WRITE_RD(tmp);
