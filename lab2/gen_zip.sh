cd doc
pandoc report.md --pdf-engine=xelatex -o report.pdf -V CJKmainfont='Microsoft YaHei' --highlight-style tango  
cd ..
zip -r lab2.zip doc/report.pdf src/*
