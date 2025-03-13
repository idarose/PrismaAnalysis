#include <iostream>
#include <fstream>
#include <string>

using namespace std;


		
//---------------------------------------------------------------------------------|
//Internal functions (NOT FOR THE USER)
//TCutG* GetCut(string cutFileName, string cutName, string var1, string var2);
void GetThr(string thrFileName, vector<double>& lower, vector<double>& upper);
void GetCal(string calFileName, vector<double>& As, vector<double>& Bs);
TFile* GetFile(string inFileName);
TGraph* GetCut(string cutName);
void GetCalandThr_1id(string calFileName, map<int,pair<double,double>>& thresholds, map<int,vector<double>>& calibrations);
void GetCalandThr_2ids(string calFileName, map<pair<int,int>,pair<double,double>>& thresholds, map<pair<int,int>,vector<double>>& calibrations);
void DrawThrLines(double xmin, double xmax, double y = 1000,int color = kRed);

void DrawPPAC_Signal(string inFileName, string thrFileName, string variable, int nrEvts = -1);
void DrawPPAC_raw(string inFileName, string thrFileName);
void DrawPPAC_ana(string inFileName);
void DrawToFs (string inFileName, string thrFileName);
void DrawIonchPads (string inFileName, string thrFileName);
void DrawSidePads (string inFileName, string thrFileName);
void DrawZed(string inFileName, string gatePattern, int dee_type, int Zmin, int Zmax);
void DrawCharge(string inFileName, string gatePattern, int charge_type, int Z, int qmin, int qmax);



//---------------------------------------------------------------------------------|
void DrawPPAC_raw(string inFileName, string thrFileName){
	cout << "DrawPPAC_Right was called" << endl;
	TFile* fIn = GetFile(inFileName);
	
	map<pair<int,int>,pair<double,double>> thr;
	map<pair<int,int>,vector<double>> cal;

	GetCalandThr_2ids(thrFileName,thr,cal);
	
	TCanvas *c_R = new TCanvas("xRight_raw", "PPAC XRight raw per section", 1600, 600);
	c_R->Divide(5,2);
	TCanvas *c_L = new TCanvas("xLeft_raw", "PPAC XLeft raw per section", 1600, 600);
	c_L->Divide(5,2);
	TCanvas *c_C = new TCanvas("Cathode_raw", "PPAC Cathode raw per section", 1600, 600);
	c_C->Divide(5,2);
	TCanvas *c_RL = new TCanvas("xRight-xLeft_raw", "PPAC XRight-XLeft raw per section", 1600, 600);
	c_RL->Divide(5,2);
	TCanvas *c_RC = new TCanvas("xRight-Cathode_raw", "PPAC XRight-Cathode raw per section", 1600, 600);
	c_RC->Divide(5,2);
	TCanvas *c_CL = new TCanvas("Cathode-xLeft_raw", "PPAC Cathode-XLeft raw per section", 1600, 600);
	c_CL->Divide(5,2);

	vector<TH1D*> h_XR(10), h_XL(10), h_C(10), h_XRmXL(10), h_XRmC(10), h_CmXL(10);
	
	for(int i = 0; i < 10; ++i){
		c_R->cd(i+1);h_XR[i] = (TH1D*) gDirectory->Get(Form("Prisma/Raw/PPAC/h_XR_%i",i));h_XR[i]->Draw();
		DrawThrLines(thr[make_pair(0,i)].first,thr[make_pair(0,i)].second,h_XR[i]->GetMaximum()*1);
		c_L->cd(i+1);h_XL[i] = (TH1D*) gDirectory->Get(Form("Prisma/Raw/PPAC/h_XL_%i",i));h_XL[i]->Draw();
		DrawThrLines(thr[make_pair(1,i)].first,thr[make_pair(1,i)].second,h_XL[i]->GetMaximum()*1);
		c_C->cd(i+1); h_C[i] = (TH1D*) gDirectory->Get(Form("Prisma/Raw/PPAC/h_Cathode_%i",i));h_C[i]->Draw();
		DrawThrLines(thr[make_pair(2,i)].first,thr[make_pair(2,i)].second,h_C[i]->GetMaximum()*1); gPad->SetLogy();
		c_RL->cd(i+1);h_XRmXL[i] = (TH1D*) gDirectory->Get(Form("Prisma/Raw/PPAC/h_XRmXL_%i",i));h_XRmXL[i]->Draw();
		//DrawThrLines(i*100, (i+1)*100,h_XRmXL[i]->GetMaximum()*1,kCyan);
		c_RC->cd(i+1);h_XRmC[i] = (TH1D*) gDirectory->Get(Form("Prisma/Raw/PPAC/h_XRmCath_%i",i));h_XRmC[i]->Draw();
		//DrawThrLines(i*100, (i+1)*100,h_XRmC[i]->GetMaximum()*1,kCyan);
		c_CL->cd(i+1);h_CmXL[i] = (TH1D*) gDirectory->Get(Form("Prisma/Raw/PPAC/h_CathmXL_%i",i));h_CmXL[i]->Draw();
		//DrawThrLines(i*100, (i+1)*100,h_CmXL[i]->GetMaximum()*1,kCyan);
	}
}

