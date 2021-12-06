# Disable unused features
VIA_ENABLE = no
WPM_ENABLE = no
NKRO_ENABLE = no
AUDIO_ENABLE = no
STENO_ENABLE = no
LEADER_ENABLE = no
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
UNICODE_ENABLE = no
MOUSEKEY_ENABLE = no
TERMINAL_ENABLE = no
KEY_LOCK_ENABLE = no
RGBLIGHT_ENABLE = no
BOOTMAGIC_ENABLE = no
SLEEP_LED_ENABLE = no
TAP_DANCE_ENABLE = no
VELOCIKEY_ENABLE = no
SWAP_HANDS_ENABLE = no
SPACE_CADET_ENABLE = no

# Common features
LTO_ENABLE = yes
EXTRAKEY_ENABLE = yes
MOUSEKEY_ENABLE = yes
BOOTLOADER = atmel-dfu
BOOTMAGIC_ENABLE = yes
COMBO_ENABLE = yes
SRC += combos.c

# Main source file
SRC += filterpaper.c

# Small split keyboards
ifeq ($(KEYBOARD), $(filter $(KEYBOARD), 3w6/rev2 a_dux cradio))
	SWAP_HANDS_ENABLE = yes
	OPT_DEFS += -DPROSE_FEATURES
	SRC += prose/autocorrection.c
	SRC += prose/caps_word.c
endif

# Boardsource's Mark65 and Technik
ifeq ($(findstring boardsource/, $(KEYBOARD)), boardsource/)
	RGB_MATRIX_ENABLE = yes
	RGB_MATRIX_CUSTOM_USER = yes
	SRC += rgb-matrix.c
endif

# Corne CRKBD
ifeq ($(KEYBOARD), crkbd/rev1)
	ifneq ($(strip $(OLED)),)
		OLED_ENABLE = yes
		OPT_DEFS += -D${OLED}
		ifeq ($(OLED), $(filter $(OLED), LEFTCAT RIGHTCAT CAT))
			SRC += oled/oled-icons.c oled/oled-bongocat.c
		else ifeq ($(OLED), $(filter $(OLED), FELIX LUNA))
			SRC += oled/oled-icons.c oled/oled-luna.c
		endif
	endif
	ifneq ($(strip $(KB)),)
		RGB_MATRIX_ENABLE = yes
		RGB_MATRIX_CUSTOM_USER = yes
		SRC += rgb/rgb-matrix.c
		OPT_DEFS += -D${KB}
	endif
	ifeq ($(strip $(KB)), $(strip $(OLED)))
		LTO_ENABLE = no
	endif
endif
