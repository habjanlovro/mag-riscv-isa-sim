require_extension('C');
require_rv64;
TAG.propagate(pc_tag, insn.rvc_rs1s(), TAG_RS1_CS, TAG_RS2_CS);
WRITE_RVC_RS1S(sext32(RVC_RS1S + RVC_RS2S));
