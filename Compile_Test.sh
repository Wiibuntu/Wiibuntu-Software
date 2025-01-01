#!/bin/bash
gcc downloader.c -o downloader `pkg-config --cflags --libs gtk+-3.0`

