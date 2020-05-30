#!/bin/bash

function_change()
{
    #sudo apt install exuberant-ctags
    ctags -R .
    # CT + ]  jump
    # CT + T  back
}
