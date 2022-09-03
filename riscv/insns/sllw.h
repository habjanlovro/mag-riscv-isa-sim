require_rv64;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1, TAG_RS2);
WRITE_RD(sext32(RS1 << (RS2 & 0x1F)));
