global _start
_start:
[bits TARGET_BITS]
[extern main]
call main
ret
