;For direction selection algorithm we used a flag called ""direction_algorithm"" instead of timer0 bits,
;Since timer0 bit always gave similar last two bits  because prescaler value is 1:64 and it doesn increment in 64 cycle
;We did some xor and increment opretions in ISR to randomize direction behaviour.
LIST P=18F8722
#include <p18f8722.inc>
CONFIG OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF,MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

UDATA_ACS
    score1 res 1
    score2 res 1
    t1 res 1
    t2 res 1
    t3 res 1
    d res  1
    e res 1
    f res 1
    direction_algorithm res 1
    permission_flag res 1
    direction res 1;if 0 left otherwise (if 1) right
w_temp  udata 0x23
w_temp

status_temp udata 0x24
status_temp

pclath_temp udata 0x25
pclath_temp

counter   udata 0x22
counter



ORG 0x00
GOTO INIT

ORG 0x08
GOTO ISR




WAIT:
    MOVLW 0x22	; Copy desired value to W
    MOVWF d	; Copy W into t3
    _loop4:
	MOVLW 0x20  ; Copy desired value to W
	MOVWF e    ; Copy W into t2
	_loop5:
	    MOVLW 0x2F	; Copy desired value to W
	    MOVWF f	; Copy W into t1
	    _loop6:
		decfsz f,F ; Decrement t1. If 0 Skip next instruction
		GOTO _loop6 ; ELSE Keep counting down
		decfsz e,F ; Decrement t2. If 0 Skip next instruction
		GOTO _loop5 ; ELSE Keep counting down
		decfsz d,F ; Decrement t3. If 0 Skip next instruction
		GOTO _loop4 ; ELSE Keep counting down
		return

DELAY:	; Time Delay Routine with 3 nested loops
    
    MOVLW 0x82	; Copy desired value to W
    MOVWF t3	; Copy W into t3
    _loop3:
	MOVLW 0xA0  ; Copy desired value to W
	MOVWF t2    ; Copy W into t2
	_loop2:
	    MOVLW 0x9F	; Copy desired value to W
	    MOVWF t1	; Copy W into t1
	    _loop1:
		decfsz t1,F ; Decrement t1. If 0 Skip next instruction
		GOTO _loop1 ; ELSE Keep counting down
		decfsz t2,F ; Decrement t2. If 0 Skip next instruction
		GOTO _loop2 ; ELSE Keep counting down
		decfsz t3,F ; Decrement t3. If 0 Skip next instruction
		GOTO _loop3 ; ELSE Keep counting down
		return


INIT:

    CLRF permission_flag
    CLRF direction_algorithm

;Make direction left
    CLRF direction
    

    ;Zero scores
    CLRF score1
    CLRF score2

    ;Configure output ports
    MOVLW b'00111111'
    MOVWF ADCON1
    CLRF TRISA
    CLRF TRISB
    CLRF TRISC
    CLRF TRISD
    CLRF TRISE
    CLRF TRISF
    CLRF TRISH
    CLRF TRISJ

    ;Put input mode RG0-RG4
    MOVLW b'00001111'
    MOVWF TRISG
                  

    ;Disable all interrupts
    CLRF    INTCON
    CLRF    INTCON2
    CLRF    INTCON3

    ;Enable interrupts
    movlw   b'11100000' ;Enable Global, peripheral, Timer0  interrupts by setting GIE, PEIE, TMR0IE bits to 1
    movwf   INTCON

   

    ;Initialize Timer0
    movlw   b'11000101' ;Enable Timer0 by setting TMR0ON to 0 (for now)
                        ;Configure Timer0 as an 8-bit timer/counter by setting T08BIT to 0
                        ;Timer0 increment from internal clock with a prescaler of 1:64.
    movwf   T0CON ; T0CON = b'10000101'
    GOTO START

START:
    CLRF score1
    CLRF score2
    MOVLW b'00111111'
    MOVWF LATA
    MOVWF LATB
    MOVWF LATC
    MOVWF LATD
    MOVWF LATE
    MOVWF LATF
    CLRF  LATG
    MOVLW b'00001010'
    MOVWF PORTH
    MOVLW b'00111111'
    MOVWF PORTJ
    CALL DELAY
    GOTO MATCH_START

MATCH_START:
    MOVLW b'00011100'
    MOVWF LATA
    MOVWF LATF
    MOVLW b'00001000'
    MOVWF LATD
    CLRF LATB
    CLRF LATC
    CLRF LATE
    MOVLW d'31' ; initialize timer0
    MOVWF TMR0
    CLRF counter
    GOTO BALL_RD3


ISR:
    call    save_registers          ;Save current content of STATUS and PCLATH registers to be able to restore them later
    incf	counter, f              ;Timer interrupt handler part begins here by incrementing count variable
    incf    direction_algorithm,F   ; Flag for direction algorithm instead of using Timer0!!
    movf	counter, w              ;Move count to Working register
    sublw	d'208'                  ;Decrement 208 from Working register
    btfss	STATUS, Z               ;Is the result Zero?
    goto	timer_interrupt_exit    ;No, then exit from interrupt service routine
    clrf	counter                 ;Yes, then clear count variable
    bsf     permission_flag,0
    ;comf    LATG,1                 ;TESTING timer interrupt (BERKANT AND ILKER - 07/04/2018)
    BTFSS   direction,0
    MOVLW   b'00000011'
    MOVLW   b'00000010'
    XORWF   direction_algorithm,F   ;Xor with direction bith for more randmized direction algorithm!!!

timer_interrupt_exit:
    bcf     INTCON, 2           ;Clear TMROIF
	movlw	d'31'               ;31
	movwf	TMR0
	call	restore_registers   ;Restore STATUS and PCLATH registers to their state before interrupt occurs
	retfie

save_registers:
    movwf 	w_temp          ;Copy W to TEMP register
    swapf 	STATUS, w       ;Swap status to be saved into W
    clrf 	STATUS          ;bank 0, regardless of current bank, Clears IRP,RP1,RP0
    movwf 	status_temp     ;Save status to bank zero STATUS_TEMP register
    movf 	PCLATH, w       ;Only required if using pages 1, 2 and/or 3
    movwf 	pclath_temp     ;Save PCLATH into W
    clrf 	PCLATH          ;Page zero, regardless of current page
    return

