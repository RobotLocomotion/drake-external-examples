#!/bin/bash

ROOT_DIR=$(git rev-parse --show-toplevel)
SETUP_DIR=${ROOT_DIR}/scripts/setup

for example_dir in $(ls -d $ROOT_DIR/drake_*); do
    for system in linux mac; do
        for file in $(find $SETUP_DIR/$system -type f -printf "%P\n"); do
            rel_file_path=$system/$file
            reference_file=$SETUP_DIR/$rel_file_path
            target_file=$example_dir/$rel_file_path

            target_dir=$(dirname $target_file)
            mkdir -p $target_dir
            $SETUP_DIR/utils/append_header.sh $reference_file > $target_file
        done
    done
done

exit 0
