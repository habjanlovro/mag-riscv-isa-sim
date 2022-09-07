require_extension('D');
require_fp;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1_F, TAG_RS2_F);
WRITE_RD(f64_lt(f64(FRS1), f64(FRS2)));
set_fp_exceptions;