restore_registers:
    movf 	pclath_temp, w  ;Restore PCLATH
    movwf 	PCLATH          ;Move W into PCLATH
    swapf 	status_temp, w  ;Swap STATUS_TEMP register into W
    movwf 	STATUS          ;Move W into STATUS register
    swapf 	w_temp, f       ;Swap W_TEMP
    swapf 	w_temp, w       ;Swap W_TEMP into W
    return




display_1:
    call Player1_INCR
    return
display_2:
    call Player2_INCR
    return

display_01:
    MOVLW b'00000010'
    MOVWF PORTH
    MOVLW b'00111111'
    MOVWF PORTJ
    return
display_11:
    MOVLW b'00000010'
    MOVWF PORTH
    MOVLW b'00000110'
    MOVWF PORTJ
    return
display_21:
    MOVLW b'00000010'
    MOVWF PORTH
    MOVLW b'01011011'
    MOVWF PORTJ
    return
display_31:
    MOVLW b'00000010'
    MOVWF PORTH
    MOVLW b'01001111'
    MOVWF PORTJ
    return
display_41:
    MOVLW b'00000010'
    MOVWF PORTH
    MOVLW b'01100110'
    MOVWF PORTJ
    return
display_51:
    MOVLW b'00000010'
    MOVWF PORTH
    MOVLW b'01101101'
    MOVWF PORTJ
    return
display_02:
    MOVLW b'00001000'
    MOVWF PORTH
    MOVLW b'00111111'
    MOVWF PORTJ
    return
display_12:
    MOVLW b'00001000'
    MOVWF PORTH
    MOVLW b'00000110'
    MOVWF PORTJ
    return
display_22:
    MOVLW b'00001000'
    MOVWF PORTH
    MOVLW b'01011011'
    MOVWF PORTJ
    return
display_32:
    MOVLW b'00001000'
    MOVWF PORTH
    MOVLW b'01001111'
    MOVWF PORTJ
    return
display_42:
    MOVLW b'00001000'
    MOVWF PORTH
    MOVLW b'01100110'
    MOVWF PORTJ
    return
display_52:
    MOVLW b'00001000'
    MOVWF PORTH
    MOVLW b'01101101'
    MOVWF PORTJ
    return

Player1_INCR:
    MOVLW 0x0
    CPFSGT score1
    GOTO goal00
    GOTO goal01
    goal00:
        call display_01
        return
    goal01:
        MOVLW  0x1
        CPFSGT score1
        goto goal10
        goto goal11
        goal10:
            call display_11
            return
        goal11:
            MOVLW 0x2
            CPFSGT score1
            goto goal20
            goto goal21
            goal20:
                call display_21
                return
            goal21:
                MOVLW 0x3
                CPFSGT score1
                goto goal30
                goto goal31
                goal30:
                    call display_31
                    return
                goal31:
                    MOVLW 0x4
                    CPFSGT score1
                    goto goal40
                    goto goal41
                    goal40:
                        call display_41
                        return
                    goal41:
                        call display_51
                        call display_2
                        CLRF INTCON        ;disable interrupts
                        goto goal41

Player2_INCR:
    MOVLW 0x0
    CPFSGT score2
    GOTO _goal00
    GOTO _goal01
    _goal00:
        call display_02
        return
    _goal01:
        MOVLW  0x1
        CPFSGT score2
        goto _goal10
        goto _goal11
        _goal10:
            call display_12
            return
        _goal11:
            MOVLW 0x2
            CPFSGT score2
            goto _goal20
            goto _goal21
            _goal20:
                call display_22
                return
            _goal21:
                MOVLW 0x3
                CPFSGT score2
                goto _goal30
                goto _goal31
                _goal30:
                    call display_32
                    return
                _goal31:
                    MOVLW 0x4
                    CPFSGT score2
                    goto _goal40
                    goto _goal41
                    _goal40:
                        call display_42
                        return
                    _goal41:
                        call display_52
                        call display_1
                        CLRF INTCON  ;disable interrupts
                        goto _goal41

BALL_RA0:
    call CLEAR
    BSF LATA,0
    bcf permission_flag,0
    incf score2
    check_ra0:
        call display_1
        call display_2
        btfss permission_flag,0
        GOTO check_ra0
    goto MATCH_START

BALL_RA1:
    call CLEAR
    BSF LATA,1
    bcf permission_flag,0
    incf score2
    check_ra1:
        call display_1
        call display_2
        btfss permission_flag,0
        GOTO check_ra1
    goto MATCH_START
BALL_RA2:
    call CLEAR
    BSF LATA,2
    bcf permission_flag,0
    incf score2
    check_ra2:
        call display_2
        call display_1
        btfss permission_flag,0
        GOTO check_ra2
    goto MATCH_START
BALL_RA3:
    call CLEAR
    BSF LATA,3
    bcf permission_flag,0
    incf score2
    check_ra3:
        call display_2
        call display_1
        btfss permission_flag,0
        GOTO check_ra3
    goto MATCH_START
BALL_RA4:
    call CLEAR
    BSF LATA,4
    bcf permission_flag,0
    incf score2
    check_ra4:
        call display_2
        call display_1
        btfss permission_flag,0
        GOTO check_ra4
    goto MATCH_START
BALL_RA5:
    call CLEAR
    BSF LATA,5
    bcf permission_flag,0
    incf score2
    check_ra5:
        call display_2
        call display_1
        btfss permission_flag,0
        GOTO check_ra5
    goto MATCH_START

BALL_RF0:
    call CLEAR
    BSF LATF,0
    bcf permission_flag,0
    incf score1
    check_rf0:
        call display_2
        call display_1
        btfss permission_flag,0
        GOTO check_rf0
    goto MATCH_START
BALL_RF1:
    call CLEAR
    BSF LATF,1
    bcf permission_flag,0
    incf score1
    check_rf1:
        call display_2
        call display_1
        btfss permission_flag,0
        GOTO check_rf1
    goto MATCH_START
BALL_RF2:
    call CLEAR
    BSF LATF,2
    bcf permission_flag,0
    incf score1
    check_rf2:
        call display_2
        call display_1
        btfss permission_flag,0
        GOTO check_rf2
    goto MATCH_START
