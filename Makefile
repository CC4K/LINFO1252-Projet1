all: philo prod-conso read-write

philo: philosophers.c
	gcc philosophers.c -lpthread -o philo_exe.o

prod-conso: producer_consumer.c
	gcc producer_consumer.c -lpthread -o prod-conso_exe.o

read-write: reader_writer.c
	gcc reader_writer.c -lpthread -o read-write_exe.o

clean:
	rm -rf *.o

help:
	@echo \> 'make help' \	\  \: show this list of commands
	@echo \> 'make all'  \  \  \  \ : compiles all files
	@echo \> 'make philo' \	 \ : compiles philosophers program
	@echo \> 'make prod-conso' \: compiles producer-consumer program
	@echo \> 'make read-write'\ : compiles readers-writers program
	@echo \> 'make clean'\   \   \  : cleans object files

.PHONY: clean