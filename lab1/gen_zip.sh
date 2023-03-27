pandoc doc/report.md --pdf-engine=xelatex -o doc/report.pdf -V CJKmainfont='Microsoft YaHei' --highlight-style tango  
zip -r lab1.zip doc/report.pdf src/Makefile src/multibootHeader.S src/multibootHeader.bin src/multibootHeader.ld
