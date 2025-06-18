#!/bin/bash

set -e

echo "===> black"
black --check .

echo "===> isort"
isort --check .

echo "===> flake8"
flake8 .
