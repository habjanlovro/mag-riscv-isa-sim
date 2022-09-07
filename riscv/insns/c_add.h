require_extension('C');
require(insn.rvc_rs2() != 0);
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1_C, TAG_RS2_C);
WRITE_RD(sext_xlen(RVC_RS1 + RVC_RS2));
