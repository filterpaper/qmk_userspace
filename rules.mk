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
RGB_MATRIX_ENABLE = no
SPACE_CADET_ENABLE = no

# Common features
LTO_ENABLE = yes
EXTRAKEY_ENABLE = yes
MOUSEKEY_ENABLE = yes
BOOTLOADER = atmel-dfu
BOOTMAGIC_ENABLE = lite
OPT_DEFS += -DCAPSWORD_ENABLE
# OPT_DEFS += -DONESHOT_MODTAP_ENABLE

# Main source file
SRC += filterpaper.c

# Settings for Planck
ifeq ($(PLATFORM), CHIBIOS)
	LTO_ENABLE = no
	BOOTLOADER = stm32-dfu
endif

ifneq ($(KEYBOARD),$(filter $(KEYBOARD), crkbd/rev1))
	RGB_MATRIX_ENABLE = yes
	RGB_MATRIX_CUSTOM_USER = yes
	SRC += rgb-matrix.c
endif

# Corne keyboard features
# Primary tap-driven cat with secondary mod status and RGB
ifeq ($(KEYBOARD) $(findstring T,$(CAT)), crkbd/rev1 T)
	OLED_DRIVER_ENABLE = yes
	RGB_MATRIX_ENABLE = yes
	RGB_MATRIX_CUSTOM_USER = yes
	SRC += oled-icons.c oled-bongocat.c rgb-matrix.c
	OPT_DEFS += -D${CAT}CAT
# RGB only without OLED support
else ifeq ($(KEYBOARD) $(RGB), crkbd/rev1 yes)
	RGB_MATRIX_ENABLE = yes
	RGB_MATRIX_CUSTOM_USER = yes
	SRC += rgb-matrix.c
# Primary status with secondary WPM-driven animation
else ifeq ($(KEYBOARD) $(WPM), crkbd/rev1 yes)
	WPM_ENABLE = yes
	OLED_DRIVER_ENABLE = yes
	SRC += oled-bongocat.c
	OPT_DEFS += -D${CAT}CAT
	# Primary OLED option
	ifneq ($(DOG),)
		SRC += oled-luna.c
		OPT_DEFS += -D${DOG}
	else
		SRC += oled-icons.c
	endif
# Minimal default
else ifeq ($(strip $(KEYBOARD)), crkbd/rev1)
#	COMBO_ENABLE = yes
endif
