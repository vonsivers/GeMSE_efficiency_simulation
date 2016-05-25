#!/bin/bash
GeMSE_efficiency -g $1 -o $2 -m macros/Pb214.mac -f simulated_efficiencies_Pb214.root & >log_Pb214.txt
GeMSE_efficiency -g $1 -o $2 -m macros/Bi214.mac -f simulated_efficiencies_Bi214.root & >log_Bi214.txt
wait
GeMSE_efficiency -g $1 -o $2 -m macros/Ac228.mac -f simulated_efficiencies_Ac228.root & >log_Ac228.txt
GeMSE_efficiency -g $1 -o $2 -m macros/Pb212.mac -f simulated_efficiencies_Pb212.root & >log_Pb212.txt