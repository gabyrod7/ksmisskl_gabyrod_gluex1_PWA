void diagram_kskl_event  () {
    TCanvas *c1 = new TCanvas("c1", "A canvas", 10,10, 600, 600);
    c1->Range(0, 0, 100, 100);
    Int_t linsav = gStyle->GetLineWidth();
    gStyle->SetLineWidth(3);

    TLatex t;
    t.SetTextAlign(22);
    t.SetTextSize(0.1);

    TLine *line;
    TCurlyLine *Gamma;
    TEllipse *ellipse;
    TArrow *arrow;

    Gamma = new TCurlyLine(8, 50, 40, 50);
    Gamma->SetWavy();
    Gamma->Draw();
    t.DrawLatex(4,50,"#gamma");

    arrow = new TArrow();
    arrow->SetLineStyle(2);
    arrow->SetFillColor(kBlack);

    line = new TArrow();
    line->SetLineStyle(2);

    arrow->DrawArrow(40, 50, 88, 90, 0.03, "|>");
    t.DrawLatex(94, 90, "K_{L}");
    line->DrawLine(40, 50, 70, 50);
    t.DrawLatex(63, 57, "K_{S}");

    arrow->SetLineStyle(1);
    arrow->DrawArrow(70, 50, 88, 60, 0.03, "|>");
    t.DrawLatex(94, 60, "#pi^{+}");

    arrow->DrawArrow(70, 50, 88, 40, 0.03, "|>");
    t.DrawLatex(94, 40, "#pi^{-}");

    arrow->DrawArrow(40, 50, 88, 10, 0.03, "|>");
    t.DrawLatex(94, 10, "p'");

    ellipse = new TEllipse(40, 50, 2);
    ellipse->SetFillColor(kRed);
    ellipse->SetLineColor(kRed);
    ellipse->Draw();
    t.DrawLatex(40, 58, "p");

    ellipse = new TEllipse(70, 50, 2);
    ellipse->SetFillColor(kRed);
    ellipse->SetLineColor(kRed);
    ellipse->Draw();

    c1->SaveAs("figs/kskl_event.pdf");
}
