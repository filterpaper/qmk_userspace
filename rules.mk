# Disable unused features
VIA_ENABLE = no
WPM_ENABLE = no
NKRO_ENABLE = no
AUDIO_ENABLE = no
STENO_ENABLE = no
LEADER_ENABLE = no
UNICODE_ENABLE = no
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
MOUSEKEY_ENABLE = no
TERMINAL_ENABLE = no
KEY_LOCK_ENABLE = no
BOOTMAGIC_ENABLE = no
SLEEP_LED_ENABLE = no
TAP_DANCE_ENABLE = no
VELOCIKEY_ENABLE = no
SWAP_HANDS_ENABLE = no
SPACE_CADET_ENABLE = no
RGBLIGHT_ENABLE = no
RGB_MATRIX_ENABLE = no

# Main source file
SRC += filterpaper.c

# Common features
EXTRAKEY_ENABLE = yes
GRAVE_ESC_ENABLE = yes

# Exclude LTO for Planck
ifneq ($(PLATFORM), CHIBIOS)
	LTO_ENABLE = yes
endif

# Boards with LEDs
ifeq ($(KEYBOARD),$(filter $(KEYBOARD), bm40hsrgb planck/rev6 boardsource/the_mark))
	BOOTMAGIC_ENABLE = lite
	RGB_MATRIX_ENABLE = yes
endif

# Corne keyboard features
ifeq ($(strip $(KEYBOARD)), crkbd/rev1/common)
	BOOTLOADER = atmel-dfu
	MOUSEKEY_ENABLE = yes
	ifneq ($(CORNELP), yes)
		WPM_ENABLE = yes
		OLED_DRIVER_ENABLE = yes

		# primary OLED selection
		ifeq ($(LUNA), yes)
			SRC += luna-status.c
			OPT_DEFS += -DLUNA
		else ifeq ($(FELIX), yes)
			SRC += luna-status.c
			OPT_DEFS += -DLUNA -DFELIX
		else
			SRC += mod-status.c
		endif

		# secondary OLED selection
		SRC += bongocat.c
		ifeq ($(SLIMCAT), yes)
			OPT_DEFS += -DSLIMCAT
		endif
		ifeq ($(RIGHTCAT), yes)
			OPT_DEFS += -DRIGHTCAT
		else ifeq ($(LEFTCAT), yes)
			OPT_DEFS += -DLEFTCAT
		endif

	endif
endif
