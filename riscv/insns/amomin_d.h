require_extension('A');
require_rv64;
WRITE_RD(MMU.amo_uint64(RS1,
  [&](int64_t lhs) { return std::min(lhs, int64_t(RS2)); },
  [&](int64_t tags) { return (int64_t) TAG.amo<uint64_t>(
    pc_tag, RS1, (uint64_t) tags, TAG_DEST, TAG_RS1, TAG_RS2); }));
