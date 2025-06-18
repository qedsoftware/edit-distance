import os
import platform
import sys
from sysconfig import get_config_vars

from Cython.Build import cythonize
from packaging.version import parse as parse_version
from setuptools import Extension, setup


def is_platform_mac():
    return sys.platform == "darwin"


extra_compile_args = []
extra_link_args = []
if is_platform_mac():
    if "MACOSX_DEPLOYMENT_TARGET" not in os.environ:
        current_system = platform.mac_ver()[0]
        python_target = get_config_vars().get("MACOSX_DEPLOYMENT_TARGET", current_system)
        target_macos_version = "10.9"
        parsed_macos_version = parse_version(target_macos_version)
        if (
            parse_version(str(python_target))
            < parsed_macos_version
            <= parse_version(current_system)
        ):
            os.environ["MACOSX_DEPLOYMENT_TARGET"] = target_macos_version

    extra_compile_args = ["-std=c++11"]
    extra_link_args = ["-stdlib=libc++"]

extensions = [
    Extension(
        "editdistance.osa",
        sources=[
            "./editdistance/edit_distance_osa.pyx",
            "./editdistance/_edit_distance_osa.cpp",
        ],
        language="c++",
        include_dirs=["./editdistance"],
        extra_compile_args=["-std=c++11"],
    )
]

setup(
    name="editdistance",
    ext_modules=cythonize(extensions),
    zip_safe=False,
)
