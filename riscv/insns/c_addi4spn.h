require_extension('C');
require(insn.rvc_addi4spn_imm() != 0);
TAG.propagate(pc_tag, insn.rvc_rs2s(), TAG_C_SP);
WRITE_RVC_RS2S(sext_xlen(RVC_SP + insn.rvc_addi4spn_imm()));
