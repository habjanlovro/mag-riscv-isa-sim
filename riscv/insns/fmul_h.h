require_extension(EXT_ZFH);
require_fp;
softfloat_roundingMode = RM;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1_F, TAG_RS2_F, true);
WRITE_FRD(f16_mul(f16(FRS1), f16(FRS2)));
set_fp_exceptions;
