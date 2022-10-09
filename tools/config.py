""" 
Inspired from MUNIX 
https://github.com/cute-engineewing/munix/blob/main/tools/config.py
"""

import kconfiglib
import menuconfig
import argparse

kconfig = kconfiglib.Kconfig("Kconfig")


def generate_header(dest):
    kconfig.load_config(".config")
    kconfig.write_autoconf(dest, "#pragma once\n")


def generate_make(dest):
    kconfig.load_config(".config")
    kconfig.write_config(dest)


def defconfig():
    kconfig.load_config(".config")
    kconfig.write_config()


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--genheader", action="store", default=None)
    parser.add_argument("--genmake", action="store", default=None)
    parser.add_argument("--menuconfig", action="store_true")
    parser.add_argument("--defconfig", action="store_true")
    args = parser.parse_args()

    if args.genheader is not None:
        generate_header(args.genheader)

    if args.genmake is not None:
        generate_make(args.genmake)

    if args.menuconfig:
        menuconfig.menuconfig(kconfig)

    if args.defconfig:
        defconfig()
