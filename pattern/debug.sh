#!/bin/bash

#DEBUG_LOG=""

DEBUG_LEVEL=2
PRINT_COLOR_TAIL="\033[0m"
SHELL_NAME=$(basename $0)
PRINT_RED="\033[31m"
PRINT_YELLOW="\033[33m"
PRINT_BLUE="\033[34m"

if [ -n "${DEBUG_LOG}" ]; then
    mkdir -p $(dirname ${DEBUG_LOG})
fi

error()
{
    if [ "$DEBUG_LEVEL" -gt 0 ]; then
        echo -e "${PRINT_RED}[error:$$:${SHELL_NAME}:${FUNCNAME[1]}] $@${PRINT_COLOR_TAIL}"
        if [ -n "${DEBUG_LOG}" ]; then
            echo -e "[error:$$:${SHELL_NAME}:${FUNCNAME[1]}] $@" >> ${DEBUG_LOG}
        fi
    fi
}

warn()
{
    if [ "$DEBUG_LEVEL" -gt 1 ]; then
        echo -e "${PRINT_YELLOW}[warn:$$:${SHELL_NAME}:${FUNCNAME[1]}] $@${PRINT_COLOR_TAIL}"
        if [ -n "${DEBUG_LOG}" ]; then
            echo -e "[warn:$$:${SHELL_NAME}:${FUNCNAME[1]}] $@" >> ${DEBUG_LOG}
        fi
    fi
}

info()
{
    if [ "$DEBUG_LEVEL" -gt 2 ]; then
        echo -e "${PRINT_BLUE}[info:$$:${SHELL_NAME}:${FUNCNAME[1]}] $@${PRINT_COLOR_TAIL}"
        if [ -n "${DEBUG_LOG}" ]; then
            echo -e "[info:$$:${SHELL_NAME}:${FUNCNAME[1]}] $@" >> ${DEBUG_LOG}
        fi
    fi
}

debug()
{
    if [ "$DEBUG_LEVEL" -gt 3 ]; then
        echo -e "[debug:$$:${SHELL_NAME}:${FUNCNAME[1]}] $@" | tee ${DEBUG_LOG}
    fi
}

debug_test()
{
    DEBUG_LEVEL=10
    error "this is a error info"
    warn "this is a warn info"
    info "this is a info info"
    debug "this is a debug info"
}
