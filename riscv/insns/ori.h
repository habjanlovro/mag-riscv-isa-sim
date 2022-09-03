// prefetch.i/r/w hint when rd = 0 and i_imm[4:0] = 0/1/3
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1);
WRITE_RD(insn.i_imm() | RS1);
