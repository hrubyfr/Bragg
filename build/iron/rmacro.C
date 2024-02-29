void rmacro(){
    std::unique_ptr<TFile> myFile (TFile::Open("Bragg_out.root", "RECREATE"));
    const int nf = 1;
    TString fname[nf] = {"Bragg.root"};
    TFile* readfile[nf];
    TH1D* BraggHist[nf];
    TH1D* histcopy[nf];
    gStyle->SetOptStat(0);
    TString dopt = "";


    TCanvas* can1 = new TCanvas ("Histogram", "Histogram", 1200, 800);
    can1->cd();

    for (int i = 0; i < nf; i++){
    readfile[i] = new TFile (fname[i]);
    if (!readfile[i] || readfile[i]->IsZombie()){
            std::cerr<<"error opening file"<<endl; //error control
            return;
        }
    BraggHist[i] = (TH1D*) readfile[i] -> Get("Eabs");
    BraggHist[i]->SetTitle("Braggova krivka zelezitych iontu o energii 1 GeV ve vode");
    BraggHist[i]->GetXaxis()->SetTitle("hloubka [mm]");
    BraggHist[i]->GetYaxis()->SetTitle("#frac{dE}{dx} #Delta x [MeV]");
    BraggHist[i]->GetXaxis()->SetRangeUser(0, 700);


    histcopy[i] = (TH1D*) BraggHist[i]->Clone();
    histcopy[i]->Scale(0.0001);
    histcopy[i]->Draw("hist"+dopt);
    dopt="same";
    }
    can1->SaveAs("BraggHist.pdf");

/*
    TF1** BraggFce = new TF1*[nf];

    TString fun_name = "Bragg fitovací fce";
    TString fun_predpis = "([0]*(exp([2]*(x-[1]))*exp(([2]**2*[3]**2)/2)))*erfc((x-[1])/([3]*sqrt(2))+[2]*[3]/sqrt(2))";
    double fun_minrange[nf], fun_maxrange[nf];
    double range[nf];


        fun_minrange[0] = (BraggHist[0]->GetMaximumBin())-6;
        fun_maxrange[0] = (BraggHist[0]->GetMaximumBin())+10;
        BraggFce[0] = new TF1 (fun_name, fun_predpis, fun_minrange[0], fun_maxrange[0]);
        BraggFce[0]->SetParameters(1.9, 159.5, 0.09, 1.5);
        histcopy[0]->Fit(BraggFce[0], "R");

        fun_minrange[1] = (BraggHist[1]->GetMaximumBin())-6;
        fun_maxrange[1] = (BraggHist[1]->GetMaximumBin())+10;
        BraggFce[1] = new TF1 (fun_name, fun_predpis, fun_minrange[1], fun_maxrange[1]);
        BraggFce[1]->SetParameters(1.7, 198, 0.09, 1.5);
        histcopy[1]->Fit(BraggFce[1], "R");

        fun_minrange[2] = (BraggHist[2]->GetMaximumBin())-6;
        fun_maxrange[2] = (BraggHist[2]->GetMaximumBin())+10;
        BraggFce[2] = new TF1 (fun_name, fun_predpis, fun_minrange[2], fun_maxrange[2]);
        BraggFce[2]->SetParameters(1.5, 238, 0.09, 1.5);
        histcopy[2]->Fit(BraggFce[2], "R");

        fun_minrange[3] = (BraggHist[3]->GetMaximumBin())-6;
        fun_maxrange[3] = (BraggHist[3]->GetMaximumBin())+13;
        BraggFce[3] = new TF1 (fun_name, fun_predpis, fun_minrange[3], fun_maxrange[3]);
        BraggFce[3]->SetParameters(1.35, 283, 0.09, 2);
        histcopy[3]->Fit(BraggFce[3], "R");

        fun_minrange[4] = (BraggHist[4]->GetMaximumBin())-6;
        fun_maxrange[4] = (BraggHist[4]->GetMaximumBin())+15;
        BraggFce[4] = new TF1 (fun_name, fun_predpis, fun_minrange[4], fun_maxrange[4]);
        BraggFce[4]->SetParameters(1.25, 330, 0.09, 2);
        histcopy[4]->Fit(BraggFce[4], "R");

        fun_minrange[5] = (BraggHist[5]->GetMaximumBin())-4;
        fun_maxrange[5] = (BraggHist[5]->GetMaximumBin())+20;
        BraggFce[5] = new TF1 (fun_name, fun_predpis, fun_minrange[5], fun_maxrange[5]);
        BraggFce[5]->SetParameters(1.15, 380, 0.09, 2);
        histcopy[5]->Fit(BraggFce[5], "R");

        fun_minrange[6] = (BraggHist[6]->GetMaximumBin())-7;
        fun_maxrange[6] = (BraggHist[6]->GetMaximumBin())+20;
        BraggFce[6] = new TF1 (fun_name, fun_predpis, fun_minrange[6], fun_maxrange[6]);
        BraggFce[6]->SetParameters(1.05, 432, 0.09, 2.5);
        histcopy[6]->Fit(BraggFce[6], "R");

        fun_minrange[7] = (BraggHist[7]->GetMaximumBin())-7;
        fun_maxrange[7] = (BraggHist[7]->GetMaximumBin())+20;
        BraggFce[7] = new TF1 (fun_name, fun_predpis, fun_minrange[7], fun_maxrange[7]);
        BraggFce[7]->SetParameters(0.95, 487, 0.09, 2.5);
        histcopy[7]->Fit(BraggFce[7], "R");

        fun_minrange[8] = (BraggHist[8]->GetMaximumBin())-7;
        fun_maxrange[8] = (BraggHist[8]->GetMaximumBin())+25;
        BraggFce[8] = new TF1 (fun_name, fun_predpis, fun_minrange[8], fun_maxrange[8]);
        BraggFce[8]->SetParameters(0.85, 543, 0.09, 2);
        histcopy[8]->Fit(BraggFce[8], "R");

        fun_minrange[9] = (BraggHist[9]->GetMaximumBin())-7;
        fun_maxrange[9] = (BraggHist[9]->GetMaximumBin())+25;
        BraggFce[9] = new TF1 (fun_name, fun_predpis, fun_minrange[9], fun_maxrange[9]);
        BraggFce[9]->SetParameters(0.8, 605, 0.09, 2);
        histcopy[9]->Fit(BraggFce[9], "R");

        fun_minrange[10] = (BraggHist[10]->GetMaximumBin())-7;
        fun_maxrange[10] = (BraggHist[10]->GetMaximumBin())+26;
        BraggFce[10] = new TF1 (fun_name, fun_predpis, fun_minrange[10], fun_maxrange[10]);
        BraggFce[10]->SetParameters(0.75, 663, 0.09, 2);
        histcopy[10]->Fit(BraggFce[10], "R");

    for (int i = 0; i < nf; i++){
        BraggFce[i]->Draw("same");


        range[i] = BraggFce[i]->GetX(0.8*BraggFce[i]->GetMaximum(), BraggHist[i]->GetMaximumBin(), (BraggHist[i]->GetMaximumBin())+26);
        std::cout << "Proton range is " << range[i] << "\n";
    }

    auto legend1 = new TLegend(0.6, 0.7, 0.9, 0.9);
    legend1->AddEntry (BraggHist[0], "Histogramy s daty", "f");
    legend1->AddEntry (BraggFce[0], "Fitovaci funkce", "l");
    legend1->Draw("same");

    can1->SaveAs("BraggFit.pdf");

    TCanvas* can2 = new TCanvas("range graph", "range graph", 1200, 800);
    can2->cd();

    double protonenergy[nf];
    protonenergy[0] = 150;
    for (int i = 1; i < nf; i++){
        protonenergy[i] = protonenergy [i-1] + 20;
    }

    auto rangeGraph = new TGraph(nf, protonenergy, range);
    rangeGraph->SetTitle("zavislost dosahu protonu ve vode na energii protonu");
    rangeGraph->GetXaxis()->SetTitle("energie [MeV]");
    rangeGraph->GetYaxis()->SetTitle ("dosah [mm]");

    rangeGraph->Draw("a*");

    TString rangeFitname = "Range fit function";
    TString rangeFitfun = "[0]*x**[1]";
    auto rangeFit = new TF1(rangeFitname, rangeFitfun, 0, 1000);
    rangeFit->SetParameters(0.022, 1.77);
    rangeGraph->Fit(rangeFit);
    rangeFit->SetLineWidth(1);
    rangeFit->SetLineColor(kRed);
    rangeFit->Draw("same");

    auto TheorFit = new TF1("theoretical empiric function", "[0]*x**[1]", 0, 1000);
    TheorFit->SetParameters(0.022, 1.77);
    TheorFit->SetLineWidth(1);
    TheorFit->SetLineColor(kBlue);
    TheorFit->Draw("same");

    auto rangeLegend = new TLegend(.1, .7, .45, .9);
    rangeLegend->AddEntry(rangeGraph, "Data", "p");
    rangeLegend->AddEntry(rangeFit, "fitovana krivka", "l");
    rangeLegend->AddEntry(TheorFit, "teoreticka krivka", "l");
    rangeLegend->AddEntry((TObject*)0, TString::Format("#chi^{2}/ndf = %0.1f", (rangeFit->GetChisquare()/rangeFit->GetNDF())), "");
    rangeLegend->Draw("same");

    can2->SaveAs("rangegraph.pdf");

    auto DivCanvas1 = new TCanvas ("divcan", "divcan", 1200, 900);
    DivCanvas1->Divide(2, 3, 0.001, 0.001);
    double peakX[nf];


    for (int i = 0; i <= 5; i++){
        DivCanvas1->cd(i+1);
        peakX[i] = histcopy[i]->GetMaximumBin();
        histcopy[i]->GetXaxis()->SetRangeUser(peakX[i]-20, peakX[i]+30);
        histcopy[i]->SetTitle(TString::Format("%c", i+97));
        histcopy[i]->Draw("hist");
        BraggFce[i]->Draw("same");

        auto divlegend = new TLegend (0.6, 0.6, 0.9, 0.9);
        divlegend->AddEntry (histcopy[i], "Data", "f");
        divlegend->AddEntry (BraggFce[i], "fitovana funkce", "l");
        divlegend->AddEntry((TObject*)0, TString::Format("#chi^{2}/ndf = %0.1f", (BraggFce[i]->GetChisquare()/BraggFce[i]->GetNDF())), "");
        divlegend->Draw("same");
    }
    DivCanvas1->SaveAs("DividedHists.pdf");

    auto DivCanvas2 = new TCanvas("divcan2", "divcan2", 1200, 600);
    DivCanvas2->Divide(2, 2, 0.001, 0.001);

    for (int i = 6; i < (nf-1); i++){
        DivCanvas2->cd(i-5);
        peakX[i] = histcopy[i]->GetMaximumBin();
        histcopy[i] -> GetXaxis()->SetRangeUser(peakX[i]-20, peakX[i]+30);
        histcopy[i]->SetTitle(TString::Format("%c", i+97));
        histcopy[i]->Draw("hist");
        BraggFce[i]->Draw("same");

        auto divlegend = new TLegend (0.6, 0.6, 0.9, 0.9);
        divlegend->AddEntry (histcopy[i], "Data", "f");
        divlegend->AddEntry (BraggFce[i], "fitovana funkce", "l");
        divlegend->AddEntry((TObject*)0, TString::Format("#chi^{2}/ndf = %0.1f", (BraggFce[i]->GetChisquare()/BraggFce[i]->GetNDF())), "");
        divlegend->Draw("same");
    }
    DivCanvas2->SaveAs("DividedHists2.pdf");

    auto lastcanvas = new TCanvas("lastc", "lastc", 600, 300);
    lastcanvas->cd();
    peakX[nf-1] = histcopy [nf-1] ->GetMaximumBin();
    histcopy[nf-1] -> GetXaxis()->SetRangeUser(peakX[nf-1]-20, peakX[nf-1]+30);
    histcopy[nf-1] -> SetTitle(TString::Format("%c", (nf-1)+97));
    histcopy[nf-1]->Draw("hist");
    BraggFce[nf-1]->Draw("same");

    auto divlegend = new TLegend (0.6, 0.6, 0.9, 0.9);
    divlegend->AddEntry (histcopy[nf-1], "Data", "f");
    divlegend->AddEntry (BraggFce[nf-1], "fitovana funkce", "l");
    divlegend->AddEntry((TObject*)0, TString::Format("#chi^{2}/ndf = %0.1f", (BraggFce[nf-1]->GetChisquare()/BraggFce[nf-1]->GetNDF())), "");
    divlegend->Draw("same");

    lastcanvas->SaveAs("DividedCanvas3.pdf");
*/
}
