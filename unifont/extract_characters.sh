#!/bin/bash

if [[ $1 == "" ]]; then
  characters="ÄÖÜäöüßáíéóúñабвьыъгдежзийклюмнопрстуфхцчшщэяあいぃえおうかがきぎけげこごくぐさざしじせぜそぞすずただちつづってでとどなにねのぬまみめもむらりれろるはばぱひびぴへべぺほぼぽふぶぷやゃゆゅよょんわ�"
else
  characters=$1
fi

output_dir=../filesystem/
if [[ ! -d $output_dir ]]; then
  mkdir $output_dir
fi

output_file="${output_dir}reduced_unifont.hex"
if [[ -e "$output_file" ]]; then
  echo "$output_file already exists, move it or delete it to continue."
  [[ "$0" == "$BASH_SOURCE" ]] && exit 1 || return 1
fi

unifont_naming_convention=unifont*.hex
unifont_file=$(ls $unifont_naming_convention)
if [[ $unifont_file == "" ]]; then
  echo "Could not find unifont file. Make sure it is in this directory and follows the naming convention $unifont_naming_convention."
  [[ "$0" == "$BASH_SOURCE" ]] && exit 1 || return 1
fi

declare -a character_codepoints
for (( i=0; i<${#characters}; i++ )); do
  character="${characters:$i:1}"
  character_codepoints+=($(echo -n "$character" | iconv -f utf8 -t utf32be | xxd -p | sed -r 's/^0+/0x/' | xargs printf "%04X"))
done

readarray -t sorted_character_codepoints < <(printf '%s\0' "${character_codepoints[@]}" | sort -z | xargs -0n1)

for codepoint in ${sorted_character_codepoints[@]}; do
  echo "$(printf %b\\n \\U$codepoint):U+$codepoint"
  grep "$codepoint:" $unifont_file | tee -a $output_file
done;
