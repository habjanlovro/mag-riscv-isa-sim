require_extension('Q');
require_fp;
softfloat_roundingMode = RM;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1_F, TAG_RS2_F, TAG_RS3_F, true);
WRITE_FRD(f128_mulAdd(f128(FRS1), f128(FRS2), f128(FRS3)));
set_fp_exceptions;
