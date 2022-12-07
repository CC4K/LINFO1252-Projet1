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
		    tpc_s=$(date +%s%N); ./producteurs_consommateurs_exe.o $n $n; tpc_t=$((($(date +%s%N) - $tpc_s)/1000000))
		    trw_s=$(date +%s%N); ./lecteurs_ecrivains_exe.o $n $n; trw_t=$((($(date +%s%N) - $trw_s)/1000000))
		else
		    # si impair
			m=$(($n+1))
			tpc_s=$(date +%s%N); ./producteurs_consommateurs_exe.o $n $m; tpc_t=$((($(date +%s%N) - $tpc_s)/1000000))
            trw_s=$(date +%s%N); ./lecteurs_ecrivains_exe.o $m $n; trw_t=$((($(date +%s%N) - $trw_s)/1000000))
		fi
		tph_s=$(date +%s%N); ./philosophes_exe.o $n; tph_t=$((($(date +%s%N) - $tph_s)/1000000))

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
# de-commenter pour voir lire les .csv sur le serveur de test
#cat philosophes.csv
#cat producteurs_consommateurs.csv
#cat lecteurs_ecrivains.csv
exit 0
