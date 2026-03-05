#pragma once

#include "NeuralAmpModel.h"
#include <vector>

namespace revithion::neural {

// Full amp model catalog — organized by category (from Yeks Guide + REVITHION-Book-Extractions.md)
struct AmpCatalogEntry {
    const char* name;
    const char* realAmp;       // Real-world amp it models
    const char* category;      // Fender, Marshall, Vox, HighGain, Boutique, Bass, Other
    int defaultHiddenSize;     // Recommended LSTM hidden size
    int tier;                  // Training priority (1=first, 2=second, 3=third)
};

// Get the full catalog
const std::vector<AmpCatalogEntry>& getAmpCatalog();

// Tier 1 — Train these FIRST (10 amps):
// 1. USA IIC+ Lead        (MESA Boogie Mark IIC+)
// 2. Solo 100 Lead        (Soldano SLO-100)
// 3. Plexi 100W           (Marshall 1959 Super Lead)
// 4. Double Verb           (Fender Twin Reverb)
// 5. Deluxe Verb           (Fender Deluxe Reverb)
// 6. Brit 800              (Marshall JCM800 2203)
// 7. 5153 Blue             (EVH 5150 III Blue)
// 8. Recto1 Orange Modern  (MESA Dual Rectifier)
// 9. Class-A 30W TB        (Vox AC30 Top Boost)
// 10. ODS-100 Clean        (Dumble Overdrive Special)

// Get Tier 1 entries only
std::vector<AmpCatalogEntry> getTier1Amps();

// Get entries filtered by category
std::vector<AmpCatalogEntry> getAmpsByCategory(const char* category);

// Get entries filtered by tier
std::vector<AmpCatalogEntry> getAmpsByTier(int tier);

} // namespace revithion::neural
