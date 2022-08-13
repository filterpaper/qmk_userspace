# Disable unused features
#VIA_ENABLE = no
#WPM_ENABLE = no
#NKRO_ENABLE = no
#AUDIO_ENABLE = no
#STENO_ENABLE = no
#LEADER_ENABLE = no
#COMMAND_ENABLE = no
#TERMINAL_ENABLE = no
#KEY_LOCK_ENABLE = no
#SLEEP_LED_ENABLE = no
#TAP_DANCE_ENABLE = no
#VELOCIKEY_ENABLE = no
MAGIC_ENABLE = no
CONSOLE_ENABLE = no
UNICODE_ENABLE = no
RGBLIGHT_ENABLE = no
SWAP_HANDS_ENABLE = no
SPACE_CADET_ENABLE = no

# Common features
WAIT_FOR_USB = yes
EXTRAKEY_ENABLE = yes
MOUSEKEY_ENABLE = yes
BOOTMAGIC_ENABLE = yes

# Main source file and source paths
SRC += filterpaper.c
VPATH += $(USER_PATH)/oled $(USER_PATH)/rgb $(USER_PATH)/features

# Custom features
COMBO_ENABLE = yes
SRC += combos.c
OPT_DEFS += -DCAPS_UNLOCK
SRC += caps_unlock.c

ifeq ($(strip $(MCU)), atmega32u4)
	LTO_ENABLE = yes
	BOOTLOADER = atmel-dfu
endif

ifneq ($(strip $(CONVERT_TO)),)
	EEPROM_DRIVER = transient
	OPT_DEFS += -DINIT_EE_HANDS_$(shell echo ${SPLIT}|tr a-z A-Z)
endif

# Small split keyboards
ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), 3w6/rev2 cradio))
	OPT_DEFS += -DAUTO_CORRECT
	SRC += autocorrect.c
endif

# The Mark65 and Technik
ifeq ($(findstring boardsource/, $(KEYBOARD)), boardsource/)
	RGB_MATRIX_ENABLE = yes
	RGB_MATRIX_CUSTOM_USER = yes
	SRC += rgb-matrix.c
endif

# Corne CRKBD
ifeq ($(strip $(KEYBOARD)), crkbd/rev1)
	RGB_MATRIX_ENABLE = yes
	RGB_MATRIX_CUSTOM_USER = yes
	OLED_ENABLE = yes
	ifeq ($(strip $(OLED)), LUNA FELIX)
		OPT_DEFS += -DAUTO_CORRECT -D${OLED}
		SRC += autocorrect.c rgb-matrix.c oled-icons.c oled-luna.c
	else
		OPT_DEFS += -DAUTO_CORRECT -D$(shell echo ${SPLIT}|tr a-z A-Z)CAT
		SRC += autocorrect.c rgb-matrix.c oled-icons.c oled-bongocat.c
	endif
endif