# Disable unused features
MAGIC_ENABLE = no
UNICODE_ENABLE = no
GRAVE_ESC_ENABLE = no
SPACE_CADET_ENABLE = no

# Enable common features
LTO_ENABLE = yes
COMBO_ENABLE = yes
SWAP_HANDS_ENABLE = yes

MAKECMDGOALS = uf2-split-$(SPLIT)
VPATH += $(USER_PATH)/features
INTROSPECTION_KEYMAP_C = filterpaper.c
SRC += autocorrect.c

ifeq ($(strip $(RGB_MATRIX_ENABLE)), yes)
    RGB_MATRIX_CUSTOM_USER = yes
    SRC += rgb_matrix.c
endif

ifeq ($(strip $(OLED_ENABLE)), yes)
    ifneq ($(strip $(OLED)),)
        OPT_DEFS += -D$(OLED)
        SRC += oled_icons.c oled_luna.c
    else
        SRC += oled_icons.c oled_bongocat.c
    endif
endif