require_extension('A');
WRITE_RD(sext32(MMU.amo_uint32(RS1,
  [&](int32_t lhs) { return std::min(lhs, int32_t(RS2));},
  [&](int32_t tags) { return (int32_t) TAG.amo<uint32_t>(
    pc_tag, RS1, (uint32_t) tags, TAG_DEST, TAG_RS1, TAG_RS2); })));
