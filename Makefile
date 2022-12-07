all: ph pc rw

ph: philosophes.c
	gcc philosophes.c -lpthread -o philosophes_exe.o

pc: producteurs_consommateurs.c
	gcc producteurs_consommateurs.c -lpthread -o producteurs_consommateurs_exe.o

rw: lecteurs_ecrivains.c
	gcc lecteurs_ecrivains.c -lpthread -o lecteurs_ecrivains_exe.o

test: all
	make all
	./experiments.sh
	make clean

plot:
	./plot.py

clean:
	rm -rf *.o

help:
	@echo \> 'make help'\  \: show this list of commands
	@echo \> 'make (all)'\ \: compile all .c files
	@echo \> 'make ph' \ \  : compile philosophes.c
	@echo \> 'make pc' \ \  : compile producteurs_consommateurs.c
	@echo \> 'make rw' \ \  : compile lecteurs_ecrivains.c
	@echo \> 'make test'\ \ : compile all .c files, fill .csv files with their execution times \then clean .o files
	@echo \> 'make plot'\ \ : create graphs out of the .csv data and save figures as .png
	@echo \> 'make clean'\ \: remove generated .o files

.PHONY: clean
