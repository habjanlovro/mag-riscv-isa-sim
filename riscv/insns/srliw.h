require_rv64;
TAG.propagate(pc, TAG_DEST, TAG_RS1);
WRITE_RD(sext32((uint32_t)RS1 >> SHAMT));
