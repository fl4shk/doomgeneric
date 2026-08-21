.section .text.start
.align 4
.global _my_text_start
_my_text_start:
    j _start

.align 4
.global my_doom_exit
my_doom_exit:
.global doom_exit
doom_exit:
    lui a5,0xe000
    sw ra,4(a5)
    # sw a0,4(a5) # e000004
my_doom_exit_infin:
    j my_doom_exit_infin
