#!/usr/bin/env bash

VG_REPO_ADDR=${VG_REPO_ADDR:="git://sourceware.org/git/valgrind.git"}
VG_BRANCH=${VG_BRANCH:="VALGRIND_3_15_0"}
VG_REPO_DEST=${VG_REPO_DEST:="valgrind"}
CURR_DIR=${PWD} 

echo "[1] Cloning repository"
git clone ${VG_REPO_ADDR} -b ${VG_BRANCH} ${VG_REPO_DEST}

echo "[2] Copying tools files"
cd ${VG_REPO_DEST}
cp -r ${CURR_DIR}/exp-memviz .

echo "[3] Registering tool"
patch < ${CURR_DIR}/register-exp-memviz-tool.patch

echo "You can now build valgind."