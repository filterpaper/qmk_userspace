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
RGBLIGHT_ENABLE = no
BOOTMAGIC_ENABLE = no
SLEEP_LED_ENABLE = no
TAP_DANCE_ENABLE = no
VELOCIKEY_ENABLE = no
SWAP_HANDS_ENABLE = no
RGB_MATRIX_ENABLE = no
SPACE_CADET_ENABLE = no

# Main source file
SRC += filterpaper.c

# Common features
EXTRAKEY_ENABLE = yes
BOOTLOADER = atmel-dfu
BOOTMAGIC_ENABLE = lite

# Custom caps word feature
OPT_DEFS += -DCAPSWORD_ENABLE

ifneq ($(strip $(KEYBOARD)), crkbd/rev1/common)
	RGB_MATRIX_ENABLE = yes
endif

# Exclude LTO for Planck
ifneq ($(PLATFORM), CHIBIOS)
	LTO_ENABLE = yes
endif

# Corne keyboard features
ifeq ($(KEYBOARD) $(TINY), crkbd/rev1/common yes)
	MOUSEKEY_ENABLE = yes
# Primary status with secondary WPM-driven animation
else ifeq ($(KEYBOARD) $(WPM), crkbd/rev1/common yes)
	MOUSEKEY_ENABLE = yes
	WPM_ENABLE = yes
	OLED_DRIVER_ENABLE = yes
	SRC += bongocat.c
	OPT_DEFS += -D${CAT}CAT
	# Primary OLED option
	ifneq ($(DOG),)
		SRC += luna-status.c
		OPT_DEFS += -D${DOG}
	else
		SRC += mod-status.c
	endif
# Primary tap-driven animation with secondary mod status
else ifeq ($(strip $(KEYBOARD)), crkbd/rev1/common)
	MOUSEKEY_ENABLE = yes
	OLED_DRIVER_ENABLE = yes
	OPT_DEFS += -D${CAT}CAT
	SRC += mod-status.c bongocat.c
endif