void DrawPPAC_ana(string inFileName){
	cout << "DrawPPAC_Right was called" << endl;
	TFile* fIn = GetFile(inFileName);
	

	TCanvas *c_XFP = new TCanvas("XFP_sections", "Calibrated X_FP per section", 1600, 600);
	c_XFP->Divide(5,2);

	vector<TH1D*> h_Xfp(10), h_Xfp_RL(10), h_Xfp_RC(10), h_Xfp_CL(10);
	TH1D* h_Xfp_tot;
	
	

	for(int i = 0; i < 10; ++i){
		TLegend * leg = new TLegend(0.1,0.7,0.3,0.9);	
		c_XFP->cd(i+1);
		h_Xfp[i] = (TH1D*) gDirectory->Get(Form("Prisma/Ana/X_FP/h_XFP_section_%i",i));h_Xfp[i]->Draw();h_Xfp[i]->SetLineColor(kBlack); h_Xfp[i]->SetLineWidth(2);
		h_Xfp_RL[i] = (TH1D*) gDirectory->Get(Form("Prisma/Ana/X_FP/h_XFP_RL_section_%i",i));h_Xfp_RL[i]->Draw("same");h_Xfp_RL[i]->SetLineColor(kRed); h_Xfp_RL[i]->SetLineWidth(1);
		h_Xfp_RC[i] = (TH1D*) gDirectory->Get(Form("Prisma/Ana/X_FP/h_XFP_RC_section_%i",i));h_Xfp_RC[i]->Draw("same");h_Xfp_RC[i]->SetLineColor(kGreen); h_Xfp_RC[i]->SetLineWidth(1);
		h_Xfp_CL[i] = (TH1D*) gDirectory->Get(Form("Prisma/Ana/X_FP/h_XFP_CL_section_%i",i));h_Xfp_CL[i]->Draw("same");h_Xfp_CL[i]->SetLineColor(kViolet); h_Xfp_CL[i]->SetLineWidth(1);
		DrawThrLines(i*0.1, (i+1)*0.1,h_Xfp[i]->GetMaximum()*1,kCyan);
		leg->AddEntry(h_Xfp[i],"TOT","l");
		leg->AddEntry(h_Xfp_RL[i],"R-L","l");
		leg->AddEntry(h_Xfp_RC[i],"R-C","l");
		leg->AddEntry(h_Xfp_CL[i],"C-L","l");
		leg->Draw("same");
	}

	TCanvas * c_XFP_tot = new TCanvas("XFP_tot", "Calibrated X_FP total", 1600, 600);
	h_Xfp_tot = (TH1D*) gDirectory->Get("Prisma/Ana/h_XFP");
	h_Xfp_tot->Draw();
}


