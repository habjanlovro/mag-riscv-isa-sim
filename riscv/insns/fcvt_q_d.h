require_extension('Q');
require_fp;
softfloat_roundingMode = RM;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1_F, true);
WRITE_FRD(f64_to_f128(f64(FRS1)));
set_fp_exceptions;
