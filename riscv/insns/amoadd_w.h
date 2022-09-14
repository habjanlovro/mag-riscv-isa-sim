require_extension('A');
WRITE_RD(sext32(MMU.amo_uint32(RS1,
  [&](uint32_t lhs) { return lhs + RS2; },
  [&](uint32_t tags) { return TAG.amo<uint32_t>(
    pc_tag, RS1, tags, TAG_DEST, TAG_RS1, TAG_RS2); })));
