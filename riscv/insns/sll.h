TAG.propagate(pc_tag, TAG_DEST, TAG_RS1, TAG_RS2);
WRITE_RD(sext_xlen(RS1 << (RS2 & (xlen-1))));
