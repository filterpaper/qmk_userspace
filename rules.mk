# Disable unused features
MAGIC_ENABLE = no
UNICODE_ENABLE = no
GRAVE_ESC_ENABLE = no
SPACE_CADET_ENABLE = no

# Enable common features
LTO_ENABLE = yes
COMBO_ENABLE = yes
SWAP_HANDS_ENABLE = yes

VPATH += $(USER_PATH)/features
INTROSPECTION_KEYMAP_C = filterpaper.c
SRC += autocorrect.c

ifneq ($(strip $(CONVERT_TO)),)
    EEPROM_DRIVER = transient
    override TARGET := $(subst /,_,$(KEYBOARD))_$(strip $(CONVERT_TO))
    ifeq ($(strip $(SPLIT)), $(filter $(SPLIT), left right))
        MAKECMDGOALS = uf2-split-$(strip $(SPLIT))
        override TARGET := $(TARGET)_$(strip $(SPLIT))
    endif
endif

ifeq ($(strip $(RGB_MATRIX_ENABLE)), yes)
    RGB_MATRIX_CUSTOM_USER = yes
    SRC += rgb_matrix.c
endif

ifeq ($(strip $(OLED_ENABLE)), yes)
    ifneq ($(strip $(OLED)),)
        OPT_DEFS += -D$(strip $(OLED))
        SRC += oled_icons.c oled_luna.c
    else
        SRC += oled_icons.c oled_bongocat.c
    endif
endif