BALL_RF3:
    call CLEAR
    BSF LATF,3
    bcf permission_flag,0
    incf score1
    check_rf3:
        call display_2
        call display_1
        btfss permission_flag,0
        GOTO check_rf3
    goto MATCH_START
BALL_RF4:
    call CLEAR
    BSF LATF,4
    bcf permission_flag,0
    incf score1
    check_rf4:
        call display_2
        call display_1
        btfss permission_flag,0
        GOTO check_rf4
    goto MATCH_START
BALL_RF5:
    call CLEAR
    BSF LATF,5
    bcf permission_flag,0
    incf score1
    check_rf5:
        call display_2
        call display_1
        btfss permission_flag,0
        GOTO check_rf5
    goto MATCH_START

BALL_RB0:
    call CLEAR
    MOVLW b'00000001'
    MOVWF LATB
    bcf permission_flag,0
    check_rb0:
        call display_2
        call display_1
        call BUTTON_TASK_RIGHT_DOWN
        call BUTTON_TASK_RIGHT_UP
        call BUTTON_TASK_LEFT_DOWN
        call BUTTON_TASK_LEFT_UP
        btfss permission_flag,0
        goto check_rb0
    BTFSC direction,0
    GOTO right_b0
    GOTO left_b0
        right_b0:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_right_b0
            GOTO up_or_straight_right_b0
            down_or_straight_right_b0:
                BTFSS direction_algorithm,1
                GOTO BALL_RC1
                GOTO BALL_RC0
            up_or_straight_right_b0:
                BTFSS direction_algorithm,1
                GOTO BALL_RC1
                GOTO BALL_RC0
        left_b0:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_left_b0
            GOTO up_or_straight_left_b0
            down_or_straight_left_b0:
                BTFSS direction_algorithm,1
                GOTO control_down_left_b0
                GOTO control_straight_left_b0
                control_down_left_b0:
                    BTFSS LATA,1
                    GOTO BALL_RA1
                    COMF direction,f
                    GOTO BALL_RB0
                control_straight_left_b0:
                    BTFSS LATA,0
                    GOTO BALL_RA0
                    COMF direction,f
                    GOTO BALL_RB0
            up_or_straight_left_b0:
                BTFSS direction_algorithm,1
                GOTO control_up_left_b0
                GOTO control_straight_left_b0
                control_up_left_b0:
                    BTFSS LATA,1
                    GOTO BALL_RA1
                    COMF direction,f
                    GOTO BALL_RB0
BALL_RB1:
    call CLEAR
    MOVLW b'00000010'
    MOVWF LATB
    bcf permission_flag,0
    check_rb1:
        call display_2
        call display_1
        call BUTTON_TASK_RIGHT_DOWN
        call BUTTON_TASK_RIGHT_UP
        call BUTTON_TASK_LEFT_DOWN
        call BUTTON_TASK_LEFT_UP
        btfss permission_flag,0
        goto check_rb1
    BTFSC direction,0
    GOTO right_b1
    GOTO left_b1
        right_b1:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_right_b1
            GOTO up_or_straight_right_b1
            down_or_straight_right_b1:
                BTFSS direction_algorithm,1
                GOTO BALL_RC2
                GOTO BALL_RC1
            up_or_straight_right_b1:
                BTFSS direction_algorithm,1
                GOTO BALL_RC0
                GOTO BALL_RC1
        left_b1:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_left_b1
            GOTO up_or_straight_left_b1
            down_or_straight_left_b1:
                BTFSS direction_algorithm,1
                GOTO control_down_left_b1
                GOTO control_straight_left_b1
                control_down_left_b1:
                    BTFSS LATA,2
                    GOTO BALL_RA2
                    COMF direction,f
                    GOTO BALL_RB1
                control_straight_left_b1:
                    BTFSS LATA,1
                    GOTO BALL_RA1
                    COMF direction,f
                    GOTO BALL_RB1
            up_or_straight_left_b1:
                BTFSS direction_algorithm,1
                GOTO control_up_left_b1
                GOTO control_straight_left_b1
                control_up_left_b1:
                    BTFSS LATA,0
                    GOTO BALL_RA0
                    COMF direction,f
                    GOTO BALL_RB1
BALL_RB2:

    call CLEAR
    MOVLW b'00000100'
    MOVWF LATB
    bcf permission_flag,0
    check_rb2:
        call display_1
        call display_2
        call BUTTON_TASK_RIGHT_DOWN
        call BUTTON_TASK_RIGHT_UP
        call BUTTON_TASK_LEFT_DOWN
        call BUTTON_TASK_LEFT_UP
        btfss permission_flag,0
        goto check_rb2
    BTFSC direction,0
    GOTO right_b2
    GOTO left_b2
        right_b2:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_right_b2
            GOTO up_or_straight_right_b2
            down_or_straight_right_b2:
                BTFSS direction_algorithm,1
                GOTO BALL_RC3
                GOTO BALL_RC2
            up_or_straight_right_b2:
                BTFSS direction_algorithm,1
                GOTO BALL_RC1
                GOTO BALL_RC2
        left_b2:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_left_b2
            GOTO up_or_straight_left_b2
            down_or_straight_left_b2:
                BTFSS TMR0,1
                GOTO control_down_left_b2
                GOTO control_straight_left_b2
                control_down_left_b2:
                    BTFSS LATA,3
                    GOTO BALL_RA3
                    COMF direction,f
                    GOTO BALL_RB2
                control_straight_left_b2:
                    BTFSS LATA,2
                    GOTO BALL_RA2
                    COMF direction,f
                    GOTO BALL_RB2
            up_or_straight_left_b2:
                BTFSS direction_algorithm,1
                GOTO control_up_left_b2
                GOTO control_straight_left_b2
                control_up_left_b2:
                    BTFSS LATA,1
                    GOTO BALL_RA1
                    COMF direction,f
                    GOTO BALL_RB2
