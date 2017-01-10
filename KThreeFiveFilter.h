//
//  KThreeFiveFilter.h
//  MiniSynth
//
//  Created by Alec McCormick on 1/9/17.
//
//

#pragma once
#include "Filter.h"
#include "VAOnePoleFilter.h"

class CKThreeFiveFilter : public CFilter
{
protected:
    // =============================================================
    // --- Variables
    // =============================================================
    
public:
    CKThreeFiveFilter(void);
    ~CKThreeFiveFilter(void);
    
    // =============================================================
    // --- Variables
    // =============================================================
    double m_dK;
    double m_dAlpha0;
    
    CVAOnePoleFilter m_LPF1;
    CVAOnePoleFilter m_LPF2;
    CVAOnePoleFilter m_HPF1;
    CVAOnePoleFilter m_HPF2;
    
    
    // =============================================================
    // --- CFilter Overrides
    // =============================================================
    virtual void reset();
    
    // recalc the coeff
    virtual void update();
    
    // do the filter
    virtual double doFilter(double xn);
    
    // --- decode the Q value; this can change from filter to filter
    virtual void setQControl(double dQControl);

};
