TAG.propagate_branch(pc, BRANCH_TARGET, TAG_RS1, TAG_RS2);
if(sreg_t(RS1) < sreg_t(RS2))
  set_pc(BRANCH_TARGET);
