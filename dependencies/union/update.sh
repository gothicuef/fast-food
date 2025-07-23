#!/usr/bin/env bash
# shellcheck disable=SC2164
cd gothic-api
git fetch origin
git checkout -d origin/main
cd ..

cd union-api
git fetch origin
git checkout -d origin/main
cd ..