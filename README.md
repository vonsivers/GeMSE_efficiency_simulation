GeMSE efficiency simulation
======
Geant4 code to simulate the detection efficiency for a sample in GeMSE

## Usage
```
GeMSE_efficiency -m <macrofile.mac> -g <geometry_file.txt> -o <results_folder>
```

## Output
* simulation_isotope_name/line_energy.root
	* TTree „GeHits“: Tree with information for every hit in the detector
		* TBranch „TotEdep“: Total deposited energy in the detector per event
		* TBranch …

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
* **WARNING: files with same names will be overwritten!**