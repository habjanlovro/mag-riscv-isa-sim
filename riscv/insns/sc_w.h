require_extension('A');

bool have_reservation = MMU.check_load_reservation(RS1, 4);

if (have_reservation) {
  uint32_t tag = TAG.store<uint32_t>(pc_tag, RS1, TAG_RS1, TAG_RS2);
  MMU.store_uint32(RS1, std::make_pair(RS2, tag));
  TAG.XPR_tags.write(TAG_DEST, (uint8_t) tag);
}

MMU.yield_load_reservation();

WRITE_RD(!have_reservation);
