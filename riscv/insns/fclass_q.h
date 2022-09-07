require_extension('Q');
require_fp;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1_F);
WRITE_RD(f128_classify(f128(FRS1)));
