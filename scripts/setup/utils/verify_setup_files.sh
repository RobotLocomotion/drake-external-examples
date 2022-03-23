#!/bin/bash

# In order to make each example folder self-contained, we need to have a copy
# of a few linux/mac scripts inside them.
#
# This utility script verifies that the copy of the files in each example directory
# matches the original file in the upstream scripts/setup directory.

set -euo pipefail

ROOT_DIR=$(cd -- $(dirname $0)/../../.. && pwd)
SETUP_DIR=${ROOT_DIR}/scripts/setup

for example_dir in $(ls -d $ROOT_DIR/drake_*); do
    for system in linux mac; do
        for file in $(find $SETUP_DIR/$system -type f -printf "%P\n"); do
            rel_file_path=$system/$file
            reference_file=$SETUP_DIR/$rel_file_path
            target_file=$example_dir/$rel_file_path

            if [[ -f $target_file ]]; then
                diff=$(diff $target_file <($SETUP_DIR/utils/append_header.sh $reference_file))
                if [[ $diff ]]; then
                    echo File is not up to date: $target_file
                    echo Run update_setup_files.sh to fix the issue.
                    exit 1
                fi
            else
                echo File does not exist: $target_file
                echo Run update_setup_files.sh to fix the issue.
                exit 1
            fi
        done
    done
done

exit 0
