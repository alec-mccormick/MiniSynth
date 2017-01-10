//
//  CSEMFilter.cpp
//  MiniSynth
//
//  Created by Alec McCormick on 1/8/17.
//
//

#include "SEMFilter.h"


// =================================================================
// --- Constructor
// =================================================================
CSEMFilter::CSEMFilter(void)
{
    // --- Init variables
    m_dAlpha0 = 1.0;
    m_dAlpha = 1.0;
    m_dRho = 1.0;
    m_dAuxControl = 0.5;
    
    // --- Default filter type
    m_uFilterType = LPF2;
    
    // --- flush
    reset();
}

CSEMFilter::~CSEMFilter(void)
{
}

// =================================================================
// --- Methods
// =================================================================
void CSEMFilter::setQControl(double dQControl)
{
    // this maps dQConytol = 1 -> 10 to Q = 0.5 -> 25
    m_dQ = (25.0 - 0.5) * (dQControl - 1.0)/(10.0 - 1.0) + 0.5;
}

void CSEMFilter::update()
{
    // Base class does modulation
    CFilter::update();
    
    // pre-warp the cutoff - these are bilinear-transform filters
    double wd = 2 * pi * m_dFc;
    double T = 1/m_dSampleRate;
    double wa = (2/T) * tan(wd * T/2);
    double g = wa * T/2;
    
    // note R is the traditional analog damping factor
    double R = 1.0/(2.0 * m_dQ);
    
    // set the coeffs
    m_dAlpha0 = 1.0/(1.0 + 2.0*R*g + g*g);
    m_dAlpha = g;
    m_dRho = 2.0*R + g;
}

double CSEMFilter::doFilter(double xn)
{
    // return xn if filter not supported
    if(m_uFilterType != LPF2 && m_uFilterType != HPF2 && m_uFilterType != BPF2 && m_uFilterType != BSF2)
        return xn;
    
    // form the HP output first
    double hpf = m_dAlpha0*(xn - m_dRho*m_dZ11 - m_dZ12);
    
    // BPF Out
    double bpf = m_dAlpha*hpf + m_dZ11;
    
    // for nonlinear proc
    if(m_uNLP == ON)
        bpf = tanh(m_dSaturation*bpf);
    
    // LPF Out
    double lpf = m_dAlpha*bpf + m_dZ12;
    
    // note R is the traditional analog damping factor
    double R = 1.0/(2.0*m_dQ);
    
    // BSF Out
    double bsf = xn - 2.0*R*bpf;
    
    // SEM BPF Output
    // using m_dAuxControl for this one-off control
    double semBSF = m_dAuxControl*hpf + (1.0 - m_dAuxControl)*lpf;
    
    // update memory
    m_dZ11 = m_dAlpha*hpf + bpf;
    m_dZ12 = m_dAlpha*bpf + lpf;
    
    //return our selected type
    switch(m_uFilterType)
    {
        case LPF2:
            return lpf;
        case HPF2:
            return hpf;
        case BPF2:
            return bpf;
        case BSF2:
            return semBSF;
        default:
            return xn;
    }
}
