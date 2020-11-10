import os
import sys
import subprocess
import argparse
from argparse import RawTextHelpFormatter
import textwrap


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Make final histograms", formatter_class=RawTextHelpFormatter)
    parser.add_argument('--configs', required=False, type=str, default='./configs/config16.yml', help='Set configuration')
    options = parser.parse_args()
    
    print textwrap.dedent("""\
        ...Select run option 
        1. sample: save ntuple information 
        2. tselector: run tselector for basic plots
        3. dnn: run dnn analyzer code
        4. qcd: run tselector for qcd estimation
        5. post: run post process
        6. draw: draw cut flow table and histograms\
    """)
    main_cmd = raw_input("...Choose option: sample/tselector/dnn/qcd/post/draw ")
    if not any(i in main_cmd for i in ['sample','tselector','dnn','qcd','post','draw']):
        print "...Wrong command"
        exit(0)

    import yaml
    with open(options.configs, 'r') as config_file:
        config = yaml.load(config_file)
    if not config:
        print "Error: Loading the config file %s" % options.configs
        sys.exit(1)

    if main_cmd == 'sample':
        print "...Save ntuple information"
        import python.saveAndLoadSamples as sls
        save = sls.SaveSamples(config['year'], config['ntuple'], './samples')
        save.store_sample_list()
        save.store_nevents()

    if main_cmd == 'tselector':
        print "...Fill basic histograms"
        # run tselector...
        import python.makeSlurmJob as sj
        job = sj.RunTSelector()
        job.make_slurm_job()

    if main_cmd == 'dnn':
        print textwrap.dedent("""\
            ====== WARNING: ACTIVATE CONDA ENVIRONMENT FIRST ======"
            cmd in htop: conda activate py27
            ...Select sub-option
            1. model: Make DNN model
            2. n2a: make array from ntuple
            3. pred: predict DNN score
            4. hist: make DNN output histograms\
        """)
        sub_cmd = raw_input("Choose subcommand: model/n2a/pred/hist ")
        if not any(i in sub_cmd for i in ['mode','n2a','pred','hist']):
            print "...Wrong command"
            exit(0)
        
        if sub_cmd == "model":
            print "...Make DNN model"
            import python.makeModel as mm
            train_input = ''
            config_file = ''
            model = mm.MakeModel(train_input, config_file)
            model.load_config()
            model.compile_train()
            model.save_config()

        if sub_cmd == "n2a":
            print "...Make array from ntuple for DNN"
            outputDir = '../output/array'+str(self.config['year'])
            for item in os.listdir(inputDir+'/TTLJ_PowhegPythia_ttbb'):
                cmd = ['sbatch','slurm_n2a.sh',inputDir+'/TTLJ_PowhegPythia_ttbb',item,outputDir+'/Training','True','']
                subprocess.call(cmd)
            for sample in os.listdir(inputDir):
                outputDir = '../output/array'+str(year)+'/'+sample
                if 'part' in sample: continue
                for item in os.listdir(inputDir+'/'+sample):
                    cmd = ['sbatch','slurm_n2a.sh',inputDir+'/'+sample,item,outputDir,'False']
                    subprocess.call(cmd)
                if not any(i in sample for i in ['QCD','Data','SYS']):
                    jetSyst = ['jerup','jerdown','jecup','jecdown']
                    for syst in jetSyst:
                        outputDir = '../output/array'+str(year)+'/'+sample+'__'+syst 
                        for item in os.listdir(inputDir+'/'+sample):
                            cmd = ['sbatch','slurm_n2a.sh',inputDir+'/'+sample,item,outputDir,'False',syst]
                            subprocess.call(cmd)

        if sub_cmd == "pred":
            print "...Predict DNN score"
            inputDir = '../output/array'+str(config['year'])
            outputDir = '../output/pred'+str(config['year'])
            if not os.path.exists(outputDir):
                os.makedirs(outputDir)
            import prediction as pred
            pred.prediction(year, inputDir, outputDir)
 
        if sub_cmd == "hist":
            print "...Make DNN output histogram root file"
            arrayDir  = '../output/pred'+str(config['year'])
            outputDir = '../output/dnn'+str(config['year'])
            if not os.path.exists(outputDir):
                os.makedirs(outputDir)
            procs = []
            for sample in os.listdir(arrayDir):
                #if not 'DataSingleMuB' in sample: continue
                if 'Training' in sample: continue
                if 'SYS' in sample:
                    tmp = sample.split('_')
                    if 'Bkg' in sample:
                        outName = tmp[0]+'_'+tmp[1]+'__'+tmp[-1].lower()
                    else:
                        outName = tmp[0]+'_'+tmp[1]+'_'+tmp[-1]+'__'+tmp[-2].lower()
                else:
                    outName = sample
                if 'tunecuetp8m4' in outName:
                    outName = outName.replace('tunecuetp8m4','tune')
                if 'tunecp5' in outName:
                    outName = outName.replace('tunecp5', 'tune')
                outputDir = '../output/dnn'+str(year)+'/'+outName
                if not os.path.exists(outputDir):
                    os.makedirs(outputDir)
                for item in os.listdir(os.path.join(arrayDir,sample)):
                    cmd = ['sbatch', 'slurm_a2h.sh', str(year), arrayDir+'/'+sample, item, outputDir, outName]
                    subprocess.call(cmd)
    
    if main_cmd == 'qcd':
        print "Melona"
        # run tselector
        # qcd post process
        # save...

    if main_cmd == 'post':
        print "Melona"
        # hadd...
        # post process 

    if main_cmd == 'draw':
        print textwrap.dedent("""\
            ...Select sub-option
            1. table: make cut flow table
            2. syst: draw systematic plot for each source
            3. plotIt: run plotter
            4. exit: Close program\
        """)
        
        input_path = config['output']+'/post16'
        while True:
            sub_cmd = raw_input("...Choose subcommand: table/syst/plotIt/exit ")
            if sub_cmd == 'exit': exit(0) 
            elif sub_cmd == 'table':
                # draw cut flot table
                import python.drawCutflowTable as cf
                table = cf.DrawYieldsTable(config['year'], input_path, config['output']+'/table')
                table.get_root_files()
                table.draw_yields()
            elif sub_cmd == 'syst':
                # draw systematic plots 
                output_path = config['output']+'/test'
                import python.drawSystPlots as sp
                hist = sp.DrawSystPlots(config['year'], input_path, config['hist'], output_path)
                hist.get_root_files()
                hist.draw_histograms()
            elif sub_cmd == 'plotIt':
                print textwrap.dedent("""\
                    ====== WARNING: ACTIVATE SL7 ENVIRONMENT FIRST ======"
                    cmd in htop: source plotIt/setup_sl7_env.sh
                    ...Select sub sub option
                    1. hist: make histogram config file
                    2. file: make input config file
                    3. run: run plotter
                    4. exit: Close program\
                """)
                while True:
                    sub_sub_cmd = raw_input("...Choose sub sub command: hist/file/run/exit ")
                    if sub_sub_cmd == "exit": exit(0)
                    if any(i in sub_sub_cmd for i in ['hist', 'file']):
                        import python.makePlotItConfigs as mpc
                        config_path = 'plotIt/configs'
                        make_config = mpc.MakeConfigs(config['year'], input_path, config_path)
                        if sub_sub_cmd == 'file':
                            make_config.make_file_configs()
                        else:
                            make_config.make_histogram_configs()
                    if sub_sub_cmd == "run":
                        print "Not developted"
                        #cmd = ['plotIt/plotIt', 'plotIt/configs/config'+str(config['year'])+'.yml', '-o output/pdf']
                        #subprocess.call(cmd)
