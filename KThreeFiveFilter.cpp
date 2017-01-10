//
//  KThreeFiveFilter.cpp
//  MiniSynth
//
//  Created by Alec McCormick on 1/9/17.
//
//

#include "KThreeFiveFilter.h"

// =================================================================
// --- Constructor
// =================================================================
CKThreeFiveFilter::CKThreeFiveFilter(void)
{
    m_dK = 0.01;
    m_dAlpha0 = 0;
    
    // --- set filters types
    m_LPF1.m_uFilterType = LPF1;
    m_LPF2.m_uFilterType = LPF1;
    m_HPF1.m_uFilterType = HPF1;
    m_HPF2.m_uFilterType = HPF1;
    
    // --- default fitler type
    m_uFilterType = LPF2;
    
    // --- flush everything
    reset();
}

CKThreeFiveFilter::~CKThreeFiveFilter(void)
{
}



// =================================================================
// --- CFilter Overrides
// =================================================================
void CKThreeFiveFilter::reset()
{
    m_LPF1.reset();
    m_LPF2.reset();
    m_HPF1.reset();
    m_HPF2.reset();
}

void CKThreeFiveFilter::update()
{
    CFilter::update();
    
    double wd = 2*pi*m_dFc;
    double T = 1/m_dSampleRate;
    double wa = (2/T)*tan(wd*T/2);
    double g = wa*T/2;

    // G - the feedforward coeff in the VA One Pole
    double G = g/(1.0 + g);
    
    // --- Set alphas
    m_LPF1.m_dAlpha = G;
    m_LPF2.m_dAlpha = G;
    m_HPF1.m_dAlpha = G;
    m_HPF2.m_dAlpha = G;
    
    m_dAlpha0 = 1.0/(1.0 - m_dK*G + m_dK*G*G);
    
    if(m_uFilterType == LPF2)
    {
        m_LPF2.m_dBeta = (m_dK - m_dK*G)/(1.0 + g);
        m_HPF1.m_dBeta = -1.0/(1.0 + g);
    }
    else // HPF
    {
        m_HPF2.m_dBeta = -1.0*G/(1.0 + g);
        m_LPF1.m_dBeta = 1.0/(1.0 + g);
    }
}

double CKThreeFiveFilter::doFilter(double xn)
{
    if(m_uFilterType != LPF2 && m_uFilterType != HPF2)
        return xn;
    
    double y = 0.0;
    if(m_uFilterType == LPF2)
    {
        double y1 = m_LPF1.doFilter(xn);
        
        // form S35
        double S35 = m_HPF1.getFeedbackOutput() + m_LPF2.getFeedbackOutput();
        
        // calculate u
        double u = m_dAlpha0*(y1 + S35);
        // NLP
        if(m_uNLP == ON)
            u = tanh(m_dSaturation*u);
        
        // feed it to LPF2, then add gain x
        y = m_dK*m_LPF2.doFilter(u);
        
        // feed y to HPF
        m_HPF1.doFilter(y);
    }
    else
    {
        // Process input through HPF1
        double y1 = m_HPF1.doFilter(xn);
        
        // form S35
        double S35 = m_HPF2.getFeedbackOutput() + m_LPF1.getFeedbackOutput();
        
        
        // calculate u
        double u = m_dAlpha0*y1 + S35;
        
        // form output
        y = m_dK*u;
        
        // NLP
        if(m_uNLP == ON)
            y = tanh(m_dSaturation*y);
        
        // process y through feedback BPF
        m_LPF1.doFilter(m_HPF2.doFilter(y));
    }
    
    //auto-normalize
    if(m_dK > 0)
        y *= 1/m_dK;
    
    return y;
}

void CKThreeFiveFilter::setQControl(double dQControl)
{
    m_dK = (2.0 - 0.01)*(dQControl - 1.0)/(10.0 - 1.0) + 0.01;
}
