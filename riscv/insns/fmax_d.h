require_extension('D');
require_fp;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1_F, TAG_RS2_F, true);
bool greater = f64_lt_quiet(f64(FRS2), f64(FRS1)) ||
               (f64_eq(f64(FRS2), f64(FRS1)) && (f64(FRS2).v & F64_SIGN));
if (isNaNF64UI(f64(FRS1).v) && isNaNF64UI(f64(FRS2).v))
  WRITE_FRD(f64(defaultNaNF64UI));
else
  WRITE_FRD(greater || isNaNF64UI(f64(FRS2).v) ? FRS1 : FRS2);
set_fp_exceptions;
