.data


.text
addi $t1 , $zero ,10   # $t1 = 0 +10 = 10
add $a0 ,$t1, $zero
addi $v0 ,$zero ,1
syscall



addi $v0 ,$zero ,10  #program ends
syscall
