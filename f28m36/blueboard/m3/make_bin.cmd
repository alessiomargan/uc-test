/*
${CG_TOOL_ROOT}/bin/armhex -o=${BuildArtifactFileBaseName}.bin -map=${BuildArtifactFileBaseName}_bin.map ${PROJECT_LOC}/make_bin.cmd ${BuildArtifactFileName}
*/
--order MS
--issue_remarks
--binary /* Note all addresses in 8 bit memwidth */
--zero /* Start from zero in output file */
--image

ROMS
{
/* multiply the linker addressing of Flash sections by 2 (16 bit to 8 bit addressing ) */
/* when using --binary option all addresses are converted to binary (byte addressing) */
APPLICATION_BINARY:
org = 0x002C0000, /* reset ISR */
len = 0x0020000,
fill = 0xFFFFFFFF
}

SECTIONS
{
.resetisr :
.intvecs :
.text :
.const :
.cinit :
.binit :
.TI.ramfunc :
.init_array :
}
