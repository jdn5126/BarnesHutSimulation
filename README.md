# BarnesHutSimulation
Parallel implementation of Barnes-Hut algorithm using OpenMP.

On the TACC super computer, you can run the simulation with multiple threads using the following
workflow.

Create the following folder `mkdir batch-out`.  This is where the output batch files will exist.

To generate input, call `sbatch inputGen-batch`.  This will output the inputs into `$WORK/input/`

Within the batch-files folder, there are several configurations to run the Parallel Barnes-Hut
Simulation on {sequentual, 8, 16, 32, 64, 68 threads}.  To run all configurations run the command 
`./runallbatches`

The results will be placed in `$WORK/output`.

To see the status of the sbatch jobs create, run `squeue -u <your username>`.