#!/usr/bin/env -S uv run --script
#
# /// script
# requires-python = ">3.12"
# dependencies = [
#   "requests",
# ]
# ///
#
#
# This script can be run with uv.
# https://docs.astral.sh/uv/#installation
#


import requests as req


def main():
    PALFFYGASSE_DIVA = 60200988

    BASE_URL = "http://www.wienerlinien.at/ogd_realtime/monitor?diva={}"

    resp = req.get(BASE_URL.format(PALFFYGASSE_DIVA))

    if resp.status_code != 200:
        raise RuntimeError(f"Got status {resp.status_code}")
    if not resp.json()["message"]["value"] == "OK":
        raise RuntimeError(f"Got message {resp.json()['message']}")

    print(resp.text)


if __name__ == "__main__":
    main()
