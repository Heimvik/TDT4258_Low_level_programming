.global _start
_start:

.global _start


//Note:
//-Written by Christian Heimvik
//-Troughout the code a stanard is that r0 is used as iterator, and being compared to final value in r1
//
//How i find the if the candidate is a palindrome:
//-Before the palindrome finding itself, the initial candidate (string) goes through the following phases:
//	-Conditioning: Filter out any spaces, and convert lower case letters to upper case
//	-Flipping: Flip the order of the candidate
//-Bytewize iteration trough both conditioned and conditioned and flipped gives whether the candidate is a palindrome

_start:
	b condition_candidate

find_length:
	//Assumes:
	//-r0-r2 will be overwritten
	//-adr of string is stored in r1
	//-result will be stored in r0
	//-has to be null terminated
	mov r0, #0x0
	b find_next_byte
	find_next_byte:
		ldrb r2, [r1,r0]
		cmp r2,#0x0
		beq end_of_string
		add r0, #0x1
		b find_next_byte
	end_of_string: 
		bx lr

condition_candidate:
	ldr r10,=input //Adr to the unconditioned candidate
	mov r1, r10
	bl find_length //Finds lenth of the string by null termination byte and sets r0 to length
	mov r1,r0
	
	add r0,r0,#0x07 //Calc the worst case next adress after the initial candidate
	add r11,r10,r0 //Adress to the conditioned candidate
	mov r0, #0x0 //Holds current offset in unconditioned candidate
	mov r3, #0x0 //Holds current offset in conitioned candidate
	b condition_next_byte
	condition_next_byte:
		ldrb r2, [r10,r0]
		
		cmp r2,#97
		blt add_other //Filters small cases
		cmp r2,#122
		bgt add_other //Filters small cases
		
		b add_non_capital_letter
		add_non_capital_letter:
			sub r2, r2, #32 //Convers from lower case to upper case
			
			strb r2, [r11,r3] //Stores the lower case letters, now as upper case letters
			add r3, r3, #1
			b check_cond_finished
		add_other:
			cmp r2,#32  //Filters space
			beq check_cond_finished
			cmp r2,#0 //Enable us to jump to check_cond_finished upon detecting end of unconditioned candidate
			beq check_cond_finished
			
			strb r2, [r11,r3] //Stores the sign
			add r3, r3, #1
			b check_cond_finished
			
		check_cond_finished:
			cmp r0,r1 //We finish when we have conditioned all bytes, r1 holds length from unconditioned word from above
			beq cond_finished
			add r0,r0,#1
			b condition_next_byte //Else start on a new byte at offet r0
		cond_finished:
			mov r0,r3
			b flip_candidate //Finished, now over to the flipping of the conditioned candidate
			
flip_candidate:
	mov r1,r0
	mov r0,#0x0
	add r12,r11,r1 //Adress of flipped, conditioned candidate
	sub r2,r12,#1 //Adress of last byte in conditioned candidate
	b flip_next_byte
	flip_next_byte:
		ldrb r3, [r2,-r0] //Loads byte by byte in reverse order from the conditioned candidate at adress r11, at offset -r0 
		strb r3, [r12,r0] //Stores them in reverse order at adress r12, at offset r0
		add r0,r0,#1
		cmp r0,r1
		beq flip_finished 
		b flip_next_byte
	flip_finished:
		b check_palindrom //Conditioned candidate at r11, flipped conditioned candidate at r12		
	
check_palindrom:
	mov r1,r0
	mov r0,#0x0
	b check_next_byte
	check_next_byte:
		ldrb r3,[r11,r0] //Load byte r0 in the conditioned candidate
		ldrb r4,[r12,r0] //Load byte r0 in the flipped, conditioned candidate
		cmp r3,#0x3f
		beq check_if_finished //Ignore questionmarks (in both unflipped and flipped candidates)
		cmp r4,#0x3f
		beq check_if_finished //Ignore questionmarks (in both unflipped and flipped candidates)
		cmp r3,r4 //Bytewize comparison between the bytes
		bne is_no_palindrom
		b check_if_finished
	check_if_finished:	
		add r0,r0,#0x1
		cmp r0,r1
		beq is_palindrom //If this holds, have we reached the end withouth branch to is_no_palindrome -> We have a palindrome
		b check_next_byte
	
is_palindrom:
    ldr r1, =0xFF200000 //Light start adress        
    mov r0, #0x0              
    orr r0, r0, #0b11111 //Bitwize or with mask			     
    str r0, [r1] //Write to location at light start adress            
	
	ldr r1, =0xFF201000 //JTAG start adress
	ldr r2, =p_string
	mov r0,#0
	p_loop: //Write to location at JTAG start adress until null termination
		ldrb r3,[r2,r0]
		strb r3,[r1]
		cmp r3,#0
		add r0,r0,#1
		beq _exit
		b np_loop

is_no_palindrom:
	ldr r1, =0xFF200000 //Light start adress        
    mov r0, #0x0                 
    orr r0, r0, #0b1111100000 //Bitwize or with mask			 
    str r0, [r1] //Write to location at light start adress                 
	
	ldr r1, =0xFF201000 //JTAG start adress
	ldr r2, =np_string
	mov r0,#0
	np_loop: //Write to location at JTAG start adress until null termination
		ldrb r3,[r2,r0]
		strb r3,[r1]
		cmp r3,#0
		add r0,r0,#1
		beq _exit
		b np_loop 
	
_exit:
	b .
	
.data
np_string: .asciz "Not a palindrom"
p_string: .asciz "Palindrome detected"
.align
	input: .asciz "?ra? ned den ?arg"
.end