BALL_RB3:

    call CLEAR
    MOVLW b'00001000'
    MOVWF LATB
    bcf permission_flag,0
    check_rb3:
        call display_1
        call display_2
        call BUTTON_TASK_RIGHT_DOWN
        call BUTTON_TASK_RIGHT_UP
        call BUTTON_TASK_LEFT_DOWN
        call BUTTON_TASK_LEFT_UP
        btfss permission_flag,0
        goto check_rb3
    BTFSC direction,0
    GOTO right_b3
    GOTO left_b3
        right_b3:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_right_b3
            GOTO up_or_straight_right_b3
            down_or_straight_right_b3:
                BTFSS direction_algorithm,1
                GOTO BALL_RC4
                GOTO BALL_RC3
            up_or_straight_right_b3:
                BTFSS direction_algorithm,1
                GOTO BALL_RC2
                GOTO BALL_RC3
        left_b3:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_left_b3
            GOTO up_or_straight_left_b3
            down_or_straight_left_b3:
                BTFSS direction_algorithm,1
                GOTO control_down_left_b3
                GOTO control_straight_left_b3
                control_down_left_b3:
                    BTFSS LATA,4
                    GOTO BALL_RA4
                    COMF direction,f
                    GOTO BALL_RB3
                control_straight_left_b3:
                    BTFSS LATA,3
                    GOTO BALL_RA3
                    COMF direction,f
                    GOTO BALL_RB3
            up_or_straight_left_b3:
                BTFSS direction_algorithm,1
                GOTO control_up_left_b3
                GOTO control_straight_left_b3
                control_up_left_b3:
                    BTFSS LATA,2
                    GOTO BALL_RA2
                    COMF direction,f
                    GOTO BALL_RB3
BALL_RB4:

    call CLEAR
    MOVLW b'00010000'
    MOVWF LATB
    bcf permission_flag,0
    check_rb4:
        call display_1
        call display_2
        call BUTTON_TASK_RIGHT_DOWN
        call BUTTON_TASK_RIGHT_UP
        call BUTTON_TASK_LEFT_DOWN
        call BUTTON_TASK_LEFT_UP
        btfss permission_flag,0
        goto check_rb4
    BTFSC direction,0
    GOTO right_b4
    GOTO left_b4
        right_b4:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_right_b4
            GOTO up_or_straight_right_b4
            down_or_straight_right_b4:
                BTFSS direction_algorithm,1
                GOTO BALL_RC5
                GOTO BALL_RC4
            up_or_straight_right_b4:
                BTFSS direction_algorithm,1
                GOTO BALL_RC3
                GOTO BALL_RC4
        left_b4:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_left_b4
            GOTO up_or_straight_left_b4
            down_or_straight_left_b4:
                BTFSS direction_algorithm,1
                GOTO control_down_left_b4
                GOTO control_straight_left_b4
                control_down_left_b4:
                    BTFSS LATA,5
                    GOTO BALL_RA5
                    COMF direction,f
                    GOTO BALL_RB4
                control_straight_left_b4:
                    BTFSS LATA,4
                    GOTO BALL_RA4
                    COMF direction,f
                    GOTO BALL_RB4
            up_or_straight_left_b4:
                BTFSS direction_algorithm,1
                GOTO control_up_left_b4
                GOTO control_straight_left_b4
                control_up_left_b4:
                    BTFSS LATA,3
                    GOTO BALL_RA3
                    COMF direction,f
                    GOTO BALL_RB4
BALL_RB5:
     
    call CLEAR
    MOVLW b'00100000'
    MOVWF LATB
    bcf permission_flag,0
    check_rb5:
        call display_1
        call display_2
        call BUTTON_TASK_RIGHT_DOWN
        call BUTTON_TASK_RIGHT_UP
        call BUTTON_TASK_LEFT_DOWN
        call BUTTON_TASK_LEFT_UP
        btfss permission_flag,0
        goto check_rb5
    BTFSC direction,0
    GOTO right_b5
    GOTO left_b5
        right_b5:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_right_b5
            GOTO up_or_straight_right_b5
            down_or_straight_right_b5:
                BTFSS direction_algorithm,1
                GOTO BALL_RC4
                GOTO BALL_RC5
            up_or_straight_right_b5:
                BTFSS direction_algorithm,1
                GOTO BALL_RC4
                GOTO BALL_RC5
        left_b5:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_left_b5
            GOTO up_or_straight_left_b5
            down_or_straight_left_b5:
                BTFSS direction_algorithm,1
                GOTO control_down_left_b5
                GOTO control_straight_left_b5
                control_down_left_b5:
                    BTFSS LATA,4
                    GOTO BALL_RA4
                    COMF direction,f
                    GOTO BALL_RB5
                control_straight_left_b5:
                    BTFSS LATA,5
                    GOTO BALL_RA5
                    COMF direction,f
                    GOTO BALL_RB5
            up_or_straight_left_b5:
                BTFSS direction_algorithm,1
                GOTO control_up_left_b5
                GOTO control_straight_left_b5
                control_up_left_b5:
                    BTFSS LATA,4
                    GOTO BALL_RA4
                    COMF direction,f
                    GOTO BALL_RB5
BALL_RC0:
     
    call CLEAR
    MOVLW b'00000001'
    MOVWF LATC
    bcf permission_flag,0
    check_rc0:
        call display_1
        call display_2
        call BUTTON_TASK_RIGHT_DOWN
        call BUTTON_TASK_RIGHT_UP
        call BUTTON_TASK_LEFT_DOWN
        call BUTTON_TASK_LEFT_UP
        btfss permission_flag,0
        goto check_rc0
    BTFSC direction,0
    GOTO right_c0
    GOTO left_c0
        right_c0:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_right_c0
            GOTO up_or_straight_right_c0
            down_or_straight_right_c0:
                BTFSS direction_algorithm,1
                GOTO BALL_RD1
                GOTO BALL_RD0
            up_or_straight_right_c0:
                BTFSS direction_algorithm,1
                GOTO BALL_RD1
                GOTO BALL_RD0
        left_c0:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_left_c0
            GOTO up_or_straight_left_c0
            down_or_straight_left_c0:
                BTFSS direction_algorithm,1
                GOTO BALL_RB1
                GOTO BALL_RB0
            up_or_straight_left_c0:
                BTFSS direction_algorithm,1
                GOTO BALL_RB1
                GOTO BALL_RB0
