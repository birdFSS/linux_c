#!/bin/bash
cat test
delete_tag="$(sed '3,6s/<[^>]*>//g' test)"
echo $delete_tag
