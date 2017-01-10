//
//  CMultiFilter.cpp
//  MiniSynth
//
//  Created by Alec McCormick on 1/8/17.
//

#include "MultiFilter.h"

CMultiFilter::CMultiFilter(void)
{
//    m_uFilterCategory = Category::MOOG_LADDER;
    m_uFilterMode = Mode::MOOG_LADDER_LPF4;
    m_uFilterType = LPF4;
    
    // flush everything
    reset();
}


CMultiFilter::~CMultiFilter(void)
{
}


// =============================================================
// --- CFilter Overrides
// =============================================================
void CMultiFilter::reset()
{
    // flush everything
    m_OnePoleFilter.reset();
    m_MoogLadderFilter.reset();
    m_SEMFilter.reset();
}

void CMultiFilter::setQControl(double dQControl)
{
    CFilter::setQControl(dQControl);
    
    m_OnePoleFilter.setQControl(dQControl);
    m_MoogLadderFilter.setQControl(dQControl);
    m_SEMFilter.setQControl(dQControl);
}

double CMultiFilter::doFilter(double xn)
{
    switch(m_uFilterMode)
    {
        case Mode::MOOG_LADDER_LPF4:
        case Mode::MOOG_LADDER_LPF2:
        case Mode::MOOG_LADDER_BPF4:
        case Mode::MOOG_LADDER_BPF2:
        case Mode::MOOG_LADDER_HPF4:
        case Mode::MOOG_LADDER_HPF2:
            return m_MoogLadderFilter.doFilter(xn);
        case Mode::VA_ONE_POLE_LPF1:
        case Mode::VA_ONE_POLE_HPF1:
            return m_OnePoleFilter.doFilter(xn);
        case Mode::SEM_LPF2:
        case Mode::SEM_BPF2:
        case Mode::SEM_BSF2:
        case Mode::SEM_HPF2:
            return m_SEMFilter.doFilter(xn);
        case Mode::THREE_FIVE_LPF2:
        case Mode::THREE_FIVE_HPF2:
        case Mode::DIODE_LADDER_LPF4:
        default:
            return xn;
    }
}

void CMultiFilter::setSampleRate(double d)
{
    CFilter::setSampleRate(d);
    
    m_OnePoleFilter.setSampleRate(d);
    m_MoogLadderFilter.setSampleRate(d);
    m_SEMFilter.setSampleRate(d);
}

void CMultiFilter::update()
{
    CFilter::update();
    updateFilterMode();
    
    m_OnePoleFilter.update();
    m_MoogLadderFilter.update();
    m_SEMFilter.update();
}

void CMultiFilter::initGlobalParameters(globalFilterParams *pGlobalFilterParams)
{
    m_OnePoleFilter.initGlobalParameters(pGlobalFilterParams);
    m_MoogLadderFilter.initGlobalParameters(pGlobalFilterParams);
    m_SEMFilter.initGlobalParameters(pGlobalFilterParams);
    
    CFilter::initGlobalParameters(pGlobalFilterParams);
    m_pGlobalFilterParams->uFilterMode = m_uFilterMode;
}

// =============================================================
// --- Methods
// =============================================================
void CMultiFilter::updateFilterMode()
{
    if(m_pGlobalFilterParams)
    {
        if(m_uFilterMode != m_pGlobalFilterParams->uFilterMode)
        {
            m_uFilterMode = m_pGlobalFilterParams->uFilterMode;
            
            printf("-- Filter Mode Changed:%d \n", m_uFilterMode);
            
            switch(m_uFilterMode)
            {
                case Mode::MOOG_LADDER_LPF4:
                case Mode::DIODE_LADDER_LPF4:
                    setFilterType(LPF4);
                    break;
                case Mode::MOOG_LADDER_LPF2:
                case Mode::SEM_LPF2:
                case Mode::THREE_FIVE_LPF2:
                    setFilterType(LPF2);
                    break;
                case Mode::MOOG_LADDER_BPF4:
                    setFilterType(BPF4);
                    break;
                case Mode::MOOG_LADDER_BPF2:
                case Mode::SEM_BPF2:
                    setFilterType(BPF2);
                    break;
                case Mode::SEM_BSF2:
                    setFilterType(BSF2);
                    break;
                case Mode::MOOG_LADDER_HPF4:
                    setFilterType(HPF4);
                    break;
                case Mode::MOOG_LADDER_HPF2:
                case Mode::SEM_HPF2:
                case Mode::THREE_FIVE_HPF2:
                    setFilterType(HPF2);
                    break;
                case Mode::VA_ONE_POLE_LPF1:
                    setFilterType(LPF1);
                    break;
                case Mode::VA_ONE_POLE_HPF1:
                    setFilterType(HPF1);
                    break;
                default:
                    break;
            }
        }
    }
}

void CMultiFilter::setFilterType(UINT uFilterType)
{
    m_uFilterType = uFilterType;
    
    if(m_pGlobalFilterParams)
    {
        m_pGlobalFilterParams->uFilterType = uFilterType;
    }
}

void CMultiFilter::initModulationMatrixLinks()
{
    m_OnePoleFilter.m_pModulationMatrix = m_pModulationMatrix;
    m_OnePoleFilter.m_uModSourceFc = m_uModSourceFc;
    m_OnePoleFilter.m_uSourceFcControl = m_uSourceFcControl;

    m_MoogLadderFilter.m_pModulationMatrix = m_pModulationMatrix;
    m_MoogLadderFilter.m_uModSourceFc = m_uModSourceFc;
    m_MoogLadderFilter.m_uSourceFcControl = m_uSourceFcControl;
    
    m_SEMFilter.m_pModulationMatrix = m_pModulationMatrix;
    m_SEMFilter.m_uModSourceFc = m_uModSourceFc;
    m_SEMFilter.m_uSourceFcControl = m_uSourceFcControl;
}
