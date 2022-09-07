require_extension('C');
reg_t jmp_addr = pc + insn.rvc_b_imm();
TAG.propagate_branch(pc_tag, jmp_addr, TAG_RS1_CS);
if (RVC_RS1S == 0)
  set_pc(jmp_addr);
