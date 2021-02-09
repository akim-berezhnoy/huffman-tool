#!/bin/bash
set -euo pipefail
IFS=$' \t\n'

git checkout feedback
git pull --ff-only upstream master
git push origin
git checkout master
git rebase feedback
git push -f origin
