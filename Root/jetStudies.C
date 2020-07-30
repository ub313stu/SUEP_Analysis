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
	AreaDefinition area_def(passive_area);

	// run the clustering, extract the jets
	//ClusterSequence cs(particles, jet_def);
	ClusterSequenceArea(particles, jet_def, area_def);
	vector<PseudoJet> jets = sorted_by_pt(cs.inclusive_jets());

	// print out some info about reco method
	//std::cout << "Clustered with " << jet_def.description() << std::endl;

	// print the jets
	int njets=0;
	float maxconstituents=0;
	PseudoJet SUEP_jet;
	float SUEP_jet_width=0;
	float SUEP_jet_pt=0;
	float SUEP_jet_eta=0;
	float SUEP_jet_phi=0;
	float SUEP_jet_mass=0;
	float SUEP_jet_area=0;
	
	for (unsigned i = 0; i < jets.size(); i++) {
		

		// basic cleaning cuts 
		if ( jets[i].pt() < 30 ) continue; // need some min jet cut
		// some min # tracks cut ? > 1 

		njets+=1;

		
		
		// add plots of other variables here

		plotter.Plot1D(Form( "%s_jetsAK%i_pt" , sample_name.c_str(),cone),";jet pt" , jets[i].pt()      , 100, 0, 1000 );
		plotter.Plot1D(Form( "%s_jetsAK%i_eta", sample_name.c_str(),cone),";jet eta", jets[i].eta()     , 100, -3.5, 3.5 );
		plotter.Plot1D(Form( "%s_jetsAK%i_phi", sample_name.c_str(),cone),";jet phi", jets[i].phi_std() , 100, -3.5, 3.5 );
		plotter.Plot1D(Form( "%s_jetsAK%i_mass", sample_name.c_str(),cone),";jet mass", jets[i].m() , 100, 0, 1000 );


		plotter.Plot2D(Form("%s_evt%lli_event_display_jetsAK%i",sample_name.c_str(),ievent,cone),";eta;phi;pt", jets[i].eta(), jets[i].phi_std(), 100, 3.5,3.5,100,3.5,3.5 , jets[i].perp());

		float dem=0;
		float num=0;
		float width=0;
		float area=0;
		
		
		
		TLorentzVector jet_p4;
		TLorentzVector constituent_p4;
		
		jet_p4.SetPtEtaPhiE(jets[i].pt(), jets[i].eta(), jets[i].phi_std(), jets[i].E());
		
		
		vector<PseudoJet> constituents = jets[i].constituents();
		
		int nconstit = constituents.size(); 		
		
		
		for (unsigned j = 0; j < constituents.size(); j++) {
			
			
			
			constituent_p4.SetPtEtaPhiM(constituents[j].pt(), constituents[j].eta(), constituents[j].phi_std(), constituents[j].E());
			
			num += jet_p4.DeltaR(constituent_p4)*constituent_p4.Pt();
			dem += constituent_p4.Pt();
			
			plotter.Plot1D(Form( "%s_jetsAK%i_constit_pt", sample_name.c_str(),cone),";constit pt", constituents[j].pt(), 100, 0, 100 );

		}
		width = num/dem;
		
		area = jets[i].area();
		
		if ( nconstit > maxconstituents ) {
				maxconstituents = nconstit;
				SUEP_jet = jets[i];
				SUEP_jet_width = width;
				SUEP_jet_pt = jets[i].pt();
				SUEP_jet_eta = jets[i].eta();
				SUEP_jet_phi = jets[i].phi_std();
				SUEP_jet_mass = jets[i].m();
				SUEP_jet_area = area;

			
			}
		
		
		
		plotter.Plot1D(Form( "%s_jetsAK%i_nconstit", sample_name.c_str(),cone),";n constit.", constituents.size(), 100, 0, 500 );
		plotter.Plot1D(Form( "%s_jetsAK%i_width", sample_name.c_str(),cone),";jet width.", width, 100, 0, 2 );
		plotter.Plot1D(Form( "%s_jetsAK%i_area", sample_name.c_str(),cone),";jet area.", area, 100, 0, 2 );
		

	}
	plotter.Plot1D(Form( "%s_jetsAK%i_njets", sample_name.c_str(),cone),";njets", njets, 20, -0.5, 19.5 );
	plotter.Plot1D(Form( "%s_jetsAK%i_suep_jet_nconstit", sample_name.c_str(),cone),";SUEP Jet.", maxconstituents, 100, 0, 500 );
	plotter.Plot1D(Form( "%s_jetsAK%i_suep_jet_width", sample_name.c_str(),cone),";jet width.", SUEP_jet_width, 100, 0, 2 );
	plotter.Plot1D(Form( "%s_jetsAK%i_suep_jet_pt", sample_name.c_str(),cone),";jet pt.", SUEP_jet_pt, 100, 0, 1000 );
	plotter.Plot1D(Form( "%s_jetsAK%i_suep_jet_eta", sample_name.c_str(),cone),";eta.", SUEP_jet_eta, 100, -3.5, 3.5 );
	plotter.Plot1D(Form( "%s_jetsAK%i_suep_jet_phi", sample_name.c_str(),cone),";phi.", SUEP_jet_phi, 100, -3.5, 3.5 );
	plotter.Plot1D(Form( "%s_jetsAK%i_suep_jet_mass", sample_name.c_str(),cone),";jet mass.", SUEP_jet_mass, 100, 0, 1000 );
	plotter.Plot1D(Form( "%s_jetsAK%i_suep_jet_area", sample_name.c_str(),cone),";jet area.", SUEP_jet_area, 100, 0, 2 );



	



}
