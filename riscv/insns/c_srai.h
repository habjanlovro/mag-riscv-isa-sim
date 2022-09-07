require_extension('C');
require(insn.rvc_zimm() < xlen);
TAG.propagate(pc_tag, insn.rvc_rs1s(), TAG_RS1_CS);
WRITE_RVC_RS1S(sext_xlen(sext_xlen(RVC_RS1S) >> insn.rvc_zimm()));