void DrawToFs (string inFileName, string thrFileName){
	cout << "DrawToFs was called" << endl;
	TFile* fIn = GetFile(inFileName);
	
	map<int,pair<double,double>> thr;
	map<int,vector<double>> cal;
	
	GetCalandThr_1id(thrFileName,thr,cal);

	TCanvas *c1 = new TCanvas("tof", "TOF", 1600, 600);
	c1->Divide(5,2);
	
	vector<TH1D*> h_TOF(10);
	
	for(int i = 0; i < 10; ++i){
		c1->cd(i+1);
		h_TOF[i] = (TH1D*) gDirectory->Get(Form("Prisma/Raw/PPAC/h_TOF_raw_%i",i));h_TOF[i]->Draw();
		DrawThrLines(thr[i].first,thr[i].second,h_TOF[i]->GetMaximum()*1);
		gPad->SetLogy();
	}
}

void DrawIonchPads (string inFileName, string thrFileName){
	cout << "DrawIonchPads was called" << endl;
	TFile* fIn = GetFile(inFileName);

	map<pair<int,int>,pair<double,double>> thr;
	map<pair<int,int>,vector<double>> cal;

	GetCalandThr_2ids(thrFileName,thr,cal);

	TCanvas *cA = new TCanvas("ionch_A", "IonCh_A", 1600, 600);
	cA->Divide(5,2);
	TCanvas *cB = new TCanvas("ionch_B", "IonCh_B", 1600, 600);
	cB->Divide(5,2);
	TCanvas *cC = new TCanvas("ionch_C", "IonCh_C", 1600, 600);
	cC->Divide(5,2);
	TCanvas *cD = new TCanvas("ionch_D", "IonCh_D", 1600, 600);
	cD->Divide(5,2);

	
	vector<TH1D*> h_ionch(10);
	
	vector<string> rows = {"A","B","C","D"};

	TPad * current = nullptr;
	for(int r = 0; r < 4; ++r){
		if(r==0)current = cA;
		else if(r==1)current = cB;
		else if(r==2)current = cC;
		else if(r==3)current = cD;
		for(int i = 0; i < 10; ++i){

			current->cd(i+1);
			h_ionch[i] = (TH1D*) gDirectory->Get(Form("Prisma/Raw/IC/h_IC_%s_%i",rows[r].c_str(),i));h_ionch[i]->Draw();
			DrawThrLines(thr[make_pair(r,i)].first,thr[make_pair(r,i)].second,h_ionch[i]->GetMaximum()*1);
			gPad->SetLogy();
		}
	}
}

void DrawSidePads (string inFileName, string thrFileName){
	cout << "DrawSidePads was called" << endl;
	TFile* fIn = GetFile(inFileName);

	map<pair<int,int>,pair<double,double>> thr;
	map<pair<int,int>,vector<double>> cal;

	GetCalandThr_2ids(thrFileName,thr,cal);

	TCanvas *c_sides = new TCanvas("sides", "sides", 800, 1600);
	c_sides->Divide(2,4);

	vector<TH1D*> h_side(8);
	
	vector<string> rows = {"A","B","C","D"};

	TPad * current = c_sides;
	for(int r = 0; r < 4; ++r){
		for(int i = 0; i < 2; ++i){

			current->cd(r*2+i+1);
			h_side[r*2+i] = (TH1D*) gDirectory->Get(Form("Prisma/Raw/IC/h_Side_%s_%i",rows[r].c_str(),i));h_side[r*2+i]->Draw();
			DrawThrLines(thr[make_pair(r,i)].first,thr[make_pair(r,i)].second,h_side[i]->GetMaximum()*1);
			gPad->SetLogy();
		}
	}
}

