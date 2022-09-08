require_extension('C');
require_extension('D');
require_fp;
reg_t store_addr = RVC_RS1S + insn.rvc_ld_imm();
uint64_t tag = TAG.store<uint64_t>(pc_tag, store_addr, TAG_RS1_CS, TAG_RS2_FCS);
MMU.store_uint64(store_addr, std::make_pair(RVC_FRS2S.v[0], tag));
