# generate the language specific po files
for i in `cat LINGUAS |grep --invert-match "^#"`; do msginit --input="`ls -1 *.pot`" --locale="$i" --no-translator; msgconv -i $i.po -t UTF-8 -o $i.po; done