void DrawZed(string inFileName, string gatePattern, int dee_type, int Zmin, int Zmax){
	cout << "DrawZed was called" << endl;

	TFile* fIn = GetFile(inFileName);
	
	TCanvas *c1 = new TCanvas("zed", "zed", 1000,1000);

	string histoName = "Prisma/Ana/DE_E/";
	if(dee_type==0) histoName += "m_ICDEAB_ICE_tot_0";
	else if (dee_type==1) histoName += "m_ICDEA_ICE_tot_1";
	else if (dee_type==2) histoName += "m_ICDEA_ICEres_tot_3";

	TH2D* zed_matrix = (TH2D*) fIn->Get(histoName.c_str());
	
	zed_matrix->Draw("col");

	for(int z = Zmin; z <= Zmax; ++z){
		string gateName = Form("%s__%i__%i.root",gatePattern.c_str(),dee_type,z);
		TGraph *cut = GetCut(gateName);
		if(cut== nullptr){
			cout << "No cut found for " << gateName << endl;
			continue;	
		}

		cut->Draw("LPsame");
	}
}

void DrawCharge(string inFileName, string gatePattern, int charge_type, int Z, int qmin, int qmax){
	cout << "DrawChargeERB was called" << endl;

	TFile* fIn = GetFile(inFileName);
	
	TCanvas *c1 = new TCanvas("charge", "charge", 1000,1000);

	string histoName = "Prisma/Ana/";
	if(charge_type==0) histoName += "ICE_RBeta/m_ICE_corr_RBeta_tot";
	else if (charge_type==1) histoName += "ICE_RBeta/m_ICERBeta_ICE_corr_tot";
	else if (charge_type==2) histoName += "ICE_XFP/m_ICE_XFP_tot";
	else if (charge_type==3) histoName += "Charge_AoQ/m_Charge_cal_AoQ_tot";

	TH2D* charge_matrix = (TH2D*) fIn->Get(histoName.c_str());
	
	charge_matrix->Draw("col");

	for(int q = qmin; q <= qmax; ++q){
		string gateName = Form("%s__%i_%i__%i.root",gatePattern.c_str(),charge_type,Z,q);
		TGraph *cut = GetCut(gateName);
		if(cut== nullptr){
			cout << "No cut found for " << gateName << endl;
			continue;	
		}

		cut->Draw("LPsame");
	}


}



//---------------------------------------------------------------------------------|
TCutG* GetCut(string cutFileName, string cutName, string varX, string varY){

	TCutG *cut = new TCutG(cutName.c_str());
	cut->SetVarX(varX.c_str());
	cut->SetVarY(varY.c_str());
	
	ifstream banFile(cutFileName);
	if(banFile.is_open()){

		string a, b, temp;
		vector <double> x_v, y_v;

		while(getline(banFile,temp)){
			if(temp[0]=='#') continue;			
			
			stringstream ss;
			ss << temp;
			ss >> a >> b;			
			
			x_v.push_back(stod(a));
			y_v.push_back(stod(b));	
		}			

		for (unsigned int j = 0; j < x_v.size(); ++j){
				cut->SetPoint(j,x_v[j],y_v[j]);
		}	
	} 
	else cout << "Could not open file " << cutFileName << endl;
 	banFile.close();
 	
	cout << "\nLoaded cut: " << endl;	
	cut->Print();
	cut->SetLineColor(kRed);
	cut->SetMarkerStyle(8);
	cut->SetLineWidth(2);
	return cut;
}

