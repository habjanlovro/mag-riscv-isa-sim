require_extension('A');

bool have_reservation = MMU.check_load_reservation(RS1, 4);

if (have_reservation)
  MMU.store_uint32(RS1, std::make_pair(RS2, 0));

MMU.yield_load_reservation();

WRITE_RD(!have_reservation);
