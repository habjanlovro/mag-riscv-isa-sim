require_rv64;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1);
WRITE_RD(sext32(int32_t(RS1) >> SHAMT));
