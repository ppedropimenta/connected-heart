"""
Adds the MinGW toolchain installed via `platform_packages` to PATH for the
native (host) build.

The `native` platform builder looks for `gcc`/`g++` on PATH; on Windows
there is usually no system compiler, so this makes `pio test -e native`
work out of the box once `platformio/toolchain-gccmingw32` has been
fetched, without requiring anyone to edit their PATH by hand.
"""

import os

Import("env")

toolchain_dir = env.PioPlatform().get_package_dir("toolchain-gccmingw32")

if toolchain_dir:
    toolchain_bin = os.path.join(toolchain_dir, "bin")

    # Makes gcc/g++ discoverable while SCons builds the test binary.
    env.PrependENVPath("PATH", toolchain_bin)

    # `pio test` later executes that binary in this same Python process; it
    # dynamically links the MinGW runtime DLLs (libstdc++-6.dll etc.), which
    # only PATH lets it find at that point, since they are not copied next
    # to the executable.
    os.environ["PATH"] = toolchain_bin + os.pathsep + os.environ.get("PATH", "")

    # `build_flags` only reaches the compiler, not the linker (SCons'
    # ParseFlags puts a plain "-static" in CCFLAGS), so the binary would
    # still end up dynamically linked against those runtime DLLs without
    # this. Linking them in statically avoids depending on PATH at run time
    # at all.
    env.Append(LINKFLAGS=["-static"])
