require_extension(EXT_ZFH);
require_rv64;
require_fp;
softfloat_roundingMode = RM;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1, true);
WRITE_FRD(ui64_to_f16(RS1));
set_fp_exceptions;
