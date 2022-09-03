reg_t tmp = npc;
TAG.jump(pc_tag, TAG_DEST);
set_pc(JUMP_TARGET);
WRITE_RD(tmp);
