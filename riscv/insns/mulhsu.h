require_either_extension('M', EXT_ZMMUL);
TAG.propagate(pc, TAG_DEST, TAG_RS1, TAG_RS2);
if (xlen == 64)
  WRITE_RD(mulhsu(RS1, RS2));
else
  WRITE_RD(sext32((sext32(RS1) * reg_t((uint32_t)RS2)) >> 32));
