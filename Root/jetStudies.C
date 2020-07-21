#define jetStudies_cxx

#include "fastjet/ClusterSequence.hh"

using namespace fastjet;

void makeJets(std::string sample_name, Long64_t ievent, std::vector<Track> tracks, float R=0.8){

	vector<PseudoJet> particles;
	// an event with  particles: px py pz E
	for (auto track : tracks){
		particles.push_back( PseudoJet( track.p4.Px(), track.p4.Py(), track.p4.Pz(), track.p4.E()) );
	}
	
	// choose a jet definition
	int cone = R*10;
	JetDefinition jet_def(antikt_algorithm, R);

	// run the clustering, extract the jets
	ClusterSequence cs(particles, jet_def);
	vector<PseudoJet> jets = sorted_by_pt(cs.inclusive_jets());

	// print out some info about reco method
	//std::cout << "Clustered with " << jet_def.description() << std::endl;

	// print the jets
	int njets=0;
	for (unsigned i = 0; i < jets.size(); i++) {

		// basic cleaning cuts 
		if ( jets[i].pt() < 30 ) continue; // need some min jet cut
		// some min # tracks cut ? > 1 

		njets+=1;

		// add plots of other variables here

		plotter.Plot1D(Form( "%s_jetsAK%i_pt" , sample_name.c_str(),cone),";jet pt" , jets[i].pt()      , 100, 0, 1000 );
		plotter.Plot1D(Form( "%s_jetsAK%i_eta", sample_name.c_str(),cone),";jet eta", jets[i].eta()     , 100, -3.5, 3.5 );
		plotter.Plot1D(Form( "%s_jetsAK%i_phi", sample_name.c_str(),cone),";jet phi", jets[i].phi_std() , 100, -3.5, 3.5 );


		plotter.Plot2D(Form("%s_evt%lli_event_display_jetsAK%i",sample_name.c_str(),ievent,cone),";eta;phi;pt", jets[i].eta(), jets[i].phi_std(), 100, 3.5,3.5,100,3.5,3.5 , jets[i].perp());

		float dem=0;
		float num=0;
		float jet_width=0;
		float SUEP_jet=0;
		float SUEP_jet.constituents=0;
		TLorentzVector jet_p4;
		TLorentzVector constituent_p4;
		
		jet_p4.SetPtEtaPhiE(jets[i].pt(), jets[i].eta(), jets[i].phi_std(), jets[i].E());
		
		
		vector<PseudoJet> constituents = jets[i].constituents();
		for (unsigned j = 0; j < constituents.size(); j++) {
			
			if ( jet[i].constituents > SUEP_jet.constituents ); {
				SUEP_jet = jet[i]
			}
				
			constituent_p4.SetPtEtaPhiM(constituents[j].pt(), constituents[j].eta(), constituents[j].phi_std(), constituents[j].E());
			
			num += jet_p4.DeltaR(constituent_p4)*constituent_p4.Pt();
			dem += constituent_p4.Pt();
			
			plotter.Plot1D(Form( "%s_jetsAK%i_constit_pt", sample_name.c_str(),cone),";constit pt", constituents[j].pt(), 100, 0, 100 );

		}
		jet_width = num/dem;
		
		plotter.Plot1D(Form( "%s_jetsAK%i_nconstit", sample_name.c_str(),cone),";n constit.", constituents.size(), 100, 0, 500 );
		plotter.Plot1D(Form( "%s_jetsAK%i_jet_width", sample_name.c_str(),cone),";jet width.", jet_width, 100, 0, 3 );
		plotter.Plot1D(Form( "%s_jetsAK%i_suep_jet", sample_name.c_str(),cone),";SUEP Jet.", SUEP_jet, 100, 0, 500 );

	}
	plotter.Plot1D(Form( "%s_jetsAK%i_njets", sample_name.c_str(),cone),";njets", njets, 20, -0.5, 19.5 );



	



}
