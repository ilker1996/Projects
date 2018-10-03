#include "p18f8722.inc"
; CONFIG1H
  CONFIG  OSC = HSPLL, FCMEN = OFF, IESO = OFF
; CONFIG2L
  CONFIG  PWRT = OFF, BOREN = OFF, BORV = 3
; CONFIG2H
  CONFIG  WDT = OFF, WDTPS = 32768
; CONFIG3L
  CONFIG  MODE = MC, ADDRBW = ADDR20BIT, DATABW = DATA16BIT, WAIT = OFF
; CONFIG3H
  CONFIG  CCP2MX = PORTC, ECCPMX = PORTE, LPT1OSC = OFF, MCLRE = ON
; CONFIG4L
  CONFIG  STVREN = ON, LVP = OFF, BBSIZ = BB2K, XINST = OFF
; CONFIG5L
  CONFIG  CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF, CP4 = OFF, CP5 = OFF
  CONFIG  CP6 = OFF, CP7 = OFF
; CONFIG5H
  CONFIG  CPB = OFF, CPD = OFF
; CONFIG6L
  CONFIG  WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF, WRT4 = OFF
  CONFIG  WRT5 = OFF, WRT6 = OFF, WRT7 = OFF
; CONFIG6H
  CONFIG  WRTC = OFF, WRTB = OFF, WRTD = OFF
; CONFIG7L
  CONFIG  EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF, EBTR4 = OFF
  CONFIG  EBTR5 = OFF, EBTR6 = OFF, EBTR7 = OFF
; CONFIG7H
  CONFIG  EBTRB = OFF
UDATA_ACS
  t1	res 1	; used in delay
  t2	res 1	; used in delay
  t3	res 1	; used in delay
  state res 1   ; (0 if state1) (1 if state2)
  B5    res 1   ; used in RB5 testing

RES_VECT CODE 0x0000    ; processor reset vector
    GOTO INIT

    INIT
        BCF  state,4
        BCF  B5,5
        MOVLW b'1111'
        MOVWF ADCON1
        MOVLW b'00010000'  ;RA4 is input ohers are output
        MOVWF TRISA
        MOVLW b'00100000'  ;RB5 is input others are output
        MOVWF TRISB
        CLRF TRISC         ;All is output
        CLRF TRISD
        call TURNON
        call DELAY_2sec
        GOTO move_1


    TESTB5
        BTFSS PORTB,5
        return
        debounce:
            BTFSC PORTB,5
            GOTO debounce	; busy waiting. FIXME !!!
            BSF  B5,5
            return
    TESTB5_0
        BTFSC state,4
        return
        call TESTA4
        BTFSC state,4
        return
        call TESTB5
        BTFSS B5,5
        GOTO TESTB5_0
        BCF B5,5
        return


    TESTB5_1
        BTFSS state,4
        return
        call TESTA4
        BTFSS state,4
        return
        call TESTB5
        BTFSS B5,5
        GOTO TESTB5_1
        BCF B5,5
        return
    TESTA4
        BTFSS PORTA,4
        return
        _debounce:
            BTFSC PORTA,4
            GOTO _debounce	; busy waiting. FIXME !!!
            BTG state,4
            return

    move_1		;A0-B0
        MOVLW b'00000000'
        MOVWF LATC
        MOVWF LATD
        MOVLW b'00000001'
        MOVWF LATA
        MOVWF LATB
        MOVF PORTB,0
        call TESTB5_1     ; STATE1 stucked
        call DELAY
        BTFSS state,4
        GOTO move_2
        GOTO move_12
    move_2		;B0-C0
        MOVLW b'00000000'
        MOVWF LATA
        MOVWF LATD
        MOVLW b'00000001'
        MOVWF LATB
        MOVWF LATC
        call DELAY
        BTFSS state,4
        GOTO move_3
        GOTO move_1
    move_3		;C0-D0
        MOVLW b'00000000'
        MOVWF LATA
        MOVWF LATB
        MOVLW b'00000001'
        MOVWF LATC
        MOVWF LATD
        call TESTB5_0       ;STATE0 stucked
        call DELAY
        BTFSS state,4
        GOTO move_4
        GOTO move_2
    move_4		;D0-D1
        MOVLW b'00000000'
        MOVWF LATA
        MOVWF LATB
        MOVWF LATC
        MOVLW b'00000011'
        MOVWF LATD
        call TESTB5_1       ; STATE1 stucked
    	call DELAY
        BTFSS state,4
        GOTO move_5
        GOTO move_3
    move_5		;D1-D2
        MOVLW b'00000000'
        MOVWF LATA
        MOVWF LATB
        MOVWF LATC
    	MOVLW b'00000110'
        MOVWF LATD
        call DELAY
        BTFSS state,4
        GOTO move_6
        GOTO move_4
    move_6		;D2-D3
        MOVLW b'00000000'
        MOVWF LATA
        MOVWF LATB
        MOVWF LATC
        MOVLW b'00001100'
        MOVWF LATD
        call TESTB5_0        ;STATE0 stucked
        call DELAY
        BTFSS state,4
        GOTO move_7
        GOTO move_5
    move_7		;D3-C3
        MOVLW b'00000000'
        MOVWF LATA
        MOVWF LATB
        MOVLW b'00001000'
        MOVWF LATC
        MOVWF LATD
        call TESTB5_1          ; STATE1 stucked
        call DELAY
        BTFSS state,4
        GOTO move_8
        GOTO move_6
    move_8		;C3-B3
        MOVLW b'00000000'
        MOVWF LATA
        MOVWF LATD
        MOVLW b'00001000'
        MOVWF LATC
        MOVWF LATB
        call DELAY
        BTFSS state,4
        GOTO move_9
        GOTO move_7
    move_9		;B3-A3
        MOVLW b'00000000'
        MOVWF LATC
        MOVWF LATD
        MOVLW b'00001000'
        MOVWF LATA
        MOVWF LATB
        call TESTB5_0     ;STATE0 stucked
        call DELAY
    	BTFSS state,4
        GOTO move_10
        GOTO move_8
    move_10		;A3-A2
        MOVLW b'00000000'
        MOVWF LATB
        MOVWF LATC
        MOVWF LATD
        MOVLW b'00001100'
        MOVWF LATA
        call TESTB5_1         ; STATE1 stucked
        call DELAY
        BTFSS state,4
        GOTO move_11
        GOTO move_9
    move_11		;A2-A1
        MOVLW b'00000000'
        MOVWF LATB
        MOVWF LATC
        MOVWF LATD
        MOVLW b'00000110'
        MOVWF LATA
        call DELAY
        BTFSS state,4
        GOTO move_12
        GOTO move_10
    move_12		;A1-A0
        MOVLW b'00000000'
        MOVWF LATB
        MOVWF LATC
        MOVWF LATD
        MOVLW b'00000011'
        MOVWF LATA
        call TESTB5_0    ; STATE0 stucked
        call DELAY
        BTFSS state,4
        GOTO move_1
        GOTO move_11

    DELAY
        BTFSS state,4
        GOTO  quit0
        GOTO  quit1
        quit1:
            call DELAY_1
            return
        quit0:
             call DELAY_0
             return

    TURNON
        MOVLW b'00001111'
    	MOVWF LATA
        MOVWF LATB
        MOVWF LATC
        MOVWF LATD
        return

    DELAY_0   ;750 MILISECOND
        MOVLW 0x4E	; 82 Copy desired value to W
        MOVWF t3	; Copy W into t3
        _loop4:
            MOVLW 0x52 ; Copy desired value to W
            MOVWF t2    ; Copy W into t2
            _loop5:
                MOVLW 0x75	; 0x9F Copy desired value to W
                MOVWF t1	; Copy W into t1
                _loop6:
                    call TESTA4
                    BTFSC state,4
                    return
                    decfsz t1,F ; Decrement t1. If 0 Skip next instruction
                    GOTO _loop6 ; ELSE Keep counting down
                    decfsz t2,F ; Decrement t2. If 0 Skip next instruction
                    GOTO _loop5 ; ELSE Keep counting down
                    decfsz t3,F ; Decrement t3. If 0 Skip next instruction
                    GOTO _loop4 ; ELSE Keep counting down
                    return
    DELAY_1   ;400 MILISECOND
        MOVLW 0x4F	; 82 Copy desired value to W
        MOVWF t3	; Copy W into t3
        _loop7:
            MOVLW 0x4A  ; Copy desired value to W
            MOVWF t2    ; Copy W into t2
            _loop8:
                MOVLW 0x44	; 0x9F Copy desired value to W
                MOVWF t1	; Copy W into t1
                _loop9:
                    call TESTA4
                    BTFSS state,4
                    return
                    decfsz t1,F ; Decrement t1. If 0 Skip next instruction
                    GOTO _loop9 ; ELSE Keep counting down
                    decfsz t2,F ; Decrement t2. If 0 Skip next instruction
                    GOTO _loop8 ; ELSE Keep counting down
                    decfsz t3,F ; Decrement t3. If 0 Skip next instruction
                    GOTO _loop7 ; ELSE Keep counting down
                    return

    DELAY_2sec ;2 SECOND
        MOVLW 0xFF	; 82 Copy desired value to W
        MOVWF t3	; Copy W into t3
        _loop3:
            MOVLW 0xA3  ; Copy desired value to W
            MOVWF t2    ; Copy W into t2
            _loop2:
                MOVLW 0x9F	; 0x9F Copy desired value to W
                MOVWF t1	; Copy W into t1
                _loop1:
                    decfsz t1,F ; Decrement t1. If 0 Skip next instruction
                    GOTO _loop1 ; ELSE Keep counting down
                    decfsz t2,F ; Decrement t2. If 0 Skip next instruction
                    GOTO _loop2 ; ELSE Keep counting down
                    decfsz t3,F ; Decrement t3. If 0 Skip next instruction
                    GOTO _loop3 ; ELSE Keep counting down
                    return

END

