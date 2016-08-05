obj/nemu/cpu/exec/exec.o: nemu/src/cpu/exec/exec.c \
 nemu/include/cpu/helper.h nemu/include/nemu.h nemu/include/common.h \
 nemu/include/debug.h nemu/include/macro.h nemu/include/memory/memory.h \
 nemu/include/common.h nemu/include/cpu/reg.h \
 nemu/include/cpu/decode/operand.h nemu/include/cpu/decode/modrm.h \
 nemu/src/cpu/exec/all-instr.h nemu/src/cpu/exec/prefix/prefix.h \
 nemu/src/cpu/exec/other/cld.h nemu/src/cpu/exec/other/lgdt.h \
 nemu/src/cpu/exec/other/lidt.h nemu/src/cpu/exec/other/movcr2r.h \
 nemu/src/cpu/exec/other/movr2cr.h nemu/src/cpu/exec/other/movr2sreg.h \
 nemu/src/cpu/exec/other/ljmp.h nemu/src/cpu/exec/other/std.h \
 nemu/src/cpu/exec/other/sti.h nemu/src/cpu/exec/other/int.h \
 nemu/src/cpu/exec/other/pusha.h nemu/src/cpu/exec/other/cli.h \
 nemu/src/cpu/exec/other/popa.h nemu/src/cpu/exec/other/iret.h \
 nemu/src/cpu/exec/other/in.h nemu/src/cpu/exec/other/out.h \
 nemu/src/cpu/exec/other/hlt.h nemu/src/cpu/exec/other/bt.h \
 nemu/src/cpu/exec/cmov/cmovs.h nemu/src/cpu/exec/cmov/cmovp.h \
 nemu/src/cpu/exec/cmov/cmovo.h nemu/src/cpu/exec/cmov/cmovns.h \
 nemu/src/cpu/exec/cmov/cmovnp.h nemu/src/cpu/exec/cmov/cmovno.h \
 nemu/src/cpu/exec/cmov/cmove.h nemu/src/cpu/exec/cmov/cmovne.h \
 nemu/src/cpu/exec/cmov/cmova.h nemu/src/cpu/exec/cmov/cmovae.h \
 nemu/src/cpu/exec/cmov/cmovb.h nemu/src/cpu/exec/cmov/cmovbe.h \
 nemu/src/cpu/exec/cmov/cmovg.h nemu/src/cpu/exec/cmov/cmovge.h \
 nemu/src/cpu/exec/cmov/cmovl.h nemu/src/cpu/exec/cmov/cmovle.h \
 nemu/src/cpu/exec/data-mov/mov.h nemu/src/cpu/exec/data-mov/xchg.h \
 nemu/src/cpu/exec/data-mov/push.h nemu/src/cpu/exec/data-mov/pop.h \
 nemu/src/cpu/exec/data-mov/leave.h nemu/src/cpu/exec/data-mov/movzx.h \
 nemu/src/cpu/exec/data-mov/cltd.h nemu/src/cpu/exec/data-mov/movsx.h \
 nemu/src/cpu/exec/arith/dec.h nemu/src/cpu/exec/arith/inc.h \
 nemu/src/cpu/exec/arith/neg.h nemu/src/cpu/exec/arith/imul.h \
 nemu/src/cpu/exec/arith/mul.h nemu/src/cpu/exec/arith/idiv.h \
 nemu/src/cpu/exec/arith/div.h nemu/src/cpu/exec/arith/add.h \
 nemu/src/cpu/exec/arith/adc.h nemu/src/cpu/exec/arith/sub.h \
 nemu/src/cpu/exec/arith/sbb.h nemu/src/cpu/exec/logic/test.h \
 nemu/src/cpu/exec/logic/cmp.h nemu/src/cpu/exec/logic/and.h \
 nemu/src/cpu/exec/logic/or.h nemu/src/cpu/exec/logic/not.h \
 nemu/src/cpu/exec/logic/xor.h nemu/src/cpu/exec/logic/sar.h \
 nemu/src/cpu/exec/logic/shl.h nemu/src/cpu/exec/logic/shr.h \
 nemu/src/cpu/exec/logic/shrd.h nemu/src/cpu/exec/logic/setcc/seta.h \
 nemu/src/cpu/exec/logic/setcc/setne.h \
 nemu/src/cpu/exec/logic/setcc/setae.h \
 nemu/src/cpu/exec/logic/setcc/setb.h \
 nemu/src/cpu/exec/logic/setcc/setbe.h \
 nemu/src/cpu/exec/logic/setcc/sete.h \
 nemu/src/cpu/exec/logic/setcc/setg.h \
 nemu/src/cpu/exec/logic/setcc/setge.h \
 nemu/src/cpu/exec/logic/setcc/setl.h \
 nemu/src/cpu/exec/logic/setcc/setle.h \
 nemu/src/cpu/exec/logic/setcc/setno.h \
 nemu/src/cpu/exec/logic/setcc/setnp.h \
 nemu/src/cpu/exec/logic/setcc/setns.h \
 nemu/src/cpu/exec/logic/setcc/seto.h \
 nemu/src/cpu/exec/logic/setcc/setp.h \
 nemu/src/cpu/exec/logic/setcc/sets.h nemu/src/cpu/exec/control/jmp.h \
 nemu/src/cpu/exec/control/ret.h nemu/src/cpu/exec/control/jcc/jbe.h \
 nemu/src/cpu/exec/control/jcc/je.h nemu/src/cpu/exec/control/call.h \
 nemu/src/cpu/exec/control/jcc/ja.h nemu/src/cpu/exec/control/jcc/jae.h \
 nemu/src/cpu/exec/control/jcc/jb.h nemu/src/cpu/exec/control/jcc/jecxz.h \
 nemu/src/cpu/exec/control/jcc/jg.h nemu/src/cpu/exec/control/jcc/jge.h \
 nemu/src/cpu/exec/control/jcc/jl.h nemu/src/cpu/exec/control/jcc/jle.h \
 nemu/src/cpu/exec/control/jcc/jne.h nemu/src/cpu/exec/control/jcc/jno.h \
 nemu/src/cpu/exec/control/jcc/jnp.h nemu/src/cpu/exec/control/jcc/jns.h \
 nemu/src/cpu/exec/control/jcc/jo.h nemu/src/cpu/exec/control/jcc/jp.h \
 nemu/src/cpu/exec/control/jcc/js.h nemu/src/cpu/exec/string/rep.h \
 nemu/src/cpu/exec/string/movs.h nemu/src/cpu/exec/string/stos.h \
 nemu/src/cpu/exec/string/cmps.h nemu/src/cpu/exec/misc/misc.h \
 nemu/src/cpu/exec/special/special.h
