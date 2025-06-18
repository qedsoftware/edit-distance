#!/bin/bash

echo "===> black"
black .

echo "===> isort"
isort .

echo "===> flake8"
flake8 .
