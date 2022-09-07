require_extension('C');
if (insn.rvc_rd() == 2) { // c.addi16sp
  require(insn.rvc_addi16sp_imm() != 0);
  TAG.propagate(pc_tag, X_SP, TAG_C_SP);
  WRITE_REG(X_SP, sext_xlen(RVC_SP + insn.rvc_addi16sp_imm()));
} else {
  require(insn.rvc_imm() != 0);
  TAG.propagate(pc_tag, TAG_DEST);
  WRITE_RD(insn.rvc_imm() << 12);
}
