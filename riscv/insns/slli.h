require(SHAMT < xlen);
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1);
WRITE_RD(sext_xlen(RS1 << SHAMT));