void GetCalandThr_2ids(string calFileName, map<pair<int,int>,pair<double,double>>& thresholds, map<pair<int,int>,vector<double>>& calibrations){

	ifstream calFile(calFileName);
	string line;
	while(std::getline(calFile, line)) {
        if(line[0] == '#' || line.empty()) continue;
        std::istringstream sstr(line);
        std::string token;
        std::array<std::vector<double>, 3> parameters;
        int parIndex = -1;
        while(sstr >> token) {
            if(token == "id"){
                parIndex = 0;
            } else if (token == "cal")
            {
                parIndex = 1;
            } else if (token == "thr"){
                parIndex = 2;
            } else if (parIndex != -1){
                parameters[parIndex].push_back(std::stod(token));
            }
        }
        if(parameters[0].size() != 2) {
            throw std::runtime_error("GetCalandThr: id size does not match N");
        }
        std::pair<int,int> tmpIdx;
        tmpIdx.first = static_cast<int>(parameters[0].at(0));
        tmpIdx.second = static_cast<int>(parameters[0].at(1));
    
        if(parameters[2].size() != 2 && parameters[2].size() != 0){
            throw std::runtime_error("GetCalandThr: thr size does not match 2");
        }
        std::pair<double, double> tmpThr;
        if(parameters[2].size() == 0){
            tmpThr = std::make_pair(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
        }else if (parameters[2].size() == 2){
            tmpThr = std::make_pair(parameters[2].at(0), parameters[2].at(1));
        } else {
            throw std::runtime_error("GetCalandThr: thr size does not match 2");
        }
		thresholds[tmpIdx] = tmpThr;
        calibrations[tmpIdx] = parameters[1];
    }
}

void GetCalandThr_1id(string calFileName, map<int,pair<double,double>>& thresholds, map<int,vector<double>>& calibrations){

	ifstream calFile(calFileName);
	string line;
	while(std::getline(calFile, line)) {
        if(line[0] == '#' || line.empty()) continue;
        std::istringstream sstr(line);
        std::string token;
        std::array<std::vector<double>, 3> parameters;
        int parIndex = -1;
        while(sstr >> token) {
            if(token == "id"){
                parIndex = 0;
            } else if (token == "cal")
            {
                parIndex = 1;
            } else if (token == "thr"){
                parIndex = 2;
            } else if (parIndex != -1){
                parameters[parIndex].push_back(std::stod(token));
            }
        }
        if(parameters[0].size() != 1) {
            throw std::runtime_error("GetCalandThr: id size does not match N");
        }
        int tmpIdx;
        for(int i = 0; i < 1; i++) {
            tmpIdx = static_cast<int>(parameters[0].at(i));
        }
        if(parameters[2].size() != 2 && parameters[2].size() != 0){
            throw std::runtime_error("GetCalandThr: thr size does not match 2");
        }
        std::pair<double, double> tmpThr;
        if(parameters[2].size() == 0){
            tmpThr = std::make_pair(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
        }else if (parameters[2].size() == 2){
            tmpThr = std::make_pair(parameters[2].at(0), parameters[2].at(1));
        } else {
            throw std::runtime_error("GetCalandThr: thr size does not match 2");
        }
		thresholds[tmpIdx] = tmpThr;
        calibrations[tmpIdx] = parameters[1];
    }
}

TGraph * GetCut(string cut_path){
	TFile *f = new TFile(cut_path.c_str());
	string cut_name = cut_path.substr(cut_path.find_last_of("/")+1);
	cut_name = cut_name.substr(0,cut_name.find("."));
	TGraph *cut = (TGraph*)f->Get(cut_name.c_str());
	if(cut == nullptr){
		cout << "Could not find cut " << cut_name << " in file " << cut_path << endl;
		return nullptr;
	}
	cut -> SetLineColor(kRed);
	cut -> SetLineWidth(2);
	cut -> SetMarkerStyle(8);
	return cut;
}

TFile * GetFile(string inFileName){
	TFile *fIn = new TFile(inFileName.c_str());
	if(!fIn->IsOpen()){
		cout << "Could not open file " << inFileName << endl;
		exit(1);
	}
	return fIn;
}

void DrawThrLines(double xmin, double xmax, double y = 1000,int color = kRed){
	TLine *Lower = new TLine (xmin, 0, xmin, y); Lower->SetLineColor(color); Lower->SetLineWidth(2);
	TLine *Upper = new TLine (xmax, 0, xmax, y); Upper->SetLineColor(color); Upper->SetLineWidth(2);
	Lower->Draw("same");
	Upper->Draw("same");
}


