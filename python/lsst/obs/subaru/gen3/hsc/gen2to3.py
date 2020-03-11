# This file is part of obs_subaru.
#
# Developed for the LSST Data Management System.
# This product includes software developed by the LSST Project
# (http://www.lsst.org).
# See the COPYRIGHT file at the top-level directory of this distribution
# for details of code ownership.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

"""Gen2->Gen3 Data repo conversion specializations for Hyper Suprime-Cam
"""

from lsst.obs.base.gen2to3 import Translator, KeyHandler, ConstantKeyHandler, CopyKeyHandler

from ....hsc.hscFilters import HSC_FILTER_DEFINITIONS


__all__ = ()


class HscAbstractFilterKeyHandler(KeyHandler):
    """KeyHandler for HSC filter keys that should be mapped to AbstractFilters.
    """

    __slots__ = ("_map",)

    def __init__(self):
        super().__init__("abstract_filter")
        self._map = {d.physical_filter: d.abstract_filter for d in HSC_FILTER_DEFINITIONS
                     if d.physical_filter is not None}

    def extract(self, gen2id, skyMap, skyMapName, datasetTypeName):
        physical = gen2id["filter"]
        return self._map.get(physical, physical)


# Translate Gen2 `filter` to AbstractFilter if it hasn't been consumed yet and gen2keys includes tract.
Translator.addRule(HscAbstractFilterKeyHandler(), instrument="HSC", gen2keys=("filter", "tract"),
                   consume=("filter",))

# Add instrument for HSC transmission curve datasets (transmission_sensor is
# already handled by the above translators).
Translator.addRule(ConstantKeyHandler("instrument", "HSC"),
                   instrument="HSC", datasetTypeName="transmission_optics")
Translator.addRule(ConstantKeyHandler("instrument", "HSC"),
                   instrument="HSC", datasetTypeName="transmission_atmosphere")
Translator.addRule(ConstantKeyHandler("instrument", "HSC"),
                   instrument="HSC", datasetTypeName="transmission_filter")
Translator.addRule(CopyKeyHandler("physical_filter", "filter"),
                   instrument="HSC", datasetTypeName="transmission_filter")
