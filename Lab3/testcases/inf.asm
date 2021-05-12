.text
	addi $t1, $zero, 5
	addi $t3, $zero, 1
stuck:
	add $a0 ,$t1, $zero
	addi $v0 ,$zero ,1
	syscall
	sub $t1, $t1, $t3
	j stuck
