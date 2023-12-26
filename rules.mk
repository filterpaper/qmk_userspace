# Disable unused features
MAGIC_ENABLE = no
CONSOLE_ENABLE = no
UNICODE_ENABLE = no
GRAVE_ESC_ENABLE = no
SPACE_CADET_ENABLE = no

# Enable common features
LTO_ENABLE = yes
COMBO_ENABLE = yes
EXTRAKEY_ENABLE = yes
MOUSEKEY_ENABLE = yes
BOOTMAGIC_ENABLE = yes
SWAP_HANDS_ENABLE = yes

VPATH += $(USER_PATH)/autocorrect $(USER_PATH)/oled $(USER_PATH)/rgb
INTROSPECTION_KEYMAP_C = filterpaper.c
MAKECMDGOALS = uf2-split-$(SPLIT)
SRC += autocorrect.c

ifeq ($(strip $(RGB_MATRIX_ENABLE)), yes)
    RGB_MATRIX_CUSTOM_USER = yes
    SRC += rgb-matrix.c
endif

ifeq ($(strip $(OLED_ENABLE)), yes)
    ifneq ($(strip $(OLED)),)
        OPT_DEFS += -D$(OLED)
        SRC += oled-icons.c oled-luna.c
    else
        SRC += oled-icons.c oled-bongocat.c
    endif
endif
