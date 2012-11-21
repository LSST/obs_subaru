#ifndef LSST_OBS_SUPRIMECAM_CROSSTALK_H
#define LSST_OBS_SUPRIMECAM_CROSSTALK_H

#include "lsst/afw/image/MaskedImage.h"

/*
 * @file HscDistortion.h
 * @brief C++ implementation of crosstalk correction with procedure by Yagi+2012
 * @ingroup obs_subaru
 * @author Hisanori Furusawa
 *
 */

namespace lsst {
    namespace obs {
        namespace suprimecam {
            std::vector<int> getCrosstalkX1(int x, int nxAmp = 512);
            std::vector<int> getCrosstalkX2(int x, int nxAmp = 512);
            void subtractCrossTalk(lsst::afw::image::MaskedImage<float> & mi, int nAmp,
                                                              std::vector< std::vector<double> > const & coeffs1List,
                                                              std::vector< std::vector<double> > const & coeffs2List,
                                                              std::vector< std::vector<double> > const & gainsPreampSig );

        }
    }
}

#endif
