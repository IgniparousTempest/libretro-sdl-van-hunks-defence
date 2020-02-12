#!/usr/bin/env bash

# This file is part of The RetroPie Project
#
# The RetroPie Project is the legal property of its developers, whose names are
# too numerous to list here. Please refer to the COPYRIGHT.md file distributed with this source.
#
# See the LICENSE.md file at the top-level directory of this distribution and
# at https://raw.githubusercontent.com/RetroPie/RetroPie-Setup/master/LICENSE.md
#

rp_module_id="lr-vanhunksdefence"
rp_module_desc="Van Hunks Defence - Naval Themed, Multiplayer Missile Command Clone"
rp_module_help="https://github.com/IgniparousTempest/libretro-sdl-van-hunks-defence"
rp_module_licence="GPL3 https://raw.githubusercontent.com/IgniparousTempest/libretro-sdl-van-hunks-defence/master/LICENSE"
rp_module_section="exp"

function depends_lr-vanhunksdefence() {
    getDepends cmake
}

function sources_lr-vanhunksdefence() {
    gitPullOrClone "$md_build" https://github.com/IgniparousTempest/libretro-sdl-van-hunks-defence.git
}

function build_lr-vanhunksdefence() {
    cmake .
    make
    md_ret_require="$md_build/vanhunksdefence_libretro.so"
}

function install_lr-vanhunksdefence() {
    md_ret_files=(
        'vanhunksdefence_libretro.so'
        'assets'
        'LICENSE'
        'README.md'
    )
}

function configure_lr-vanhunksdefence() {
    setConfigRoot "ports"

    addPort "$md_id" "vanhunksdefence" "Van Hunks Defence" "$md_inst/vanhunksdefence_libretro.so"

    ensureSystemretroconfig "ports/vanhunksdefence"
}
