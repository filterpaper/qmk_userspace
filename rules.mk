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

# Common features
EXTRAKEY_ENABLE = yes
BOOTLOADER = atmel-dfu
BOOTMAGIC_ENABLE = lite
OPT_DEFS += -DCAPSWORD_ENABLE

# Main source file
SRC += filterpaper.c

# Exclude LTO for Planck
ifneq ($(PLATFORM), CHIBIOS)
	LTO_ENABLE = yes
endif

ifneq ($(KEYBOARD),$(filter $(KEYBOARD), crkbd/rev1/common))
	RGB_MATRIX_ENABLE = yes
	SRC += rgb-matrix.c
	RGB_MATRIX_CUSTOM_USER = yes
endif

# Corne keyboard features
# Primary tap-driven cat with secondary mod status and RGB
ifeq ($(KEYBOARD) $(findstring T,$(CAT)), crkbd/rev1/common T)
	MOUSEKEY_ENABLE = yes
	OLED_DRIVER_ENABLE = yes
	SRC += mod-status.c bongocat.c
	OPT_DEFS += -D${CAT}CAT
	RGB_MATRIX_ENABLE = yes
	SRC += rgb-matrix.c
	RGB_MATRIX_CUSTOM_USER = yes
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
# Minimal default
else ifeq ($(strip $(KEYBOARD)), crkbd/rev1/common)
	MOUSEKEY_ENABLE = yes
endif
