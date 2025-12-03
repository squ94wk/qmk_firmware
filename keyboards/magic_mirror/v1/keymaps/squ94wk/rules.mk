CONSOLE_ENABLE = yes
COMBO_ENABLE = yes
OS_DETECTION_ENABLE = yes
USER_NAME := squ94wk

# Add user includes and source files manually
VPATH += users/squ94wk
SRC += users/squ94wk/squ94wk.c \
       users/squ94wk/smart_case.c \
       users/squ94wk/history.c \
       users/squ94wk/magickey.c \
       users/squ94wk/metrics.c
