#!/bin/bash
echo "### starting simulation runs 1-10 ..."
GeMSE_efficiency -g $1 -o $2 -m macros/1001keV.mac -f simulated_efficiencies_1001keV >log/log_1001keV.txt & 
GeMSE_efficiency -g $1 -o $2 -m macros/63keV.mac -f simulated_efficiencies_63keV >log/log_63keV.txt & 
GeMSE_efficiency -g $1 -o $2 -m macros/93keV.mac -f simulated_efficiencies_93keV >log/log_93keV.txt & 
GeMSE_efficiency -g $1 -o $2 -m macros/Pb214.mac -f simulated_efficiencies_Pb214 >log/log_Pb214.txt & 
GeMSE_efficiency -g $1 -o $2 -m macros/Bi214.mac -f simulated_efficiencies_Bi214 >log/log_Bi214.txt &
GeMSE_efficiency -g $1 -o $2 -m macros/Ac228.mac -f simulated_efficiencies_Ac228 >log/log_Ac228.txt &
GeMSE_efficiency -g $1 -o $2 -m macros/Pb212.mac -f simulated_efficiencies_Pb212 >log/log_Pb212.txt &
GeMSE_efficiency -g $1 -o $2 -m macros/Bi212.mac -f simulated_efficiencies_Bi212 >log/log_Bi212.txt &
GeMSE_efficiency -g $1 -o $2 -m macros/Tl208.mac -f simulated_efficiencies_Tl208 >log/log_Tl208.txt &
GeMSE_efficiency -g $1 -o $2 -m macros/Cs137.mac -f simulated_efficiencies_Cs137 >log/log_Cs137.txt &
wait
echo "### runs 1-10 finished!"
echo "### starting simulation runs 11-16 ..."
GeMSE_efficiency -g $1 -o $2 -m macros/Co60.mac -f simulated_efficiencies_Co60 >log/log_Co60.txt &
GeMSE_efficiency -g $1 -o $2 -m macros/K40.mac -f simulated_efficiencies_K40 >log/log_K40.txt &
GeMSE_efficiency -g $1 -o $2 -m macros/Mn54.mac -f simulated_efficiencies_Mn54 >log/log_Mn54.txt &
GeMSE_efficiency -g $1 -o $2 -m macros/Na22.mac -f simulated_efficiencies_Na22 >log/log_Na22.txt &
GeMSE_efficiency -g $1 -o $2 -m macros/Al26.mac -f simulated_efficiencies_Al26 >log/log_Al26.txt &
GeMSE_efficiency -g $1 -o $2 -m macros/Sc44.mac -f simulated_efficiencies_Sc44 >log/log_Sc44.txt &
wait
echo "### all simulation runs finished!"
echo "### merging simulation files ..."
merge_simulated_efficiencies $2/simulated_efficiencies_1001keV.root $2/simulated_efficiencies_63keV.root $2/simulated_efficiencies_93keV.root $2/simulated_efficiencies_Ac228.root $2/simulated_efficiencies_Al26.root $2/simulated_efficiencies_Bi212.root $2/simulated_efficiencies_Bi214.root $2/simulated_efficiencies_Co60.root $2/simulated_efficiencies_Cs137.root $2/simulated_efficiencies_K40.root $2/simulated_efficiencies_Mn54.root $2/simulated_efficiencies_Na22.root $2/simulated_efficiencies_Pb212.root $2/simulated_efficiencies_Pb214.root $2/simulated_efficiencies_Sc44.root $2/simulated_efficiencies_Tl208.root $2/simulated_efficiencies_merged
echo "### all simulation files merged to 'simulated_efficiencies_merged.root'"
