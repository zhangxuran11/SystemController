INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    InputMethodPkg/soft_keyboard_symbol.h \
    InputMethodPkg/soft_keyboard_numbers.h \
    InputMethodPkg/soft_keyboard_lowercase_letter.h \
    InputMethodPkg/soft_keyboard_complete.h \
    InputMethodPkg/soft_keyboard_capital_letter.h \
    InputMethodPkg/soft_keyboard.h \
    InputMethodPkg/klineedit.h \
    InputMethodPkg/ktextedit.h

SOURCES += \
    InputMethodPkg/soft_keyboard_symbol.cpp \
    InputMethodPkg/soft_keyboard_numbers.cpp \
    InputMethodPkg/soft_keyboard_lowercase_letter.cpp \
    InputMethodPkg/soft_keyboard_complete.cpp \
    InputMethodPkg/soft_keyboard_capital_letter.cpp \
    InputMethodPkg/soft_keyboard.cpp \
    InputMethodPkg/klineedit.cpp \
    InputMethodPkg/ktextedit.cpp

RESOURCES +=

FORMS +=

OTHER_FILES += \
    qss/mainframe.qss
