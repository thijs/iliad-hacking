# generate the language specific po files starting from the settings.pot template file 
msginit --input='ersetup.pot' --locale='nl' 
msginit --input='ersetup.pot' --locale='en' 
msginit --input='ersetup.pot' --locale='zh' --no-translator