BALL_RC1:
   
    call CLEAR
    MOVLW b'00000010'
    MOVWF LATC
    bcf permission_flag,0
    check_rc1:
        call display_1
        call display_2
        call BUTTON_TASK_RIGHT_DOWN
        call BUTTON_TASK_RIGHT_UP
        call BUTTON_TASK_LEFT_DOWN
        call BUTTON_TASK_LEFT_UP
        btfss permission_flag,0
        goto check_rc1
    BTFSC direction,0
    GOTO right_c1
    GOTO left_c1
        right_c1:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_right_c1
            GOTO up_or_straight_right_c1
            down_or_straight_right_c1:
                BTFSS direction_algorithm,1
                GOTO BALL_RD2
                GOTO BALL_RD1
            up_or_straight_right_c1:
                BTFSS direction_algorithm,1
                GOTO BALL_RD0
                GOTO BALL_RD1
        left_c1:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_left_c1
            GOTO up_or_straight_left_c1
            down_or_straight_left_c1:
                BTFSS direction_algorithm,1
                GOTO BALL_RB2
                GOTO BALL_RB1
            up_or_straight_left_c1:
                BTFSS direction_algorithm,1
                GOTO BALL_RB0
                GOTO BALL_RB1
BALL_RC2:
   
    call CLEAR
    MOVLW b'00000100'
    MOVWF LATC
    bcf permission_flag,0
    check_rc2:
        call display_1
        call display_2
        call BUTTON_TASK_RIGHT_DOWN
        call BUTTON_TASK_RIGHT_UP
        call BUTTON_TASK_LEFT_DOWN
        call BUTTON_TASK_LEFT_UP
        btfss permission_flag,0
        goto check_rc2
    BTFSC direction,0
    GOTO right_c2
    GOTO left_c2
        right_c2:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_right_c2
            GOTO up_or_straight_right_c2
            down_or_straight_right_c2:
                BTFSS direction_algorithm,1
                GOTO BALL_RD3
                GOTO BALL_RD2
            up_or_straight_right_c2:
                BTFSS direction_algorithm,1
                GOTO BALL_RD1
                GOTO BALL_RD2
        left_c2:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_left_c2
            GOTO up_or_straight_left_c2
            down_or_straight_left_c2:
                BTFSS direction_algorithm,1
                GOTO BALL_RB3
                GOTO BALL_RB2
            up_or_straight_left_c2:
                BTFSS direction_algorithm,1
                GOTO BALL_RB1
                GOTO BALL_RB2
    
BALL_RC3:
   
    call CLEAR
    MOVLW b'00001000'
    MOVWF LATC
    bcf permission_flag,0
    check_rc3:
        call display_1
        call display_2
        call BUTTON_TASK_RIGHT_DOWN
        call BUTTON_TASK_RIGHT_UP
        call BUTTON_TASK_LEFT_DOWN
        call BUTTON_TASK_LEFT_UP
        btfss permission_flag,0
        goto check_rc3
    BTFSC direction,0
    GOTO right_c3
    GOTO left_c3
        right_c3:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_right_c3
            GOTO up_or_straight_right_c3
            down_or_straight_right_c3:
                BTFSS direction_algorithm,1
                GOTO BALL_RD4
                GOTO BALL_RD3
            up_or_straight_right_c3:
                BTFSS direction_algorithm,1
                GOTO BALL_RD2
                GOTO BALL_RD3
        left_c3:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_left_c3
            GOTO up_or_straight_left_c3
            down_or_straight_left_c3:
                BTFSS direction_algorithm,1
                GOTO BALL_RB4
                GOTO BALL_RB3
            up_or_straight_left_c3:
                BTFSS direction_algorithm,1
                GOTO BALL_RB2
                GOTO BALL_RB3
    
BALL_RC4:
    
    call CLEAR
    MOVLW b'00010000'
    MOVWF LATC
    bcf permission_flag,0
    check_rc4:
        call display_1
        call display_2
        call BUTTON_TASK_RIGHT_DOWN
        call BUTTON_TASK_RIGHT_UP
        call BUTTON_TASK_LEFT_DOWN
        call BUTTON_TASK_LEFT_UP
        btfss permission_flag,0
        goto check_rc4
    BTFSC direction,0
    GOTO right_c4
    GOTO left_c4
        right_c4:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_right_c4
            GOTO up_or_straight_right_c4
            down_or_straight_right_c4:
                BTFSS direction_algorithm,1
                GOTO BALL_RD5
                GOTO BALL_RD4
            up_or_straight_right_c4:
                BTFSS direction_algorithm,1
                GOTO BALL_RD3
                GOTO BALL_RD4
        left_c4:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_left_c4
            GOTO up_or_straight_left_c4
            down_or_straight_left_c4:
                BTFSS direction_algorithm,1
                GOTO BALL_RB5
                GOTO BALL_RB4
            up_or_straight_left_c4:
                BTFSS direction_algorithm,1
                GOTO BALL_RB3
                GOTO BALL_RB4
 
BALL_RC5:
    
    call CLEAR
    MOVLW b'00100000'
    MOVWF LATC
    bcf permission_flag,0
    check_rc5:
        call display_1
        call display_2
        call BUTTON_TASK_RIGHT_DOWN
        call BUTTON_TASK_RIGHT_UP
        call BUTTON_TASK_LEFT_DOWN
        call BUTTON_TASK_LEFT_UP
        btfss permission_flag,0
        goto check_rc5
    BTFSC direction,0
    GOTO right_c5
    GOTO left_c5
        right_c5:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_right_c5
            GOTO up_or_straight_right_c5
            down_or_straight_right_c5:
                BTFSS direction_algorithm,1
                GOTO BALL_RD4
                GOTO BALL_RD5
            up_or_straight_right_c5:
                BTFSS direction_algorithm,1
                GOTO BALL_RD4
                GOTO BALL_RD5
        left_c5:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_left_c5
            GOTO up_or_straight_left_c5
            down_or_straight_left_c5:
                BTFSS direction_algorithm,1
                GOTO BALL_RB4
                GOTO BALL_RB5
            up_or_straight_left_c5:
                BTFSS direction_algorithm,1
                GOTO BALL_RB4
                GOTO BALL_RB5

