/*
${CG_TOOL_ROOT}/bin/hex2000 -o=${BuildArtifactFileBaseName}.bin -map=${BuildArtifactFileBaseName}_bin.map ${PROJECT_LOC}/make_bin.cmd ${BuildArtifactFileName}
*/
--order LS
--issue_remarks
--binary /* Note all addresses in 8 bit memwidth */
--zero /* Start from zero in output file */
--image

ROMS
{
/* multiply the linker addressing of Flash sections by 2 (16 bit to 8 bit addressing ) */
/* when using --binary option all addresses are converted to binary (byte addressing) */
APPLICATION_BINARY:
org = 0x00260000, /* 0x130000 in linker */
/*org = 0x00240000,*/ /* 0x120000 in linker */
len = 0x010000, /* 0x8000 in linker */
romwidth = 16, /* no influence because romwidth > 8 */
fill = 0xFFFF
}

SECTIONS
{
.cinit :
.text :
codestart :
.econst :
.binit :
.TI.ramfunc :
}
