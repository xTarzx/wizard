#!/usr/bin/env python3.11
import os
import sys
from devox import Devox as DVX


def main():
    rebuild = False
    sys.argv.pop(0)
    if len(sys.argv) > 0:
        arg = sys.argv.pop(0)

        match arg:
            case "-a":
                rebuild = True
            case _:
                print(f"ERROR: unknown parameter '{arg}'")
                exit(1)

    dvx = DVX("WiZard_devox")
    jansson_root = os.path.join(dvx.project_root, "extern/jansson/build")
    imgui_root = os.path.join(dvx.project_root, "extern/imgui")
    glfw_root = os.path.join(dvx.project_root, "extern/glfw")

    dvx.add_cflag("-Wall", "-Werror", "-std=c++17")

    dvx.add_src("src/main.cpp",
                "src/pilot.cpp",
                "src/udp.cpp",
                "src/wizard.cpp",
                os.path.join(imgui_root, "imgui.cpp"),
                os.path.join(imgui_root, "imgui_draw.cpp"),
                os.path.join(imgui_root, "imgui_tables.cpp"),
                os.path.join(imgui_root, "imgui_widgets.cpp"),
                os.path.join(imgui_root, "backends/imgui_impl_glfw.cpp"),
                os.path.join(imgui_root, "backends/imgui_impl_opengl3.cpp"),
                )

    dvx.add_inc_dir_rel(
        "include",
        os.path.join(jansson_root, "include"),
        os.path.join(glfw_root, "include"),
        imgui_root,
        os.path.join(imgui_root, "backends"),
    )

    dvx.link_lib("jansson", "GL", "glfw")
    dvx.add_link_dir_rel(
        os.path.join(jansson_root, "lib"),
        os.path.join(glfw_root, "build/src")
    )

    dvx.build(rebuild, thread=True)


if __name__ == "__main__":
    main()
