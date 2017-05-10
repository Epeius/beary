loc_805147D:
	mov      eax, [ebp+i] 
	mov      edx, [ebp+buf]
	add      eax, edx
	movzx    eax, byte ptr [eax]
	movzx    eax, al
	movzx    eax, text_chars[eax]
	movsx    eax, al
	mov      [ebp+t], eax
	cmp      [ebp+t], 1
	jz       short loc_80514A5
	; do other jobs

