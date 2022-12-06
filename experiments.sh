#!/usr/bin/bash
echo "Philos,1,2,3,4,5" > philosophes.csv
echo "Prod-conso,1,2,3,4,5" > producteurs_consommateurs.csv
echo "Lect-ecriv,1,2,3,4,5" > lecteurs_ecrivains.csv
for n in 1 2 4 8 16 32 64; do
    echo -n "$n," >> philosophes.csv
    echo -n "$n," >> producteurs_consommateurs.csv
    echo -n "$n," >> lecteurs_ecrivains.csv
	for i in $(seq 5); do
		if [ $(($n%2)) -eq 0 ]; then
		    # si pair
		    # ts=$(date +%s%N) ; my_command ; tt=$((($(date +%s%N) - $ts)/1000000)) ; echo "Time taken: $tt milliseconds"
		    tpc_s=$(date +%s%N); ./producteurs_consommateurs_exe.o $n $n; tpc_t=$((($(date +%s%N) - $tpc_s)/1000000))
		    trw_s=$(date +%s%N); ./lecteurs_ecrivains_exe.o $n $n; trw_t=$((($(date +%s%N) - $trw_s)/1000000))
			#time_prod_conso=$(/usr/bin/time -f "%e" ./producteurs_consommateurs_exe.o $n $n 2>&1 | tail -n 1)
			#time_read_write=$(/usr/bin/time -f "%e" ./lecteurs_ecrivains_exe.o $n $n 2>&1 | tail -n 1)
		else
		    # si impair
			m=$(($n+1))
			tpc_s=$(date +%s%N); ./producteurs_consommateurs_exe.o $n $m; tpc_t=$((($(date +%s%N) - $tpc_s)/1000000))
            trw_s=$(date +%s%N); ./lecteurs_ecrivains_exe.o $m $n; trw_t=$((($(date +%s%N) - $trw_s)/1000000))
			#time_prod_conso=$(/usr/bin/time -f "%e" ./producteurs_consommateurs_exe.o $n $m 2>&1 | tail -n 1)
			#time_read_write=$(/usr/bin/time -f "%e" ./lecteurs_ecrivains_exe.o $m $n 2>&1 | tail -n 1)
		fi
		tph_s=$(date +%s%N); ./philosophes_exe.o $n; tph_t=$((($(date +%s%N) - $tph_s)/1000000))
		#time_philo=$(/usr/bin/time -f "%e" ./philosophes_exe.o $n 2>&1 | tail -n 1)

		if [ $i -eq 5 ]; then
            echo $tph_t >> philosophes.csv
			echo $tpc_t >> producteurs_consommateurs.csv
			echo $trw_t >> lecteurs_ecrivains.csv
        else
        	echo -n "$tph_t," >> philosophes.csv
			echo -n "$tpc_t," >> producteurs_consommateurs.csv
			echo -n "$trw_t," >> lecteurs_ecrivains.csv
        fi
	done
done
exit 0
