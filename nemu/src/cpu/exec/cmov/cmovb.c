#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "cmovb-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmovb-template.h"
#undef DATA_BYTE

make_helper_v(cmovb_rm2r)
