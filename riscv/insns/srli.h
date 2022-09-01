require(SHAMT < xlen);
TAG.propagate(pc, TAG_DEST, TAG_RS1);
WRITE_RD(sext_xlen(zext_xlen(RS1) >> SHAMT));
