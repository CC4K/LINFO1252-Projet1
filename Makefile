all: philo prod_conso read_write

philo: philosophes.c
	gcc philosophes.c -lpthread -o philosophes_exe.o

prod_conso: producteurs_consommateurs.c
	gcc producteurs_consommateurs.c -lpthread -o producteurs_consommateurs_exe.o

read_write: lecteurs_ecrivains.c
	gcc lecteurs_ecrivains.c -lpthread -o lecteurs_ecrivains_exe.o

plot:
	./testing_script
	./plot.py

clean:
	rm -rf *.o

help:
	@echo \> 'make help' \	\  \: shows this list of commands
	@echo \> 'make all'  \  \  \  \ : compiles all .c files
	@echo \> 'make philo' \	 \ : compiles philosophes.c
	@echo \> 'make prod-conso' \: compiles producteurs_consommateurs.c
	@echo \> 'make read-write'\ : compiles lecteurs_ecrivains.c
	@echo \> 'make plot'\  \  \  \ : writes programs execution times \in csv files and plots them
	@echo \> 'make clean'\   \   \  : removes generated object files

.PHONY: clean
