require_extension('C');
if (xlen == 32) {
  reg_t tmp = npc;
  reg_t jmp_addr = pc + insn.rvc_j_imm();
  TAG.jump(pc_tag, jmp_addr, X_RA);
  set_pc(jmp_addr);
  WRITE_REG(X_RA, tmp);
} else { // c.addiw
  require(insn.rvc_rd() != 0);
  TAG.propagate(pc_tag, TAG_DEST, TAG_RS1_C);
  WRITE_RD(sext32(RVC_RS1 + insn.rvc_imm()));
}
