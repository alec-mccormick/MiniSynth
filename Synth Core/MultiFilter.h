//
//  CMultiFilter.h
//  MiniSynth
//
//  Created by Alec McCormick on 1/8/17.
//
//

#pragma once
#include "Filter.h"
#include "VAOnePoleFilter.h"
#include "MoogLadderFilter.h"
#include "SEMFilter.h"

class CMultiFilter : public CFilter
{
protected:
    // =============================================================
    // --- Variables
    // =============================================================
    CVAOnePoleFilter m_OnePoleFilter;
    CMoogLadderFilter m_MoogLadderFilter;
    CSEMFilter m_SEMFilter;
    
    enum Mode {
        MOOG_LADDER_LPF4,
        MOOG_LADDER_LPF2,
        MOOG_LADDER_BPF4,
        MOOG_LADDER_BPF2,
        MOOG_LADDER_HPF4,
        MOOG_LADDER_HPF2,
        VA_ONE_POLE_LPF1,
        VA_ONE_POLE_HPF1,
        SEM_LPF2,
        SEM_BPF2,
        SEM_HPF2,
        SEM_BSF2,
        THREE_FIVE_LPF2,
        THREE_FIVE_HPF2,
        DIODE_LADDER_LPF4
    };
    
    UINT m_uFilterMode;
public:
    // =============================================================
    // --- Methods
    // =============================================================
    CMultiFilter(void);
    ~CMultiFilter(void);
    
    virtual void updateFilterMode();
    virtual void setFilterType(UINT filterType);
    virtual void initModulationMatrixLinks();
    
    // =============================================================
    // --- CFilter Overrides
    // =============================================================
    virtual void reset();
    virtual void setQControl(double dQControl);
    virtual double doFilter(double xn);
    virtual void setSampleRate(double d);
    virtual void update();
    
    virtual void initGlobalParameters(globalFilterParams* pGlobalFilterParams);
};
