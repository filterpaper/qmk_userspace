# Disable unused features
#LEADER_ENABLE = no
#COMMAND_ENABLE = no
#TERMINAL_ENABLE = no
#KEY_LOCK_ENABLE = no
#SLEEP_LED_ENABLE = no
#VELOCIKEY_ENABLE = no
MAGIC_ENABLE = no
CONSOLE_ENABLE = no
UNICODE_ENABLE = no
SPACE_CADET_ENABLE = no

# Common features
COMBO_ENABLE = yes
EXTRAKEY_ENABLE = yes
MOUSEKEY_ENABLE = yes
BOOTMAGIC_ENABLE = yes

VPATH += $(USER_PATH)/oled $(USER_PATH)/rgb $(USER_PATH)/features
OPT_DEFS += -DCAPS_UNLOCK -DINIT_EE_HANDS_$(shell echo ${SPLIT}|tr a-z A-Z)
SRC += filterpaper.c caps_unlock.c combos.c

ifeq ($(strip $(MCU)), atmega32u4)
	LTO_ENABLE = yes
	TOP_SYMBOLS = yes
	BOOTLOADER = atmel-dfu
else
	SWAP_HANDS_ENABLE = yes
	DEBOUNCE_TYPE = asym_eager_defer_pk
#	EEPROM_DRIVER = transient
	ifeq ($(strip $(CONVERT_TO)), kb2040)
		RGB_MATRIX_ENABLE = yes
		RGB_MATRIX_DRIVER = WS2812
		RGB_MATRIX_CUSTOM_USER = yes
		SRC += rgb-matrix.c
	endif
endif

# Split boards
ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), cradio crkbd/rev1))
	OPT_DEFS += -DAUTO_CORRECT
	SRC += autocorrect.c
endif

# RGB boards
ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), crkbd/rev1 boardsource/technik_o))
	RGB_MATRIX_ENABLE = yes
	RGB_MATRIX_CUSTOM_USER = yes
	SRC += rgb-matrix.c
endif

# OLED
ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), crkbd/rev1))
	OLED_ENABLE = yes
	ifneq ($(strip $(OLED)),)
		OPT_DEFS += -D${OLED}
		SRC += oled-icons.c oled-luna.c
	else
		SRC += oled-icons.c oled-bongocat.c
	endif
endif
