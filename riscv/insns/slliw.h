require_rv64;
TAG.propagate(pc, TAG_DEST, TAG_RS1);
WRITE_RD(sext32(RS1 << SHAMT));
