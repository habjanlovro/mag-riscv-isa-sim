require_extension('C');
reg_t jmp_addr = pc + insn.rvc_j_imm();
TAG.propagate_branch(pc_tag, jmp_addr);
set_pc(jmp_addr);