BALL_RD0:
    
    call CLEAR
    MOVLW b'00000001'
    MOVWF LATD
    bcf permission_flag,0
    check_rd0:
        call display_1
        call display_2
        call BUTTON_TASK_RIGHT_DOWN
        call BUTTON_TASK_RIGHT_UP
        call BUTTON_TASK_LEFT_DOWN
        call BUTTON_TASK_LEFT_UP
        btfss permission_flag,0
        goto check_rd0
    BTFSC direction,0
    GOTO right_d0
    GOTO left_d0
        right_d0:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_right_d0
            GOTO up_or_straight_right_d0
            down_or_straight_right_d0:
                BTFSS direction_algorithm,1
                GOTO BALL_RE1
                GOTO BALL_RE0
            up_or_straight_right_d0:
                BTFSS direction_algorithm,1
                GOTO BALL_RE1
                GOTO BALL_RE0
        left_d0:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_left_d0
            GOTO up_or_straight_left_d0
            down_or_straight_left_d0:
                BTFSS direction_algorithm,1
                GOTO BALL_RC1
                GOTO BALL_RC0
            up_or_straight_left_d0:
                BTFSS direction_algorithm,1
                GOTO BALL_RC1
                GOTO BALL_RC0
BALL_RD1:
     
    call CLEAR
    MOVLW b'00000010'
    MOVWF LATD
    bcf permission_flag,0
    check_rd1:
        call display_1
        call display_2
        call BUTTON_TASK_RIGHT_DOWN
        call BUTTON_TASK_RIGHT_UP
        call BUTTON_TASK_LEFT_DOWN
        call BUTTON_TASK_LEFT_UP
        btfss permission_flag,0
        goto check_rd1
    BTFSC direction,0
    GOTO right_d1
    GOTO left_d1
        right_d1:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_right_d1
            GOTO up_or_straight_right_d1
            down_or_straight_right_d1:
                BTFSS direction_algorithm,1
                GOTO BALL_RE2
                GOTO BALL_RE1
            up_or_straight_right_d1:
                BTFSS direction_algorithm,1
                GOTO BALL_RE0
                GOTO BALL_RE1
        left_d1:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_left_d1
            GOTO up_or_straight_left_d1
            down_or_straight_left_d1:
                BTFSS direction_algorithm,1
                GOTO BALL_RC2
                GOTO BALL_RC1
            up_or_straight_left_d1:
                BTFSS direction_algorithm,1
                GOTO BALL_RC0
                GOTO BALL_RC1
BALL_RD2:
   
    call CLEAR
    MOVLW b'00000100'
    MOVWF LATD
    bcf permission_flag,0
    check_rd2:
        call display_1
        call display_2
        call BUTTON_TASK_RIGHT_DOWN
        call BUTTON_TASK_RIGHT_UP
        call BUTTON_TASK_LEFT_DOWN
        call BUTTON_TASK_LEFT_UP
        btfss permission_flag,0
        goto check_rd2
    BTFSC direction,0
    GOTO right_d2
    GOTO left_d2
        right_d2:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_right_d2
            GOTO up_or_straight_right_d2
            down_or_straight_right_d2:
                BTFSS direction_algorithm,1
                GOTO BALL_RE3
                GOTO BALL_RE2
            up_or_straight_right_d2:
                BTFSS direction_algorithm,1
                GOTO BALL_RE1
                GOTO BALL_RE2
        left_d2:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_left_d2
            GOTO up_or_straight_left_d2
            down_or_straight_left_d2:
                BTFSS direction_algorithm,1
                GOTO BALL_RC3
                GOTO BALL_RC2
            up_or_straight_left_d2:
                BTFSS direction_algorithm,1
                GOTO BALL_RC1
                GOTO BALL_RC2
BALL_RD3:
    call CLEAR
    MOVLW b'00001000'
    MOVWF LATD
    bcf permission_flag,0
    check_rd3:
        call display_1
        call display_2
        call BUTTON_TASK_RIGHT_DOWN
        call BUTTON_TASK_RIGHT_UP
        call BUTTON_TASK_LEFT_DOWN
        call BUTTON_TASK_LEFT_UP
        btfss permission_flag,0
        goto check_rd3
    BTFSC direction,0
    GOTO right_d3
    GOTO left_d3
        right_d3:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_right_d3
            GOTO up_or_straight_right_d3
            down_or_straight_right_d3:
                BTFSS direction_algorithm,1
                GOTO BALL_RE4
                GOTO BALL_RE3
            up_or_straight_right_d3:
                BTFSS direction_algorithm,1
                GOTO BALL_RE2
                GOTO BALL_RE3
        left_d3:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_left_d3
            GOTO up_or_straight_left_d3
            down_or_straight_left_d3:
                BTFSS direction_algorithm,1
                GOTO BALL_RC4
                GOTO BALL_RC3
            up_or_straight_left_d3:
                BTFSS direction_algorithm,1
                GOTO BALL_RC2
                GOTO BALL_RC3
BALL_RD4:
    call CLEAR
    MOVLW b'00010000'
    MOVWF LATD
    bcf permission_flag,0
    check_rd4:
        call display_1
        call display_2
        call BUTTON_TASK_RIGHT_DOWN
        call BUTTON_TASK_RIGHT_UP
        call BUTTON_TASK_LEFT_DOWN
        call BUTTON_TASK_LEFT_UP
        btfss permission_flag,0
        goto check_rd4
    BTFSC direction,0
    GOTO right_d4
    GOTO left_d4
        right_d4:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_right_d4
            GOTO up_or_straight_right_d4
            down_or_straight_right_d4:
                BTFSS direction_algorithm,1
                GOTO BALL_RE5
                GOTO BALL_RE4
            up_or_straight_right_d4:
                BTFSS direction_algorithm,1
                GOTO BALL_RE3
                GOTO BALL_RE4
        left_d4:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_left_d4
            GOTO up_or_straight_left_d4
            down_or_straight_left_d4:
                BTFSS direction_algorithm,1
                GOTO BALL_RC5
                GOTO BALL_RC4
            up_or_straight_left_d4:
                BTFSS direction_algorithm,1
                GOTO BALL_RC3
                GOTO BALL_RC4
