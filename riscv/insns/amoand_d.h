require_extension('A');
require_rv64;
WRITE_RD(MMU.amo_uint64(RS1,
  [&](uint64_t lhs) { return lhs & RS2; },
  [&](uint64_t tags) { return TAG.amo<uint64_t>(
    pc_tag, RS1, tags, TAG_DEST, TAG_RS1, TAG_RS2); }));
