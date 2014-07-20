#!/bin/bash
# 
# JAMS
# (c) 2014 Justin Gottula
# The source code of this project is distributed under the terms of the
# simplified BSD license. See the LICENSE file for details.
# 
# main redo script


exec >&2


# redo appears to be broken
set -e


TARGET=$1
TARGET_BASE=$2
OUTPUT=$3


PROJ_NAME="jams"


# configuration files for microcontroller and programmer
CFG_MCU="atxmega128a3u"
CFG_PROG="dragon_jtag"

CFG_FILES=("cfg/mcu/$CFG_MCU" "cfg/prog/$CFG_PROG")
redo-ifchange "${CFG_FILES[@]}"
for CFG_FILE in "${CFG_FILES[@]}"; do
	source "$CFG_FILE"
done


CC="avr-gcc"
AS="avr-as"
LD="avr-ld"

NM="avr-nm"
OBJCOPY="avr-objcopy"
OBJDUMP="avr-objdump"

AVRDUDE="avrdude"


SOURCES=$(find src -type f -iname "*.c")
OBJECTS=${SOURCES//.c/.o}

OUT_ELF="out/$PROJ_NAME.elf"
OUT_BIN="out/$PROJ_NAME.bin"
OUT_MAP="out/$PROJ_NAME.map"
OUT_DUMP="out/$PROJ_NAME.dump"
OUT_LST="out/$PROJ_NAME.lst"


DEFINES=("F_CPU=${CPU_KHZ}000UL")
INC_DIRS=(src)
LIB_DIRS=()
LIBS=()

CFLAGS_STD=(-std=gnu11)
CFLAGS_CPU=("-mmcu=$MCU" -fpack-struct -funsigned-bitfields)
CFLAGS_DEBUG=(-ggdb -fno-omit-frame-pointer -fvar-tracking-assignments \
	-fno-inline)
CFLAGS_OPT=(-O1 -mrelax -mstrict-X -fmerge-constants -ffast-math \
	-fno-jump-tables -fshort-enums -Wl,--gc-sections)
CFLAGS_WARN=(-Wall -Wextra -Wno-unused-function -fno-diagnostics-show-caret)
CFLAGS_ETC=()

CFLAGS=("${CFLAGS_STD[@]}" "${CFLAGS_CPU[@]}" "${CFLAGS_DEBUG[@]}" \
	"${CFLAGS_OPT[@]}" "${CFLAGS_WARN[@]}" "${CFLAGS_ETC[@]}")
CFLAGS_COMPILE=("${CFLAGS[@]}" "${DEFINES[@]/#/-D}" "${INC_DIRS[@]/#/-I}")
CFLAGS_LINK=("${CFLAGS[@]}" "${LIB_DIRS[@]/#/-L}" "${LIBS[@]/#/-l}")


AVRDUDE_FLAGS=("${AVRDUDE_PART[@]/#/-p}" "${AVRDUDE_PROG[@]/#/-c}" \
	"${AVRDUDE_PORT[@]/#/-P}" "${AVRDUDE_BITCLOCK[@]/#/-B}" \
	"${AVRDUDE_BAUDRATE[@]/#/-b}" "${AVRDUDE_BANGDELAY[@]/#/-i}")

AVRDUDE_FLAGS_LOAD=("${AVRDUDE_FLAGS[@]}" -u \
	-Uflash:w:"$OUT_BIN":r)
AVRDUDE_FLAGS_VERIFY=("${AVRDUDE_FLAGS[@]}" -u \
	-Uflash:v:"$OUT_BIN":r)
AVRDUDE_FLAGS_FUSE=("${AVRDUDE_FLAGS[@]}" -q \
	-Ulfuse:w:"$AVRDUDE_LFUSE":m \
	-Uhfuse:w:"$AVRDUDE_HFUSE":m \
	-Uefuse:w:"$AVRDUDE_EFUSE":m)
AVRDUDE_FLAGS_TERM=("${AVRDUDE_FLAGS[@]}" -t)


case "$TARGET" in
all)
	redo-ifchange $OUT_ELF $OUT_BIN $OUT_MAP $OUT_DUMP $OUT_LST
	;;
$OUT_ELF)
	redo-ifchange $OBJECTS
	$CC "${CFLAGS_LINK[@]}" -o$OUTPUT $OBJECTS
	;;
$OUT_BIN)
	redo-ifchange $OUT_ELF
	$OBJCOPY -Obinary -j.text -j.data $OUT_ELF $OUTPUT
	
	stat --printf="%n: %s bytes\n" $OUTPUT
	md5sum $OUTPUT
	;;
$OUT_MAP)
	redo-ifchange $OUT_ELF
	$NM -Cn $OUT_ELF >$OUTPUT
	;;
$OUT_DUMP)
	redo-ifchange $OUT_ELF
	$OBJDUMP -aCdfhp $OUT_ELF >$OUTPUT
	;;
$OUT_LST)
	redo-ifchange $OUT_ELF
	$OBJDUMP -CdS -j.text $OUT_ELF >$OUTPUT
	;;
*.o)
	$CC "${CFLAGS_COMPILE[@]}" -o${TARGET//.o/.dep} -MM -MG ${TARGET//.o/.c}
	
	read DEPS <${TARGET//.o/.dep}
	redo-ifchange ${DEPS#*:}
	
	$CC "${CFLAGS_COMPILE[@]}" -o$OUTPUT -c ${TARGET//.o/.c}
	;;
clean)
	rm -rf $(find out -type f)
	rm -rf $(find src -type f -iname "*.o")
	rm -rf $(find src -type f -iname "*.dep")
	;;
load)
	redo all
	echo "sudo $AVRDUDE ${AVRDUDE_FLAGS_LOAD[@]}"
	sudo $AVRDUDE "${AVRDUDE_FLAGS_LOAD[@]}"
	;;
verify)
	echo "sudo $AVRDUDE ${AVRDUDE_FLAGS_VERIFY[@]}"
	sudo $AVRDUDE "${AVRDUDE_FLAGS_VERIFY[@]}"
	;;
fuse)
	echo "sudo $AVRDUDE ${AVRDUDE_FLAGS_FUSE[@]}"
	sudo $AVRDUDE "${AVRDUDE_FLAGS_FUSE[@]}"
	;;
term)
	echo "sudo $AVRDUDE ${AVRDUDE_FLAGS_TERM[@]}"
	sudo $AVRDUDE "${AVRDUDE_FLAGS_TERM[@]}"
	;;
reset)
	# TODO: variable-ize this
	echo "avarice -g -P atmega1284p -R"
	avarice -g -P atmega1284p -R
	;;
*)
	echo "unknown target '$TARGET'"
	exit 1
	;;
esac
