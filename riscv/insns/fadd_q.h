require_extension('Q');
require_fp;
softfloat_roundingMode = RM;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1_F, TAG_RS2_F, true);
WRITE_FRD(f128_add(f128(FRS1), f128(FRS2)));
set_fp_exceptions;
