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
INTROSPECTION_KEYMAP_C = combos.c
SRC += autocorrect.c filterpaper.c

ifeq ($(strip $(RGB_MATRIX_ENABLE)), yes)
    RGB_MATRIX_CUSTOM_USER = yes
    SRC += rgb_matrix.c
endif

ifneq ($(strip $(CONVERT_TO)),)
    EEPROM_DRIVER = transient
    DEBOUNCE_TYPE = asym_eager_defer_pk
    ifneq ($(filter left right, $(strip $(SPLIT))),)
        _SUFFIX := _$(strip $(SPLIT))
        MAKECMDGOALS = uf2-split-$(strip $(SPLIT))
    endif
    override TARGET := $(subst /,_,$(KEYBOARD))_$(strip $(CONVERT_TO))$(_SUFFIX)
endif
