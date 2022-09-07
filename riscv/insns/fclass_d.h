require_extension('D');
require_fp;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1_F);
WRITE_RD(f64_classify(f64(FRS1)));
