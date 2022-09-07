require_extension('F');
require_fp;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1_F);
WRITE_RD(f32_classify(f32(FRS1)));