BALL_RD5:
   
    call CLEAR
    MOVLW b'00100000'
    MOVWF LATD
    bcf permission_flag,0
    check_rd5:
        call display_1
        call display_2
        call BUTTON_TASK_RIGHT_DOWN
        call BUTTON_TASK_RIGHT_UP
        call BUTTON_TASK_LEFT_DOWN
        call BUTTON_TASK_LEFT_UP
        btfss permission_flag,0
        goto check_rd5
    BTFSC direction,0
    GOTO right_d5
    GOTO left_d5
        right_d5:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_right_d5
            GOTO up_or_straight_right_d5
            down_or_straight_right_d5:
                BTFSS direction_algorithm,1
                GOTO BALL_RE4
                GOTO BALL_RE5
            up_or_straight_right_d5:
                BTFSS direction_algorithm,1
                GOTO BALL_RE4
                GOTO BALL_RE5
        left_d5:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_left_d5
            GOTO up_or_straight_left_d5
            down_or_straight_left_d5:
                BTFSS direction_algorithm,1
                GOTO BALL_RC4
                GOTO BALL_RC5
            up_or_straight_left_d5:
                BTFSS direction_algorithm,1
                GOTO BALL_RC4
                GOTO BALL_RC5
BALL_RE0:
    call CLEAR
    MOVLW b'00000001'
    MOVWF LATE
    bcf permission_flag,0
    check_re0:
        call display_1
        call display_2
        call BUTTON_TASK_RIGHT_DOWN
        call BUTTON_TASK_RIGHT_UP
        call BUTTON_TASK_LEFT_DOWN
        call BUTTON_TASK_LEFT_UP
        btfss permission_flag,0
        goto check_re0
    BTFSC direction,0
    GOTO right_e0
    GOTO left_e0
        right_e0:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_right_e0
            GOTO up_or_straight_right_e0
            down_or_straight_right_e0:
                BTFSS direction_algorithm,1
                GOTO control_down_right_e0
                GOTO control_straight_right_e0
                control_down_right_e0:
                    BTFSS LATF,1
                    GOTO BALL_RF1
                    COMF direction,f
                    GOTO BALL_RE0
                control_straight_right_e0:
                    BTFSS LATF,0
                    GOTO BALL_RF0
                    COMF direction,f
                    GOTO BALL_RE0
            up_or_straight_right_e0:
                BTFSS direction_algorithm,1
                GOTO control_up_right_e0
                GOTO control_straight_right_e0
                control_up_right_e0:
                    BTFSS LATF,1
                    GOTO BALL_RF1
                    COMF direction,f
                    GOTO BALL_RE0
        left_e0:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_left_e0
            GOTO up_or_straight_left_e0
            down_or_straight_left_e0:
                BTFSS direction_algorithm,1
                GOTO BALL_RD1
                GOTO BALL_RD0
            up_or_straight_left_e0:
                BTFSS direction_algorithm,1
                GOTO BALL_RD1
                GOTO BALL_RD0
BALL_RE1:
    call CLEAR
    MOVLW b'00000010'
    MOVWF LATE
    bcf permission_flag,0
    check_re1:
        call display_1
        call display_2
        call BUTTON_TASK_RIGHT_DOWN
        call BUTTON_TASK_RIGHT_UP
        call BUTTON_TASK_LEFT_DOWN
        call BUTTON_TASK_LEFT_UP
        btfss permission_flag,0
        goto check_re1
    BTFSC direction,0
    GOTO right_e1
    GOTO left_e1
        right_e1:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_right_e1
            GOTO up_or_straight_right_e1
            down_or_straight_right_e1:
                BTFSS direction_algorithm,1
                GOTO control_down_right_e1
                GOTO control_straight_right_e1
                control_down_right_e1:
                    BTFSS LATF,2
                    GOTO BALL_RF2
                    COMF direction,f
                    GOTO BALL_RE1
                control_straight_right_e1:
                    BTFSS LATF,1
                    GOTO BALL_RF1
                    COMF direction,f
                    GOTO BALL_RE1
            up_or_straight_right_e1:
                BTFSS direction_algorithm,1
                GOTO control_up_right_e1
                GOTO control_straight_right_e1
                control_up_right_e1:
                    BTFSS LATF,0
                    GOTO BALL_RF0
                    COMF direction,f
                    GOTO BALL_RE1
        left_e1:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_left_e1
            GOTO up_or_straight_left_e1
            down_or_straight_left_e1:
                BTFSS direction_algorithm,1
                GOTO BALL_RD2
                GOTO BALL_RD1
            up_or_straight_left_e1:
                BTFSS direction_algorithm,1
                GOTO BALL_RD0
                GOTO BALL_RD1

BALL_RE2:
    call CLEAR
    MOVLW b'00000100'
    MOVWF LATE
    bcf permission_flag,0
    check_re2:
        call display_1
        call display_2
        call BUTTON_TASK_RIGHT_DOWN
        call BUTTON_TASK_RIGHT_UP
        call BUTTON_TASK_LEFT_DOWN
        call BUTTON_TASK_LEFT_UP
        btfss permission_flag,0
        goto check_re2
   BTFSC direction,0
   GOTO right_e2
   GOTO left_e2
        right_e2:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_right_e2
            GOTO up_or_straight_right_e2
            down_or_straight_right_e2:
                BTFSS direction_algorithm,1
                GOTO control_down_right_e2
                GOTO control_straight_right_e2
                control_down_right_e2:
                    BTFSS LATF,3
                    GOTO BALL_RF3
                    COMF direction,f
                    GOTO BALL_RE2
                control_straight_right_e2:
                    BTFSS LATF,2
                    GOTO BALL_RF2
                    COMF direction,f
                    GOTO BALL_RE2
            up_or_straight_right_e2:
                BTFSS direction_algorithm,1
                GOTO control_up_right_e2
                GOTO control_straight_right_e2
                control_up_right_e2:
                    BTFSS LATF,1
                    GOTO BALL_RF1
                    COMF direction,f
                    GOTO BALL_RE2
        left_e2:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_left_e2
            GOTO up_or_straight_left_e2
            down_or_straight_left_e2:
                BTFSS direction_algorithm,1
                GOTO BALL_RD3
                GOTO BALL_RD2
            up_or_straight_left_e2:
                BTFSS direction_algorithm,1
                GOTO BALL_RD1
                GOTO BALL_RD2
    
