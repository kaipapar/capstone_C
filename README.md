This is code to extract certain fields from CSV files. It is for a specific application and only to be used as an example.

You might need to Download the [CsvParser](https://sourceforge.net/projects/cccsvparser/files/) and [CsvWriter](https://sourceforge.net/projects/cccsvparser/files/) libraries for this to work. Not tested.

compile src: `gcc -I C_lib/CsvWriter/include/ -I C_lib/CsvParser/include/ -o parse_write_csv parse_write_csv.c  parser.o writer.o`
run: `./parse_write_csv filein.csv fileout.csv`
