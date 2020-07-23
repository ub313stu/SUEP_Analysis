import ROOT
from array import array
from plothelper import *
ROOT.gROOT.SetBatch(ROOT.kTRUE)

setStyle()

def adjust(hist):
    name = hist.GetName()
    #if "ntracks" in name: hist.Rebin()
    if "ntracks" in name: hist.GetXaxis().SetRangeUser(0,600)
    #if "npfs" in name: hist.Rebin()
    if "npfs" in name: 
        hist.GetXaxis().SetRangeUser(0,600)
    if "nneutrals" in name: 
        hist.GetXaxis().SetRangeUser(0,600)
        hist.GetXaxis().SetTitle("n neutrals")
    return
  
def clean1D(hist):
    # Clean
    adjust(hist)
    hist.SetLineWidth(2)
    hist.GetYaxis().SetNdivisions(505)
    hist.GetXaxis().SetNdivisions(505)
    hist.SetDirectory(0)
    #hist.Scale(1.0/hist.Integral(0,-1))
    return hist
  
def get1D(mMed,mDark,temp,decay,histname):

    # Get hist
    filename = "output/mMed-{}_mDark-{}_temp-{}_decay-{}.root".format(mMed,mDark,temp,decay)
    f = ROOT.TFile.Open(filename)
    hist = f.Get(histname)
    clean1D(hist)

    return hist

def decay_label(decay):
    if "darkPhoHad" in decay: return "m_{A'}=0.7 GeV"
    if "darkPho" in decay: return "m_{A'}=0.5 GeV"
    if "generic" in decay: return "m_{A'}=m_{#phi}/2, A'#rightarrowu#bar{u}"

def label(mMed,mDark,temp,decay):
    #return "(m_{S},m_{#phi},T)=(%i,%i,%i), %s"%(mMed,mDark,temp,decay_label(decay))
    return "m_{S}=%i, m_{#phi}=%i, T=%i, %s"%(mMed,mDark,temp,decay_label(decay))

def makeROC(hists,labels,filename):
    c = ROOT.TCanvas(filename,"",800,800)

    dy = 0.05*len(hists)
    leg = ROOT.TLegend(0.18,0.86-dy,0.86,0.86)
    leg.SetTextSize(0.04)
    leg.SetBorderSize(0)
    
def compare1D(hists,labels,filename):
    c = ROOT.TCanvas(filename,"",800,800)

    dy = 0.05*len(hists)
    leg = ROOT.TLegend(0.18,0.86-dy,0.86,0.86)
    leg.SetTextSize(0.035)
    leg.SetBorderSize(0)

    ymax = 0
    for i,hist in enumerate(hists): 
        hist.SetLineColor(colors[i])
        if "QCD" in labels[i]: hist.SetLineColor(ROOT.kBlack) 
        if i==0: hist.Draw("hist")
        else : hist.Draw("hist same")

        if hist.GetMaximum() > ymax: ymax=hist.GetMaximum()

        leg.AddEntry(hist,labels[i],"l")


    leg.Draw()
    
    c.SetLogy(1)
    hists[0].GetYaxis().SetRangeUser(0.001,ymax*100)
    c.Print("plots/{}_log.png".format(filename))
    hists[0].GetYaxis().SetRangeUser(0,ymax*1.8)
    c.SetLogy(0)
    c.Print("plots/{}_lin.png".format(filename))
    
def compareSUEPs(size,mMed,dist):

    temp = 2
    mDark = 2
    decay = "generic"
    hists = []
    labels = []
  
    histname = "mMed-{}_mDark-{}_temp-{}_decay-{}_{}_{}".format(mMed,mDark,temp,decay,size,dist)
    hists.append(get1D(mMed,mDark,temp,decay,histname))
    #labels.append(label(mMed,mDark,temp,decay))
    labels.append("all_jets")
  
    histname = "mMed-{}_mDark-{}_temp-{}_decay-{}_{}_suep_jet_{}".format(mMed,mDark,temp,decay,size,dist)
    hists.append(get1D(mMed,mDark,temp,decay,histname))
    #labels.append(label(mMed,mDark,temp,decay))
    labels.append("suep_jets")  
  
    compare1D(hists,labels,"compare_suep/mMed{}_temp{}_mDark{}_decay_{}_{}_{}".format(mMed,temp,mDark,decay,size,dist))


sizes = []
sizes.append("jetsAK8")
sizes.append("jetsAK15")
sizes.append("jetsAK20")

mMeds = []
mMeds.append(125)
mMeds.append(400)
mMeds.append(750)
mMeds.append(1000)
    
dists = []
dists.append("width")
dists.append("nconstit")
dists.append("pt")
dists.append("eta")
dists.append("phi")
dists.append("mass")

for size in sizes:
    for mMed in mMeds:
        for dist in dists:
            compareSUEPs(size,mMed,dist)
        
    
