TARGET := 3DS

NAME := 3DSync

BUILD_DIR := build
OUTPUT_DIR := output
INCLUDE_DIRS := include
SOURCE_DIRS := source
ROMFS_DIR := romfs

LIBRARY_DIRS += $(DEVKITPRO)/libctru $(DEVKITPRO)/portlibs/armv6k $(DEVKITPRO)/portlibs/3ds
LIBRARIES += curl mbedtls mbedx509 mbedcrypto z ctru m

EXTRA_OUTPUT_FILES := 

BUILD_FLAGS := -Wno-format-truncation -DVERSION_STRING="\"`git describe --tags --abbrev=0`\"" -DREVISION_STRING="\"`git rev-parse --short HEAD``git diff-index --quiet HEAD -- || echo ' (dirty)'`\""

VERSION_PARTS := $(subst ., ,$(shell git describe --tags --abbrev=0))

VERSION_MAJOR := $(word 1, $(VERSION_PARTS))
VERSION_MINOR := $(word 2, $(VERSION_PARTS))
VERSION_MICRO := $(word 3, $(VERSION_PARTS))

DESCRIPTION := Sync your saves
AUTHOR := Kyraminol

PRODUCT_CODE := CTR-K-SYNC
UNIQUE_ID := 0xF5555

BANNER_AUDIO := meta/audio_3ds.wav
BANNER_IMAGE := meta/banner_3ds.png
ICON := meta/icon_3ds.png

# INTERNAL #

include buildtools/make_base
