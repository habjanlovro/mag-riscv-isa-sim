require_extension('F');
require_fp;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1, true);
WRITE_FRD(f32(RS1));
