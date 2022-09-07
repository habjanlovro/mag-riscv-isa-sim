require_extension('C');
require_extension('D');
require_fp;
uint64_t tag = TAG.store<uint64_t>(pc_tag, TAG_RS1_CS, TAG_RS2_FCS);
MMU.store_uint64(RVC_RS1S + insn.rvc_ld_imm(), std::make_pair(RVC_FRS2S.v[0], tag));
