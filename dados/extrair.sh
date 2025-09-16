#!/bin/bash
mkdir csv_bruto -p
for dir in ./dataserver-coids.inpe.br/queimadas/queimadas/focos/csv/anual/EstadosBr_sat_ref/*; do
    estado=$(basename $dir)
    echo "$estado:"
    for file in $dir/*; do
        unzip $file -d ./csv_bruto/$estado/
    done
done
