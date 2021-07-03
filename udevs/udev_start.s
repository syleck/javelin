.global _start
_start:
	pusha
	call udev_main
	popa
	ret