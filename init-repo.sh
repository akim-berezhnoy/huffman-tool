#!/bin/bash
# Данный скрипт следует вызвать после первого клонирования
# репозитория. Подробная информация:
# https://cpp-kt.github.io/course/instruction.html

set -euo pipefail
IFS=$' \t\n'

git fetch upstream
git branch feedback upstream/master
git push -u origin feedback:feedback
git checkout feedback
git branch -f master feedback
git checkout master
git push -f origin master
