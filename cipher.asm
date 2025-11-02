.DATA
	alphabet DB "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*_", 0
	alphabet_size DQ ($ - alphabet) - 1

	FindMod MACRO number1, number2
		LOCAL positive, negative
		PUSH RDX
		MOV RAX, number1
		negative:
			CMP RAX, 0						;FOR DECODE FUNCTION
			JGE positive					;I KNOW ZERO TECHNICALLY NOT POSITIVE, BUT I DON'T WANNA CALL IT not_negative
			ADD RAX, alphabet_size
			JMP negative

		positive:
			CQO
			DIV number2
			MOV RAX, RDX
			POP RDX
	ENDM

	CipherLogic MACRO direction
		;IF DIRECTION POSITIVE THEN ENCODE, IF NEGATIVE - DECODE
		;CREATE AN INT POINTER OF NUMBER OF THE LETTER INDECES
		MOV RBX, RSP

		PUSH RCX
		SUB RSP, 20h						;RESERVE 32 BITS OF A SHADOW SPACE
		CALL FindLength						;RAX - LENGTH OF THE KEY
		ADD RSP, 20h						;RETURN 32 BITS OF THE SHADOW SPACE BACK
		POP RCX

		IMUL RAX, 8							;RAX - SIZE IN BYTES OF THE KEY
		XOR RSI, RSI
		;get the int array of key indeces
		GetCharacter:
			MOVZX R8, BYTE PTR[RCX]
			INC RCX

			PUSH RAX
			PUSH RSI
			PUSH RCX

			MOV RCX, R8
			SUB RSP, 20h
			CALL GetIndexOfCharacter	
			ADD RSP, 20h
			MOV R9, RAX						;R9 - INDEX OF THE KEY CHARACTER RELATIVE TO MY ALPHABET
			
			POP RCX
			POP RSI							;GET THE RSI BACK, DUH
			POP RAX							;GET THE KEY LENTH BACK

			MOV key_indeces[RSI], R9
			ADD RSI, 8
			CMP RAX, RSI
			JNE GetCharacter

		;NOW ITERATE THROUGH THE PLAINTEXT AND MODIFY THEM WITH THE KEY INDECES WITH THE FORMULA, YOU KNOW WHICH ONE
		MOV R9, RAX							;THE SIZE OF THE KEY
		XOR RSI, RSI
		IterateThroughPlaintext:
			MOVZX RCX, BYTE PTR [RDX]
			CMP RCX, 0
			JE end_of_string

			SUB RSP, 20h
			CALL GetIndexOfCharacter
			ADD RSP, 20h

			MOV RCX, key_indeces[RSI]
			
			MOV R8, direction
			CMP R8, 0
			JL _decode
			ADD RAX, RCX
			JMP _proceed
			_decode:
				SUB RAX, RCX
			
			_proceed:
				FindMod RAX, alphabet_size
				MOV AL, alphabet[RAX]
				MOV [RDX], AL

				INC RDX
				ADD RSI, 8
				FindMod RSI, R9
				MOV RSI, RAX
				JMP IterateThroughPlaintext

		end_of_string:
			MOV RSP, RBX
	ENDM


.DATA?
	key_indeces DQ 27 DUP(?)			;For Hubert Blaine Wolfeschlegelsteinhausenbergerdorff Sr., who's no longer with us

PUBLIC Encode, Decode
.CODE

Encode PROC								;RCX - KEY, RDX - PLAINTEXT
	CipherLogic 1
		RET
Encode ENDP

Decode PROC								;RCX - DECRYPTED_TEXT, RDX - KEY
	CipherLogic -1
	RET
Decode ENDP

FindLength PROC							;RCX -	THE STRING	
	PUSH RSI
	XOR RSI, RSI
	CheckCharacter:
		MOVZX RAX, BYTE PTR[RCX]
		INC RCX
		CMP RAX, 0
		JE NullTerminaterDetected
		INC RSI
		JMP CheckCharacter
	NullTerminaterDetected:
		MOV RAX, RSI
		POP RSI
	RET	
FindLength ENDP

GetIndexOfCharacter PROC				;RCX - CHARACTER
	PUSH RSI
	XOR RSI, RSI
	L1:
		MOVZX R8, BYTE PTR alphabet[RSI]
		CMP RCX, R8
		JE Found
		INC RSI
		JMP L1
		Found:
			MOV RAX, RSI
			POP RSI
			RET
GetIndexOfCharacter ENDP

END