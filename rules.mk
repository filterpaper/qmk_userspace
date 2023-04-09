# Disable unused features
MAGIC_ENABLE = no
CONSOLE_ENABLE = no
UNICODE_ENABLE = no
SPACE_CADET_ENABLE = no

# Enable common features
LTO_ENABLE = yes
COMBO_ENABLE = yes
EXTRAKEY_ENABLE = yes
MOUSEKEY_ENABLE = yes
BOOTMAGIC_ENABLE = yes
SWAP_HANDS_ENABLE = yes

VPATH += $(USER_PATH)/oled $(USER_PATH)/rgb $(USER_PATH)/features
SRC += filterpaper.c combos.c caps_unlock.c autocorrect.c
OPT_DEFS += -DCAPS_UNLOCK -DAUTOCORRECT

ifeq ($(strip $(MCU)), atmega32u4)
	TOP_SYMBOLS = yes
	BOOTLOADER = atmel-dfu
endif

ifneq ($(strip $(CONVERT_TO)),)
	SWAP_HANDS_ENABLE = yes
	DEBOUNCE_TYPE = sym_defer_pk
	ifeq ($(strip $(CONVERT_TO)), kb2040)
		RGB_MATRIX_ENABLE = yes
		RGB_MATRIX_DRIVER = WS2812
		RGB_MATRIX_CUSTOM_USER = yes
		SRC += rgb-matrix.c
		DEBOUNCE_TYPE = asym_eager_defer_pk
	endif
	ifeq ($(strip $(SPLIT)), $(filter $(SPLIT), left right))
		OPT_DEFS += -DINIT_EE_HANDS_$(shell echo $(SPLIT)|tr a-z A-Z)
	endif
endif

ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), crkbd/rev1))
	RGB_MATRIX_ENABLE = yes
	RGB_MATRIX_CUSTOM_USER = yes
	SRC += rgb-matrix.c
	OLED_ENABLE = yes
	ifneq ($(strip $(OLED)),)
		OPT_DEFS += -D$(OLED)
		SRC += oled-icons.c oled-luna.c
	else
		SRC += oled-icons.c oled-bongocat.c
	endif
endif