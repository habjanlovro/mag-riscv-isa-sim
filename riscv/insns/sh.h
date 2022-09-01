reg_t addr = RS1 + insn.s_imm();
MMU.store_uint16(addr, RS2);
TAG.store(pc, TAG_RS1, TAG_RS2, addr, sizeof(uint16_t));
