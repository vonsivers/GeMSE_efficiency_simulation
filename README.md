GeMSE efficiency simulation
======
Geant4 code to simulate the detection efficiency for a sample in GeMSE

## Usage
* You can run the efficiency simulation for all standard isotopes in parallel by
```
./run.sh <geometry_file.txt> <results_folder>
```
* To run a specific macro without parallelization do
```
GeMSE_efficiency -m <macrofile.mac> -g <geometry_file.txt> -o <results_folder> -f <results_file>
```

## Output
* results_file.root
	* TTree "tree": Tree with information about detection efficiency and branching ratio (emission probability) for every gamma line
		* TBranch "energy": energy of gamma line
		* TBranch "efficiency": detection efficiency
        * TBranch "efficiency_err": statistical uncertainty of detection efficiency
        * TBranch "eff_BR": product of detection efficiency and branching ratio

## macrofile.mac
* contains the information which isotopes or gamma lines are simulated
* for standard analysis use „macros/standard_isotopes.mac“
* for visualization only use „macros/visualization.mac“

## geometry_file.txt
* contains information about sample geometry and material
* for examples see folder example_geometries or refer to the Geant4 documentation (http://geant4.cern.ch/collaboration/working_groups/geometry/docs/textgeom/textgeom.pdf)
* **it is important that the sample volume is named “sample” otherwise the simulated decays/gammas will not be confined to this shape!**

## results_folder
* folder where result files are written. 