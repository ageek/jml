# Makefile for Jeremy's Machine Learning library
# Copyright (c) 2006 Jeremy Barnes.  All rights reserved.

-include local.mk

default: all
.PHONY: default

BUILD 	?= ./build
ARCH 	?= $(shell uname -m)
OBJ 	:= $(BUILD)/$(ARCH)/obj
BIN 	:= $(BUILD)/$(ARCH)/bin
TESTS 	:= $(BUILD)/$(ARCH)/tests
SRC 	:= .
PWD     := $(shell pwd)

JML_TOP := .

include $(JML_TOP)/arch/$(ARCH).mk

ifeq ($(MAKECMDGOALS),failed)
include .target.mk
failed:
	+make $(FAILED) $(GOALS)
else

include $(JML_TOP)/functions.mk
include $(JML_TOP)/rules.mk
include $(JML_TOP)/python.mk
include $(JML_TOP)/node.mk

include $(JML_TOP)/jml.mk

$(shell echo GOALS := $(MAKECMDGOALS) > .target.mk)
endif