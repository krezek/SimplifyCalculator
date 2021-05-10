include bgnt.inc

extern ASM_BigInteger_bcd_to_bin:proc

.data

.code

asm_bgnt_set_size proc
	;Prototype: void asm_bgnt_set_size(BigInteger* bg, __int64 len)

	push r8

	xor rax, rax
	mov r8, 010h

	test rdx, 0fh
	cmovnz rax, r8
	
	and rdx, 0fffffffffffffff0h
	add rdx, rax
	shr rdx, 1
	mov qword ptr[rcx + BigInteger._size], rdx

	pop r8

	ret
asm_bgnt_set_size endp

ASM_BigInteger_get_size proc
	;Prototype: int ASM_BigInteger_get_size(__int64 strLen)
	
	push rdx
	push r8

	xor rax, rax
	mov r8, 1

	mov rdx, rcx
	test rdx, 1111b
	cmovnz rax, r8
	shr rdx, 4
	add rax, rdx

	pop r8
	pop rdx
	
	ret
ASM_BigInteger_get_size endp

ASM_BigInteger_fill proc
	; Prototype void ASM_BigInteger_fill(BigInteger* bg, wchar_t* str, __int64 strLen)

	push rsi
	push rdi
	push r8
	push r10
	push r12
	push r13
	
	;fill BigInteger._value
	mov rsi, rdx
	mov rdi, qword ptr[rcx + BigInteger._value]
	mov r10, qword ptr[rcx + BigInteger._size]
	
	shl r10, 3
	shl r8, 1
LB1:
	cmp r10, 0
	je LE1
	dec r10

	xor r12w, r12w
	xor r13w, r13w
	cmp r8, 0
	je CND1
	sub r8, 2

	mov r12w, word ptr[rsi + r8]
	sub r12w, '0'

	cmp r8, 0
	je CND1
	sub r8, 2

	mov r13w, word ptr[rsi + r8]
	sub r13w, '0'

CND1:
	xor al, al
	or al, r13b
	shl al, 4
	or al, r12b

	mov byte ptr [rdi], al
	inc rdi

	jmp LB1
LE1:
	
	pop r13
	pop r12
	pop r10
	pop r8
	pop rdi
	pop rsi

	ret
ASM_BigInteger_fill endp

ASM_BigInteger_to_char proc
	; Prototype void ASM_BigInteger_to_char(__int64* value, wchar_t* str, __int64 size)

	push rsi
	push rdi
	push r8
	push r9
	
	mov rsi, rcx
	mov rdi, rdx
	shl r8, 3

LB1:
	cmp r8, 0
	je LE1

	dec r8
	
	mov r9b, byte ptr[rsi + r8]
	and r9b, 11110000b
	shr r9b, 4

	mov byte ptr[rdi + 1], 0
	add r9b, '0'
	mov byte ptr[rdi], r9b
	add rdi, 2

	mov r9b, byte ptr[rsi + r8]
	and r9b, 1111b

	mov byte ptr[rdi + 1], 0
	add r9b, '0'
	mov byte ptr[rdi], r9b
	add rdi, 2

	jmp LB1
LE1:
	mov byte ptr[rdi], 0
	mov byte ptr[rdi + 1], 0

	pop r8
	pop r9
	pop rdi
	pop rsi

	ret
ASM_BigInteger_to_char endp

end
