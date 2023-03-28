cd doc
pandoc report.md --pdf-engine=xelatex -o report.pdf -V CJKmainfont='Microsoft YaHei' --highlight-style tango  
cd ..
zip -r lab1.zip doc/report.pdf src/Makefile src/multibootHeader.S src/multibootHeader.bin src/multibootHeader.ld
