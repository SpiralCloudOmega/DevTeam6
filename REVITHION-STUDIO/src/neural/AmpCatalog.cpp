#include "AmpCatalog.h"

#include <cstring>

namespace revithion::neural {

//==============================================================================
// Full amp model catalog — 103+ entries
// Source: Yeks Guide to Fractal Audio Amplifier Models + REVITHION-Book-Extractions.md
//
// Hidden size recommendations:
//   32 = clean/low-gain amps (linear response, simpler harmonics)
//   40 = crunch/medium-gain amps (moderate nonlinearity)
//   48 = high-gain amps (complex harmonic content)
//   64 = extreme high-gain / complex multi-stage amps
//
// Tiers:
//   1 = Train first (10 essential amps covering all genres)
//   2 = Train second (20 important amps for broader coverage)
//   3 = Train third (remaining amps for completeness)
//==============================================================================

static const std::vector<AmpCatalogEntry> kCatalog = {

    //==========================================================================
    // FENDER (16 entries)
    //==========================================================================

    // Tweed era
    {"5F1 Tweed",            "Fender '57 Champ (5F1)",                 "Fender", 32, 3},
    {"Deluxe Tweed",         "Fender '53 Deluxe (5E3)",                "Fender", 32, 2},
    {"Prince Tone",          "Fender '57 Tweed Princeton (5F2-A)",     "Fender", 32, 3},
    {"59 Bassguy",           "Fender '59 Bassman (5F6-A)",             "Fender", 40, 2},
    {"5F8 Tweed",            "Fender '58 Tweed Twin (5F8)",            "Fender", 32, 3},

    // Brown era
    {"6G4 Super",            "Fender '60 Brown Super (6G4)",           "Fender", 32, 3},
    {"Vibrato Lux",          "Fender '63 Vibrolux (6G11)",             "Fender", 32, 3},
    {"6G12 Concert",         "Fender '60 Brown Concert (6G12)",        "Fender", 32, 3},

    // Blackface / Silverface era
    {"Tremolo Lux",          "Fender '63 Tremolux (AA763)",            "Fender", 32, 3},
    {"Prince Tone Reverb",   "Fender '66 Princeton Reverb (AA763)",    "Fender", 32, 2},
    {"65 Bassguy",           "Fender '65 Bassman (AB165)",             "Fender", 32, 3},
    {"Band-Commander",       "Fender '68 Bandmaster (AB165)",          "Fender", 32, 3},
    {"Deluxe Verb",          "Fender '65 Deluxe Reverb (AB763)",       "Fender", 32, 1},
    {"Super Verb",           "Fender '64 Super Reverb (AB763)",        "Fender", 32, 3},
    {"Double Verb",          "Fender '65 Twin Reverb (AB763)",         "Fender", 32, 1},
    {"Double Verb SF",       "Fender '71 Twin Reverb Silverface",      "Fender", 32, 3},

    //==========================================================================
    // MARSHALL / British (15 entries)
    //==========================================================================

    {"Brit JM45",            "Marshall JTM 45",                        "Marshall", 40, 3},
    {"1987X",                "Marshall 1987X Plexi 50W",               "Marshall", 40, 3},
    {"Plexi 50W",            "Marshall Plexi 50W",                     "Marshall", 40, 3},
    {"Plexi 100W",           "Marshall Plexi 100W (1959 Super Lead)",  "Marshall", 40, 1},
    {"1959SLP",              "Marshall SLP1959 Plexi",                 "Marshall", 40, 3},
    {"Brit 800",             "Marshall JCM 800 (2204 50W)",            "Marshall", 48, 1},
    {"Brit 800 Mod",         "Marshall JCM 800 Modded",                "Marshall", 48, 3},
    {"Brit 800 #34",         "Marshall JCM 800 (#34 Jose Mod)",        "Marshall", 48, 3},
    {"Brit AFS100",          "Marshall AFD100 (Slash Signature)",       "Marshall", 48, 3},
    {"Brit JVM",             "Marshall JVM410H",                       "Marshall", 48, 3},
    {"Brit Silver",          "Marshall Silver Jubilee 2555",           "Marshall", 48, 2},
    {"Brit Pre",             "Marshall JMP-1 Preamp",                  "Marshall", 40, 3},
    {"JMPre-1 OD1",         "Marshall JMP-1 Preamp OD1",              "Marshall", 48, 3},
    {"JMPre-1 OD2",         "Marshall JMP-1 Preamp OD2",              "Marshall", 48, 3},
    {"Dirty Shirley",        "Friedman Dirty Shirley (Marshall-type)",  "Marshall", 40, 3},

    //==========================================================================
    // VOX / Class-A / EL84 (9 entries)
    //==========================================================================

    {"Class-A 15W TB",       "Vox AC15 Top Boost",                    "Vox", 32, 2},
    {"Class-A 30W",          "Vox AC30 Normal",                       "Vox", 32, 3},
    {"Class-A 30W TB",       "Vox AC30 Top Boost",                    "Vox", 40, 1},
    {"Class-A 30W BRT",      "Vox AC30 Bright",                       "Vox", 32, 3},
    {"Class-A 30W HOT",      "Vox AC30 Hot (high gain mod)",          "Vox", 40, 3},
    {"Citrus A30 Cln",       "Orange AD30HTC Clean",                  "Vox", 32, 3},
    {"Citrus A30 Drty",      "Orange AD30HTC Dirty",                  "Vox", 40, 3},
    {"Buttery",              "Budda Twinmaster",                      "Vox", 32, 3},
    {"Ruby Rocket",          "Paul Ruby Rocket",                      "Vox", 32, 3},

    //==========================================================================
    // HIGH-GAIN / Modern (26 entries)
    //==========================================================================

    // MESA/Boogie
    {"USA IIC+ Lead",        "MESA/Boogie Mark IIC+ Lead",            "HighGain", 64, 1},
    {"USA IIC++",            "MESA/Boogie Mark IIC+ Modded",          "HighGain", 64, 3},
    {"USA Clean",            "MESA/Boogie Mark IV Clean",             "HighGain", 32, 3},
    {"USA Rhythm",           "MESA/Boogie Mark IV Rhythm",            "HighGain", 48, 2},
    {"USA Lead",             "MESA/Boogie Mark IV Lead",              "HighGain", 48, 2},
    {"USA Lead+",            "MESA/Boogie Mark IV Lead+ (Bright)",    "HighGain", 48, 3},
    {"Recto1 Orange Modern", "MESA Dual Rectifier (2-ch)",            "HighGain", 64, 1},
    {"Recto2 Red Modern",    "MESA Dual Rectifier (3-ch Red Modern)", "HighGain", 64, 2},
    {"USA Pre Clean",        "MESA/Boogie TriAxis Clean",             "HighGain", 32, 3},
    {"USA Pre LD1",          "MESA/Boogie TriAxis Lead 1",            "HighGain", 48, 3},
    {"USA Pre LD2",          "MESA/Boogie TriAxis Lead 2",            "HighGain", 48, 3},

    // EVH / Peavey
    {"5153 Green",           "EVH 5150-III Green (Clean)",            "HighGain", 32, 3},
    {"5153 Blue",            "EVH 5150-III Blue (Crunch)",            "HighGain", 48, 1},
    {"5153 Red",             "EVH 5150-III Red (Lead)",               "HighGain", 64, 2},
    {"PVH 6160 Block",       "Peavey 5150 / 6505 Block Letter",      "HighGain", 64, 2},
    {"PVH 6160+",            "Peavey 6160+ / EVH 5150-II",           "HighGain", 64, 3},
    {"FAS 6160",             "FAS Custom 5150-style",                 "HighGain", 64, 3},

    // Soldano
    {"Solo 100 Lead",        "Soldano SLO-100 Lead",                  "HighGain", 64, 1},
    {"Solo 100 Clean",       "Soldano SLO-100 Clean",                "HighGain", 32, 3},
    {"Solo 88 Lead",         "Soldano X88R Preamp Lead",              "HighGain", 48, 3},
    {"Solo 99 Lead",         "Soldano X99 Preamp Lead",               "HighGain", 48, 3},

    // Bogner
    {"Euro Blue",            "Bogner Ecstasy Blue Channel",           "HighGain", 48, 2},
    {"Euro Red",             "Bogner Ecstasy Red Channel",            "HighGain", 64, 2},
    {"Euro Uber",            "Bogner Uberschall",                     "HighGain", 64, 2},

    // Diezel
    {"Das Metall",           "Diezel VH4 Ch.3",                      "HighGain", 64, 2},
    {"Dizzy V4 Blue",        "Diezel VH4 Blue (Clean)",              "HighGain", 32, 3},
    {"Herbie Ch2",           "Diezel Herbert Ch.2",                   "HighGain", 64, 3},
    {"Herbie Ch3",           "Diezel Herbert Ch.3",                   "HighGain", 64, 3},

    // Engl
    {"Angle Severe",         "Engl Savage 120",                       "HighGain", 64, 2},
    {"Energyball",           "Engl Powerball",                        "HighGain", 64, 2},

    // Friedman
    {"Friedman BE",          "Friedman BE-100 (Brown Eye)",           "HighGain", 48, 2},
    {"Friedman HBE",         "Friedman HBE (Hot Brown Eye)",         "HighGain", 64, 2},

    // Splawn
    {"Spawn Nitrous",        "Splawn Nitro",                          "HighGain", 48, 3},
    {"Spawn QRod",           "Splawn Quickrod",                       "HighGain", 48, 3},

    // Cameron
    {"Cameron CCV",          "Cameron CCV-100",                       "HighGain", 48, 3},
    {"Atomica High",         "Cameron Atomica High Gain",             "HighGain", 64, 3},

    //==========================================================================
    // BOUTIQUE / Dumble-style (12 entries)
    //==========================================================================

    {"ODS-100 Clean",        "Dumble Overdrive Special Clean",        "Boutique", 40, 1},
    {"ODS-100 Drive",        "Dumble Overdrive Special Drive",        "Boutique", 48, 3},
    {"Bludojai",             "Bludotone Ojai (Dumble-style)",         "Boutique", 40, 3},
    {"CA OD-2 Cln",          "Carol-Ann OD2 Clean",                   "Boutique", 32, 3},
    {"CA OD-2 OD",           "Carol-Ann OD2 Overdrive",               "Boutique", 40, 3},
    {"CA Triptik",           "Carol-Ann Triptik",                     "Boutique", 40, 3},
    {"CA Tucana",            "Carol-Ann Tucana 3",                    "Boutique", 40, 3},
    {"Fox ODS",              "Fuchs Overdrive Supreme",               "Boutique", 40, 3},
    {"Two Stone J35",        "Two-Rock Jet 35",                       "Boutique", 40, 3},
    {"Matchbox D-30",        "Matchless DC-30",                       "Boutique", 40, 2},
    {"Boutique 1",           "Matchless Chieftain (bright)",          "Boutique", 32, 3},
    {"Boutique 2",           "Matchless Chieftain (warm)",            "Boutique", 32, 3},

    //==========================================================================
    // OTHER / Miscellaneous (14 entries)
    //==========================================================================

    {"AC-20 12AX7",          "Morgan AC20 Deluxe (12AX7)",            "Boutique", 32, 3},
    {"AC-20 EF86",           "Morgan AC20 Deluxe (EF86)",             "Boutique", 32, 3},
    {"Wrecker Express",      "Trainwreck Express",                    "Boutique", 40, 3},
    {"Wrecker Liverpool",    "Trainwreck Liverpool",                  "Boutique", 40, 3},
    {"Wrecker Rocket",       "Trainwreck Rocket",                     "Boutique", 40, 3},
    {"Comet 60",             "Komet 60",                              "Boutique", 40, 3},
    {"Supremo Trem",         "Supro 1964T Dual-Tone",                 "Boutique", 32, 3},
    {"Nuclear-Tone",         "Swart Atomic Space Tone",               "Boutique", 32, 3},
    {"Car Roamer",           "Carr Rambler",                          "Boutique", 32, 3},
    {"TX Star",              "MESA Lone Star",                        "Boutique", 40, 3},
    {"Hipower",              "Hiwatt DR103",                          "Boutique", 40, 3},
    {"Jazz 120",             "Roland Jazz Chorus JC-120",             "Boutique", 32, 3},

    //==========================================================================
    // BASS (5 entries)
    //==========================================================================

    {"SV Bass",              "Ampeg SVT",                             "Bass", 48, 2},
    {"USA Bass 400",         "MESA Bass 400+",                        "Bass", 48, 3},
    {"FAS Bass",             "FAS Custom Bass Amp",                   "Bass", 40, 3},
    {"GK 800RB",             "Gallien-Krueger 800RB",                 "Bass", 40, 3},
    {"Darkglass Ultra",      "Darkglass Microtubes 900",              "Bass", 48, 3},

    //==========================================================================
    // FAS CUSTOM / Fractal Originals (14 entries)
    //==========================================================================

    {"FAS Brootalz",         "FAS Custom Extreme High-Gain",          "HighGain", 64, 3},
    {"FAS Brown",            "FAS Custom Brown Sound",                "HighGain", 48, 3},
    {"FAS Modern",           "FAS Custom Modern High-Gain",           "HighGain", 64, 3},
    {"FAS Modern II",        "FAS Custom Modern High-Gain II",        "HighGain", 64, 3},
    {"FAS Modern III",       "FAS Custom Modern High-Gain III",       "HighGain", 64, 3},
    {"FAS Lead 1",           "FAS Custom Lead 1",                     "HighGain", 48, 3},
    {"FAS Lead 2",           "FAS Custom Lead 2",                     "HighGain", 48, 3},
    {"FAS Crunch",           "FAS Custom Crunch",                     "Marshall", 40, 3},
    {"FAS Wreck",            "FAS Custom Trainwreck-style",           "Boutique", 40, 3},
    {"FAS Class-A",          "FAS Custom Class-A",                    "Vox",      32, 3},
    {"FAS Hot Rod",          "FAS Custom Hot Rod",                    "Fender",   40, 3},
    {"FAS Rhythm",           "FAS Custom Rhythm",                     "HighGain", 48, 3},
    {"Big Hair",             "FAS Custom '80s Style",                 "HighGain", 48, 3},
    {"Supertweed",           "FAS Custom Tweed-inspired",             "Fender",   32, 3},

    //==========================================================================
    // ACOUSTIC (3 entries)
    //==========================================================================

    {"Acoustic Sim 1",       "Piezo Acoustic Simulator (Bright)",     "Acoustic", 32, 3},
    {"Acoustic Sim 2",       "Piezo Acoustic Simulator (Warm)",       "Acoustic", 32, 3},
    {"Acoustic Sim 3",       "Magnetic-to-Acoustic Simulator",        "Acoustic", 32, 3},

    //==========================================================================
    // RACK PREAMPS — additional entries (6 entries)
    //==========================================================================

    {"Bogfish Brown",        "Bogner Fish Preamp (Brown)",            "HighGain", 48, 3},
    {"Bogfish Strato",       "Bogner Fish Preamp (Strato)",           "Boutique", 40, 3},
    {"CA3+ Clean",           "CAE 3+ SE Preamp Clean",               "Boutique", 32, 3},
    {"CA3+ Rhythm",          "CAE 3+ SE Preamp Rhythm",              "HighGain", 40, 3},
    {"CA3+ Lead",            "CAE 3+ SE Preamp Lead",                "HighGain", 48, 3},
    {"Solo 88 Clean",        "Soldano X88R Preamp Clean",             "HighGain", 32, 3},
};

const std::vector<AmpCatalogEntry>& getAmpCatalog()
{
    return kCatalog;
}

std::vector<AmpCatalogEntry> getTier1Amps()
{
    std::vector<AmpCatalogEntry> result;
    for (const auto& e : kCatalog)
        if (e.tier == 1)
            result.push_back(e);
    return result;
}

std::vector<AmpCatalogEntry> getAmpsByCategory(const char* category)
{
    std::vector<AmpCatalogEntry> result;
    for (const auto& e : kCatalog)
        if (std::strcmp(e.category, category) == 0)
            result.push_back(e);
    return result;
}

std::vector<AmpCatalogEntry> getAmpsByTier(int tier)
{
    std::vector<AmpCatalogEntry> result;
    for (const auto& e : kCatalog)
        if (e.tier == tier)
            result.push_back(e);
    return result;
}

} // namespace revithion::neural
