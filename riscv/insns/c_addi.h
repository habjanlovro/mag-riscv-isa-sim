require_extension('C');
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1_C);
WRITE_RD(sext_xlen(RVC_RS1 + insn.rvc_imm()));
