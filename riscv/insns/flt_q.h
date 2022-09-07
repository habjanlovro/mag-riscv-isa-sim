require_extension('Q');
require_fp;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1_F, TAG_RS2_F);
WRITE_RD(f128_lt(f128(FRS1), f128(FRS2)));
set_fp_exceptions;
