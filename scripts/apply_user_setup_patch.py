import subprocess as sp

try:
    sp.check_output(
        [
            "patch",
            "-N",
            ".pio/libdeps/esp32dev/TFT_eSPI/User_Setup.h",
            "patches/override_user_setup.h.diff",
        ]
    )
except sp.CalledProcessError as err:
    print(err)
