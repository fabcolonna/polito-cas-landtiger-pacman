#!/bin/bash

# Variables
REPO="fabcolonna/polito-cas-landtiger-lib"
LIB_DIR="./Keil/Libs/Peripherals/"
PATTERN="peripherals-lib.tar.gz"

if ! command -v gh &> /dev/null; then
    echo "Error: GitHub CLI (gh) is required, but not installed."
    exit 1
fi

echo "Authenticating with GitHub CLI..."
gh auth status &> /dev/null || gh auth login

if ! gh repo view "$REPO" &> /dev/null; then
    echo "Error: You don't have access to the repository $REPO."
    exit 1
fi

[[ -d $LIB_DIR ]] && rm -rf $LIB_DIR
mkdir -p $LIB_DIR
gh release download --repo $REPO --pattern $PATTERN --dir $LIB_DIR
tar --strip-components=1 --exclude='._*' -xf $LIB_DIR$PATTERN -C $LIB_DIR
rm $LIB_DIR$PATTERN
echo "Done."