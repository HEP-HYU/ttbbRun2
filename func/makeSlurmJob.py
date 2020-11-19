import os
import sys
import textwrap
import subprocess

class MakeSlurmJob:
    _slurm_template = textwrap.dedent("""\
        #! /bin/bash
        
        #SBATCH -J {job_name}
        #SBATCH -p {node} 
        #SBATCH -N 1 
        #SBATCH --open-mode=append
        #SBATCH -o %x.out
        #SBATCH -e %x.error
        #SBATCH --ntasks=1
        #SBATCH --mem=1gb
        #SBATCH --cpus-per-task=1
        #SBATCH --comment python
        #SBATCH --hint=compute_bound

        {opts}
        echo {command}
        {command}
    """)
    def __init__(self, year, ntuple_path, base_path):
        self.year = year
        self.ntuple_path = ntuple_path
        self.base_path = base_path
        self.testset = ['TTLJ_PowhegPythia_ttbb', 'Tree_ttbbLepJets_000.root']

    def make_slurm_job(self, is_test=False, is_qcd=False):
        import template
        job_name = str(self.year)+'_plots'
        node = 'gpu'
        opts = 'opt1=$1\nopt2=$2\nopt3=$3\nopt4=$4\nopt5=$5\n\n'
        command = 'python func/runTSelector.py $opt1 $opt2 $opt3 $opt4 $opt5'
        script = MakeSlurmJob._slurm_template.format(job_name=job_name, node=node, opts=opts, command=command)
        script_file = os.path.join(self.base_path, 'job_slurm_'+str(self.year)+'.sh')
        with open(script_file, 'w') as f:
            f.write(script)
        
        from func import saveAndLoadSamples as sls
        load = sls.LoadSamples(self.year, self.base_path+'/samples')
        data = load.get_data()
        samples = load.get_samples()
        extsyst = load.get_extsyst()
        
        samples = data + samples
        
        from func import runTSelector as ts
        #Compile TSelector
        ts.runAna(self.year, self.ntuple_path+'/'+self.testset[0], self.testset[1], "Nosys_Compile", 'False')
        
        if is_test:
            #ts.runAna(self.year, self.ntuple_path+'/'+self.testset[0], self.testset[1], 'Test_ttbb', 'False')
            cmd = ['sbatch',script_file,str(self.year),self.ntuple_path+'/'+self.testset[0],self.testset[1],'Test_ttbb','False']
            subprocess.call(cmd)
        elif is_qcd:
            for sample in samples:
                for item in os.listdir(self.ntuple_path+'/'+sample):
                    cmd = ['sbatch',script_file,str(self.year),self.ntuple_path+'/'+sample,item,'dataDriven_'+sample,'True']
                    subprocess.call(cmd)
                    cmd = ['sbatch',script_file,str(self.year),self.ntuple_path+'/'+sample,item,'dataDriven_'+sample+'__qcdisoup','True']
                    subprocess.call(cmd)
                    cmd = ['sbatch',script_file,str(self.year),self.ntuple_path+'/'+sample,item,'dataDriven_'+sample+'__qcdisodown','True']
                    subprocess.call(cmd)
        else:
            for sample in samples:
                for item in os.listdir(self.ntuple_path+'/'+sample):
                    cmd = ['sbatch',script_file,str(self.year),self.ntuple_path+'/'+sample,item,sample,'False']
                    subprocess.call(cmd)
           
            for sample in samples:
                if "QCD" in sample or "Data" in sample: continue
                jetsyst = ['jerup','jerdown','jecup','jecdown']
                for syst in jetsyst:
                    for item in os.listdir(self.ntuple_path+'/'+sample):
                        cmd = ['sbatch',script_file,str(self.year),self.ntuple_path+'/'+sample,item,sample+'__'+syst,'False']
                        subprocess.call(cmd)
 
            for syst in extsyst:
                tmp = syst.split('_')
                if 'Bkg' in syst:
                    outName = tmp[0]+'_'+tmp[1]+'__'+tmp[-1].lower()
                else:
                    outName = tmp[0]+'_'+tmp[1]+'_'+tmp[-1]+'__'+tmp[-2].lower()
                if 'tunecuetp8m4' in outName:
                    outName = outName.replace('tunecuetp8m4','tune')
                if 'tunecp5' in outName:
                    outName = outName.replace('tunecp5', 'tune')
                for item in os.listdir(self.ntuple_path+'/'+syst):
                    cmd = ['sbatch',script_file,str(self.year),self.ntuple_path+'/'+syst,item,outName,'False']
                    subprocess.call(cmd)
