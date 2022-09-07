require_extension(EXT_ZFH);
require_fp;
softfloat_roundingMode = RM;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1, true);
WRITE_FRD(ui32_to_f16((uint32_t)RS1));
set_fp_exceptions;