BALL_RE3:

    call CLEAR
    MOVLW b'00001000'
    MOVWF LATE
    bcf permission_flag,0
    check_re3:
        call display_1
        call display_2
        call BUTTON_TASK_RIGHT_DOWN
        call BUTTON_TASK_RIGHT_UP
        call BUTTON_TASK_LEFT_DOWN
        call BUTTON_TASK_LEFT_UP
        btfss permission_flag,0
        goto check_re3
   BTFSC direction,0
   GOTO right_e3
   GOTO left_e3
        right_e3:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_right_e3
            GOTO up_or_straight_right_e3
            down_or_straight_right_e3:
                BTFSS direction_algorithm,1
                GOTO control_down_right_e3
                GOTO control_straight_right_e3
                control_down_right_e3:
                    BTFSS LATF,4
                    GOTO BALL_RF4
                    COMF direction,f
                    GOTO BALL_RE3
                control_straight_right_e3:
                    BTFSS LATF,3
                    GOTO BALL_RF3
                    COMF direction,f
                    GOTO BALL_RE3
            up_or_straight_right_e3:
                BTFSS direction_algorithm,1
                GOTO control_up_right_e3
                GOTO control_straight_right_e3
                control_up_right_e3:
                    BTFSS LATF,2
                    GOTO BALL_RF2
                    COMF direction,f
                    GOTO BALL_RE3
        left_e3:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_left_e3
            GOTO up_or_straight_left_e3
            down_or_straight_left_e3:
                BTFSS direction_algorithm,1
                GOTO BALL_RD4
                GOTO BALL_RD3
            up_or_straight_left_e3:
                BTFSS direction_algorithm,1
                GOTO BALL_RD2
                GOTO BALL_RD3
    
BALL_RE4:

    call CLEAR
    MOVLW b'00010000'
    MOVWF LATE
    bcf permission_flag,0
    check_re4:
        call display_1
        call display_2
        call BUTTON_TASK_RIGHT_DOWN
        call BUTTON_TASK_RIGHT_UP
        call BUTTON_TASK_LEFT_DOWN
        call BUTTON_TASK_LEFT_UP
        btfss permission_flag,0
        goto check_re4
   BTFSC direction,0
   GOTO right_e4
   GOTO left_e4
        right_e4:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_right_e4
            GOTO up_or_straight_right_e4
            down_or_straight_right_e4:
                BTFSS direction_algorithm,1
                GOTO control_down_right_e4
                GOTO control_straight_right_e4
                control_down_right_e4:
                    BTFSS LATF,5
                    GOTO BALL_RF5
                    COMF direction,f
                    GOTO BALL_RE4
                control_straight_right_e4:
                    BTFSS LATF,4
                    GOTO BALL_RF4
                    COMF direction,f
                    GOTO BALL_RE4
            up_or_straight_right_e4:
                BTFSS direction_algorithm,1
                GOTO control_up_right_e4
                GOTO control_straight_right_e4
                control_up_right_e4:
                    BTFSS LATF,3
                    GOTO BALL_RF3
                    COMF direction,f
                    GOTO BALL_RE4
        left_e4:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_left_e4
            GOTO up_or_straight_left_e4
            down_or_straight_left_e4:
                BTFSS direction_algorithm,1
                GOTO BALL_RD5
                GOTO BALL_RD4
            up_or_straight_left_e4:
                BTFSS direction_algorithm,1
                GOTO BALL_RD3
                GOTO BALL_RD4
    
BALL_RE5:

    call CLEAR
    MOVLW b'00100000'
    MOVWF LATE
    bcf permission_flag,0
    check_re5:
        call display_1
        call display_2
        call BUTTON_TASK_RIGHT_DOWN
        call BUTTON_TASK_RIGHT_UP
        call BUTTON_TASK_LEFT_DOWN
        call BUTTON_TASK_LEFT_UP
        btfss permission_flag,0
        goto check_re5
   BTFSC direction,0
   GOTO right_e5
   GOTO left_e5
        right_e5:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_right_e5
            GOTO up_or_straight_right_e5
            down_or_straight_right_e5:
                BTFSS direction_algorithm,1
                GOTO control_down_right_e5
                GOTO control_straight_right_e5
                control_down_right_e5:
                    BTFSS LATF,4
                    GOTO BALL_RF4
                    COMF direction,f
                    GOTO BALL_RE5
                control_straight_right_e5:
                    BTFSS LATF,5
                    GOTO BALL_RF5
                    COMF direction,f
                    GOTO BALL_RE5
            up_or_straight_right_e5:
                BTFSS direction_algorithm,1
                GOTO control_up_right_e5
                GOTO control_straight_right_e5
                control_up_right_e5:
                    BTFSS LATF,4
                    GOTO BALL_RF4
                    COMF direction,f
                    GOTO BALL_RE5
        left_e5:
            BTFSS direction_algorithm,0
            GOTO down_or_straight_left_e5
            GOTO up_or_straight_left_e5
            down_or_straight_left_e5:
                BTFSS direction_algorithm,1
                GOTO BALL_RD4
                GOTO BALL_RD5
            up_or_straight_left_e5:
                BTFSS direction_algorithm,1
                GOTO BALL_RD4
                GOTO BALL_RD5
CLEAR:
    CLRF LATB
    CLRF LATC
    CLRF LATD
    CLRF LATE
    return

BUTTON_TASK_LEFT_UP:        ; RG3
    BTFSS PORTG,3
    return
    left_up:
        BTFSC permission_flag,0
        return
        BTFSC PORTG,3
        GOTO left_up
        MOVLW 0x7
        CPFSEQ LATA
        RRNCF LATA,F
        return
BUTTON_TASK_LEFT_DOWN:        ; RG2
    BTFSS PORTG,2
    return
    left_down:
        BTFSC permission_flag,0
        return
        BTFSC PORTG,2
        GOTO left_down
        MOVLW 0x38
        CPFSEQ LATA
        RLNCF LATA,F
        return
BUTTON_TASK_RIGHT_UP:        ; RG1
    BTFSS PORTG,1
    return
    right_up:
        BTFSC permission_flag,0
        return
        BTFSC PORTG,1
        GOTO right_up
        MOVLW 0x7
        CPFSEQ LATF
        RRNCF LATF,F
        return
BUTTON_TASK_RIGHT_DOWN:        ; RG0
    BTFSS PORTG,0
    return
    right_down:
        BTFSC permission_flag,0
        return
        BTFSC PORTG,0
        GOTO right_down
        MOVLW 0x38
        CPFSEQ LATF
        RLNCF LATF,F
        return




END





