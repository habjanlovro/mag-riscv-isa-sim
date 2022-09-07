require_extension('C');
require(insn.rvc_rs2() != 0);
TAG.propagate(pc_tag, TAG_DEST, TAG_RS2_C);
WRITE_RD(RVC_RS2);
