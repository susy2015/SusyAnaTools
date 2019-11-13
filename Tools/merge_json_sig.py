import json

def myprocess(j_full,sample,year):
    j_in = "fastsim_results/{0}{1}/{0}{1}.json".format(sample,year)
    with open(j_in,"r") as inputfile:
        j_part = json.load(inputfile)
    #print j_part.keys()
    #j["signals"].append(j_part["signals"]) #This leads to a list of lists
    for signal in j_part["signals"]:
        j["signals"].append(signal)
    for k,v in j_part["yieldsMap"].iteritems():
        j["yieldsMap"].update({k:v})

if __name__ == "__main__":
    year = "_2016"
    #years = ["_2016","_2017","_2018"]
    #samples = ["SMS_T2tt_mStop_150to250_fastsim","SMS_T2tt_mStop_250to350_fastsim","SMS_T2tt_mStop_350to400_fastsim","SMS_T2tt_mStop_400to1200_fastsim","SMS_T2tt_mStop_1200to2000_fastsim"]
    #Only ran a few for testing
    samples = ["SMS_T2tt_mStop_150to250_fastsim","SMS_T2tt_mStop_250to350_fastsim","SMS_T2tt_mStop_350to400_fastsim"]
    j = {}
    j["yieldsMap"] = {}
    j["signals"] = []
    for sample in samples:
        myprocess(j,sample,year)
    outputfilename = "fastsim_results/SMS_T2tt_fastsim{0}.json".format(year) 
    print "Creating file fastsim_results/SMS_T2tt_fastsim{0}.json".format(year)
    with open(outputfilename,"w") as outputfile:
        json.dump(j,outputfile,sort_keys=True,indent=4,separators=(',', ': '))

