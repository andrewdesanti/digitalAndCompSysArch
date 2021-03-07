#LAB1 FROM CPE517 SPRING 2021
#We pledge our honor that we have abided by the Stevens Honor System:
# -Andrew DeSanti, Joshua Hornilla

.data
str1: .asciiz "hello"	#strings to test#1
str2: .asciiz "zxhcLkk"	#strings to test#2
.text
	main: 
			la $a0, str1		#load the address of string1
			la $a1, str2		#load the address of string2
			jal firstmatch	
		
			beqz $v1, mainexit	#if there is no match, jump to end and print nothing!
			li $v0, 11		#syscall to print a character
			lb $a0, ($v1)		#load character to be printed into $a0
			syscall			#actually print using syscall
		mainexit:
			li $v0, 10 		#syscall to end a program
			syscall			#actually end the program!
		
		
	
	firstmatch:
			addi $sp, $sp, -4	#keeping track of stack pointer...
			sw $ra, 0($sp)		#fill in the 4 new missing bits on the stack
		search2:
			lb $t6, ($a0)		#load the address to str1 in t6
			beqz $t6, zero		#if t6 is zero, jump to zero
			jal strchr		#jump and link to strchr
			addi $a0, $a0, 1	#increment the pointer to str1 by 1
			bnez $v1, final		#if v1 is not equal to zero after a particular strchr call, it is the match!
			j search2		#jump back to search2 to pass through again...
		zero:
           		li $v1, 0 		#there was no match so load zero into v1 to ensure nothing is printed
           		lw $ra, 0($sp)		#fiz the stack pointer
			addi $sp, $sp, 4
			jr $ra 			#jump back to main!					
		final:	
			lw $ra, 0($sp)		#fix the stack pointer
			addi $sp, $sp, 4	
			jr $ra			#jump to ra to exit the function and return to main

					
	strchr:
			
			addi $sp, $sp, -4 	#keeping track of stack pointer
			sw $ra, 0($sp)		#fill in new bits on the stack
			lb $t1, ($a0)		#load character of str1 into t1
			la $t3, ($a1)		#load address if character of str2 into t3
		search1:	
			lb $t2, ($t3)		#load the character of str2 from t3 into t2
			beq $t2, $zero, notfound#if the character from str2 is zero it is the end string and the function can end
			beq $t2, $t1, found	#if the character from str2 matches from str1 the function can end
			addi $t3, $t3, 1	#increment the address of str2
			j search1		#jump to search1, redo the test
		found:				
			la $v1, ($t3)		#if found, load the address of the matched letter to v1
			j end			#jump to end
		notfound:
			li $v1, 0		#if not found load a zero into v1
			j end			#jump to end
		end:
			lw $ra, 0($sp)		#fix stack pointer
			addi $sp, $sp, 4	
			jr $ra			#jump to ra to exit the function and return to firstmatch
