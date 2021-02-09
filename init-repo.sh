#!/bin/bash
set -euo pipefail
IFS=$' \t\n'

git fetch upstream
git branch feedback upstream/master
git push -u origin feedback:feedback
git checkout feedback
git branch -f master feedback
git checkout master
git push -f origin master
