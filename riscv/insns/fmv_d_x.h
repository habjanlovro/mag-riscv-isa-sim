require_extension('D');
require_rv64;
require_fp;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1, true);
WRITE_FRD(f64(RS1));
