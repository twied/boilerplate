#!/bin/bash
for file in ../examples/*.lang0
do
    basename=$(basename "$file" ".lang0")
    echo testing $basename
    echo -e "\tlexer";  ../language0 lex "$file" -- | diff -u "$basename".lex -
    echo -e "\tdot";    ../language0 dot "$file" -- | diff -u "$basename".dot -
    echo -e "\tcc0";    ../language0 cc0 "$file" -- | diff -u "$basename".cc0 -
    echo -e "\tas0";    ../language0 as0 "$basename".cc0 -- | diff -u "$basename".as0 -
    echo -e "\trun";    ../language0 run "$basename".as0 -- | diff -u "$basename".run -
    echo -e "\tg32";    ../language0 g32 "$file" -- | diff -q "$basename".g32 -
    echo -e "\tgcc";    gcc -m32 -o "$basename".elf -x assembler "$basename".g32
    echo -e "\telf";    ./"$basename".elf | diff -u "$basename.run" -
    rm -f "$basename".elf
done
echo done
