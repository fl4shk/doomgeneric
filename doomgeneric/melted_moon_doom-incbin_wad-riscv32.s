.section .rodata
.global doom1_wad
.type doom1_wad, @object
.align 2

doom1_wad:
.incbin "doom1.wad"

.section .rodata
.global doom1_wad_end
.type doom1_wad_end, @object
.align 2
doom1_wad_end:

.section .rodata
.global DEMO1_lmp
.type DEMO1_lmp, @object
.align 2

DEMO1_lmp:
.incbin "DEMO1.lmp"

.section .rodata
.global DEMO1_lmp_end
.type DEMO1_lmp_end, @object
.align 2
DEMO1_lmp_end:

.section .rodata
.global DEMO2_lmp
.type DEMO2_lmp, @object
.align 2

DEMO2_lmp:
.incbin "DEMO2.lmp"

.section .rodata
.global DEMO2_lmp_end
.type DEMO2_lmp_end, @object
.align 2
DEMO2_lmp_end:

.section .rodata
.global DEMO3_lmp
.type DEMO3_lmp, @object
.align 2

DEMO3_lmp:
.incbin "DEMO3.lmp"

.section .rodata
.global DEMO3_lmp_end
.type DEMO3_lmp_end, @object
.align 2
DEMO3_lmp_end:
