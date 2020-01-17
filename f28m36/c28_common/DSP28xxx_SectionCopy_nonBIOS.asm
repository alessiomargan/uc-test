;############################################################################
;
; FILE:   DSP28xxx_SectionCopy_nonBIOS.asm
;
; DESCRIPTION:  Provides functionality for copying intialized sections from 
;				flash to ram at runtime before entering the _c_int00 startup
;				routine
;############################################################################
; Author: Tim Love
; Release Date: March 2008	
;############################################################################


	.ref _c_int00
	.global copy_sections
	.global _init_load, _init_run, _init_size
	;.global _cinit_loadstart, _cinit_runstart, _cinit_size
	;.global _const_loadstart, _const_runstart, _const_size
	;.global _econst_loadstart, _econst_runstart, _econst_size
	;.global _pinit_loadstart, _pinit_runstart, _pinit_size
	;.global _switch_loadstart, _switch_runstart, _switch_size
	;.global _text_loadstart, _text_runstart, _text_size
	
***********************************************************************
* Function: copy_sections
*
* Description: Copies initialized sections from flash to ram
***********************************************************************

	.sect "copysections"

copy_sections:

	MOVL XAR5,#_init_size				; Store Section Size in XAR5
	MOVL ACC,@XAR5						; Move Section Size to ACC
	MOVL XAR6,#_init_load				; Store Load Starting Address in XAR6
    MOVL XAR7,#_init_run					; Store Run Address in XAR7
    LCR  copy							; Branch to Copy

    LB _c_int00				 			; Branch to start of boot.asm in RTS library

copy:	
	B return,EQ							; Return if ACC is Zero (No section to copy)

    RPT AL								; Copy Section From Load Address to
    || PWRITE  *XAR7, *XAR6++			; Run Address

return:
	LRETR								; Return

	.end
	
;//===========================================================================
;// End of file.
;//===========================================================================
