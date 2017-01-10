//
//  CSEMFilter.h
//  MiniSynth
//
//  Created by Alec McCormick on 1/8/17.
//
//

#pragma once

#include "Filter.h"

class CSEMFilter : public CFilter
{
protected:
    // =============================================================
    // --- Variables
    // =============================================================
    double m_dZ11;
    double m_dZ12;

public:
    CSEMFilter(void);
    ~CSEMFilter(void);

    // =============================================================
    // --- Variables
    // =============================================================
    double m_dAlpha;
    double m_dAlpha0;
    double m_dRho;
    
    // =============================================================
    // --- CFilter Overrides
    // =============================================================
    virtual void reset()
    {
        m_dZ11 = 0;
        m_dZ12 = 0.0;
    }
    
    // recalc the coeff
    virtual void update();
    
    // do the filter
    virtual double doFilter(double xn);
    
    // --- decode the Q value; this can change from filter to filter
    virtual void setQControl(double dQControl);
    
};

