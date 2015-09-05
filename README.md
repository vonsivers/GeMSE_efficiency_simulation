#####################################
# GeMSE efficiency simulation
#####################################
simulates the detection efficiency for a sample in GeMSE

usage
	GeMSE_efficiency -m <macrofile.mac> -g <geometry_file.txt> -o <results_folder>

output
	simulation_<isotope_name/line_energy>.root

#######################
# <macrofile.mac>
#######################
contains the information which isotopes or gamma lines are simulated
for standard analysis use „macros/standard_isotopes.mac“
for visualization only use „macros/visualization.mac“

#######################
# <geometry_file.txt>
#######################
contains information about sample geometry and material

#######################
# <results_folder>
#######################
folder where result files are written. 
WARNING: files with same names will be overwritten!