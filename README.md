# LINFO1252-Projet 1

### Programmation multi-threadée et évaluation de performances

###

## Repository content

This is only the first part of the project but it is enough for an interesting analysis of the results.

The `.c` files are the source files containing the multi-threaded programs we had to evaluate

`experiments.sh` is a bash script for evaluating the programs. 5 runs are recorded for 1, 2, 4, 8, 16, 32 and 64 threads (off course it means it needs to be run on a 32 core processor machine)

The `.csv` files contains the data resulting from `experiments.sh`

`plot.py` is a Python script that will take the data from the `.csv` and plot them into graphs

The `.png` files are the graphs resulting from `plot.py`

Finally, the Makefile is where the make commands are (compiling, cleaning and running the scripts) 

## Makefile commands

Here are all the possible commands you can run from the root of the project :

-`make help`: show this list of commands

-`make (all)`: compile all .c files

-`make ph`: compile philosophes.c

-`make pc`: compile producteurs_consommateurs.c

-`make rw`: compile lecteurs_ecrivains.c

-`make test`: compile all .c files, fill .csv files with their execution times then clean .o files

-`make plot`: create graphs out of the .csv data and save figures as .png

-`make clean`: remove generated .o files
