import os
import shutil

SOURCE = "patches/User_Setup.h"
TARGET = ".pio/libdeps/esp32dev/TFT_eSPI/User_Setup.h"
BACKUP = ".pio/libdeps/esp32dev/TFT_eSPI/User_Setup.h.backup"

if not os.path.exists(BACKUP):
    shutil.copy(TARGET, BACKUP)
shutil.copy(SOURCE, TARGET)
