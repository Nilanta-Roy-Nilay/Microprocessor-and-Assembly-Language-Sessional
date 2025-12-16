                                                                                         .MODEL SMALL
.STACK 100H

.DATA
message DB 'Welcome to microprocessor and assembly language course$'

.CODE
MAIN PROC
    MOV AX, @DATA     ; Load data segment
    MOV DS, AX

    MOV AH, 09H       ; DOS function to print string
    LEA DX, message   ; Load address of message
    INT 21H           ; Call DOS interrupt to display

    MOV AH, 4CH       ; DOS terminate program
    INT 21H
MAIN ENDP
END MAIN