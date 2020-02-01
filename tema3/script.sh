#!/bin/bash

CORES=12
REPS=100

for threads in $(seq 1 $CORES)
do
    mean=0

    for rep in $(seq 1 $REPS)
    do
        aux=$(/usr/bin/time -o out -f "%E" mpirun -np $threads ./main ./in/PGM/rorschach.pgm ./rorschach.pgm \
            blur smooth sharpen emboss mean blur smooth sharpen emboss mean \
            && cat out | cut -d ':' -f 2)

        aux=$(bc -l <<<"${aux}")
        mean=$(bc -l <<<"${mean}+${aux}")
    done

    mean=$(bc -l <<<"${mean}/${REPS}")
    mean=$(echo $mean | head -c6)
    echo "    $threads threads: avg $mean sec"
done

rm -r out

exit 0