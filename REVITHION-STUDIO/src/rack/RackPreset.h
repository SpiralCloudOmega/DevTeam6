#pragma once

#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>
#include <optional>
#include <unordered_map>
#include <vector>

#include "AudioModule.h"
#include "RackProcessor.h"

namespace revithion::rack {

// ============================================================================
// PresetTag — classification tags for presets
// ============================================================================

enum class PresetTag {
    Clean,
    Distorted,
    Ambient,
    Vocal,
    Mastering,
    Bass,
    Acoustic,
    Metal,
    Jazz,
    Blues,
    Pop,
    Rock,
    Electronic,
    Experimental,
    Template,
    UserCreated
};

inline juce::String presetTagToString(PresetTag tag)
{
    switch (tag)
    {
        case PresetTag::Clean:        return "Clean";
        case PresetTag::Distorted:    return "Distorted";
        case PresetTag::Ambient:      return "Ambient";
        case PresetTag::Vocal:        return "Vocal";
        case PresetTag::Mastering:    return "Mastering";
        case PresetTag::Bass:         return "Bass";
        case PresetTag::Acoustic:     return "Acoustic";
        case PresetTag::Metal:        return "Metal";
        case PresetTag::Jazz:         return "Jazz";
        case PresetTag::Blues:        return "Blues";
        case PresetTag::Pop:          return "Pop";
        case PresetTag::Rock:         return "Rock";
        case PresetTag::Electronic:   return "Electronic";
        case PresetTag::Experimental: return "Experimental";
        case PresetTag::Template:     return "Template";
        case PresetTag::UserCreated:  return "UserCreated";
    }
    return "Unknown";
}

inline PresetTag presetTagFromString(const juce::String& str)
{
    if (str == "Clean")        return PresetTag::Clean;
    if (str == "Distorted")    return PresetTag::Distorted;
    if (str == "Ambient")      return PresetTag::Ambient;
    if (str == "Vocal")        return PresetTag::Vocal;
    if (str == "Mastering")    return PresetTag::Mastering;
    if (str == "Bass")         return PresetTag::Bass;
    if (str == "Acoustic")     return PresetTag::Acoustic;
    if (str == "Metal")        return PresetTag::Metal;
    if (str == "Jazz")         return PresetTag::Jazz;
    if (str == "Blues")        return PresetTag::Blues;
    if (str == "Pop")          return PresetTag::Pop;
    if (str == "Rock")         return PresetTag::Rock;
    if (str == "Electronic")   return PresetTag::Electronic;
    if (str == "Experimental") return PresetTag::Experimental;
    if (str == "Template")     return PresetTag::Template;
    if (str == "UserCreated")  return PresetTag::UserCreated;
    return PresetTag::UserCreated;
}

// ============================================================================
// PresetMetadata — common metadata shared by all preset types
// ============================================================================

struct PresetMetadata
{
    juce::String          name;
    juce::String          author       { "REVITHION" };
    juce::String          description;
    juce::String          version      { "1.0" };
    juce::Time            createdAt    { juce::Time::getCurrentTime() };
    juce::Time            modifiedAt   { juce::Time::getCurrentTime() };
    std::vector<PresetTag> tags;
    ModuleCategory        targetCategory {};
    int                   rating       { 0 };
    bool                  isFavorite   { false };
    bool                  isFactory    { false };

    juce::ValueTree toValueTree() const
    {
        juce::ValueTree tree { "PresetMetadata" };
        tree.setProperty("name",           name,                          nullptr);
        tree.setProperty("author",         author,                        nullptr);
        tree.setProperty("description",    description,                   nullptr);
        tree.setProperty("version",        version,                       nullptr);
        tree.setProperty("createdAt",      createdAt.toMilliseconds(),    nullptr);
        tree.setProperty("modifiedAt",     modifiedAt.toMilliseconds(),   nullptr);
        tree.setProperty("targetCategory", static_cast<int>(targetCategory), nullptr);
        tree.setProperty("rating",         rating,                        nullptr);
        tree.setProperty("isFavorite",     isFavorite,                    nullptr);
        tree.setProperty("isFactory",      isFactory,                     nullptr);

        juce::ValueTree tagsTree { "Tags" };
        for (const auto& tag : tags)
        {
            juce::ValueTree tagNode { "Tag" };
            tagNode.setProperty("value", presetTagToString(tag), nullptr);
            tagsTree.appendChild(tagNode, nullptr);
        }
        tree.appendChild(tagsTree, nullptr);

        return tree;
    }

    static PresetMetadata fromValueTree(const juce::ValueTree& tree)
    {
        PresetMetadata m;
        m.name           = tree.getProperty("name",           "");
        m.author         = tree.getProperty("author",         "REVITHION");
        m.description    = tree.getProperty("description",    "");
        m.version        = tree.getProperty("version",        "1.0");
        m.createdAt      = juce::Time(static_cast<juce::int64>(tree.getProperty("createdAt",  0)));
        m.modifiedAt     = juce::Time(static_cast<juce::int64>(tree.getProperty("modifiedAt", 0)));
        m.targetCategory = static_cast<ModuleCategory>(static_cast<int>(tree.getProperty("targetCategory", 0)));
        m.rating         = tree.getProperty("rating",         0);
        m.isFavorite     = tree.getProperty("isFavorite",     false);
        m.isFactory      = tree.getProperty("isFactory",      false);

        auto tagsTree = tree.getChildWithName("Tags");
        for (int i = 0; i < tagsTree.getNumChildren(); ++i)
            m.tags.push_back(presetTagFromString(tagsTree.getChild(i).getProperty("value").toString()));

        return m;
    }
};

// ============================================================================
// ModulePreset — preset for a single module
// ============================================================================

struct ModulePreset
{
    PresetMetadata  metadata;
    juce::String    moduleTypeId;
    juce::ValueTree moduleState;

    juce::ValueTree toValueTree() const
    {
        juce::ValueTree tree { "ModulePreset" };
        tree.appendChild(metadata.toValueTree(), nullptr);
        tree.setProperty("moduleTypeId", moduleTypeId, nullptr);
        tree.appendChild(moduleState.createCopy(), nullptr);
        return tree;
    }

    static ModulePreset fromValueTree(const juce::ValueTree& tree)
    {
        ModulePreset p;
        p.metadata     = PresetMetadata::fromValueTree(tree.getChildWithName("PresetMetadata"));
        p.moduleTypeId = tree.getProperty("moduleTypeId", "");

        for (int i = 0; i < tree.getNumChildren(); ++i)
        {
            auto child = tree.getChild(i);
            if (child.getType().toString() != "PresetMetadata" && child.getType().toString() != "Tags")
            {
                p.moduleState = child.createCopy();
                break;
            }
        }

        return p;
    }
};

// ============================================================================
// RackPreset — preset for the entire rack (modules + cables + routing)
// ============================================================================

struct RackPreset
{
    PresetMetadata             metadata;
    juce::ValueTree            rackState;
    std::vector<juce::String>  requiredModules;

    juce::ValueTree toValueTree() const
    {
        juce::ValueTree tree { "RackPreset" };
        tree.appendChild(metadata.toValueTree(), nullptr);
        tree.appendChild(rackState.createCopy(),  nullptr);

        juce::ValueTree reqTree { "RequiredModules" };
        for (const auto& id : requiredModules)
        {
            juce::ValueTree node { "Module" };
            node.setProperty("typeId", id, nullptr);
            reqTree.appendChild(node, nullptr);
        }
        tree.appendChild(reqTree, nullptr);

        return tree;
    }

    static RackPreset fromValueTree(const juce::ValueTree& tree)
    {
        RackPreset p;
        p.metadata = PresetMetadata::fromValueTree(tree.getChildWithName("PresetMetadata"));

        auto reqTree = tree.getChildWithName("RequiredModules");
        for (int i = 0; i < reqTree.getNumChildren(); ++i)
            p.requiredModules.push_back(reqTree.getChild(i).getProperty("typeId").toString());

        for (int i = 0; i < tree.getNumChildren(); ++i)
        {
            auto child = tree.getChild(i);
            auto type  = child.getType().toString();
            if (type != "PresetMetadata" && type != "RequiredModules" && type != "Tags")
            {
                p.rackState = child.createCopy();
                break;
            }
        }

        return p;
    }
};

// ============================================================================
// DefaultRackTemplates — factory template presets (definitions in .cpp)
// ============================================================================

namespace DefaultRackTemplates {

// Input → Parametric EQ → Compressor → Plate Reverb → Output
RackPreset createCleanStudio();

// Input → Drive → Neural Amp → Cab Sim → Noise Gate → Delay → Output
RackPreset createHeavyMetal();

// Input → Chorus → Plate Reverb → Tape Delay → Phaser → Output
RackPreset createAmbientWash();

// Input → EQ → Compressor → De-esser → Plate Reverb → Output
RackPreset createVocalChain();

// Input → EQ → Multiband Comp → Limiter → Meter → Output
RackPreset createMasteringChain();

// Returns all built-in template presets
std::vector<RackPreset> getAllTemplates();

} // namespace DefaultRackTemplates

// ============================================================================
// PresetManager — persistent preset storage, search, and apply/capture
// ============================================================================

class PresetManager
{
public:
    // ---- Listener -----------------------------------------------------------

    class Listener
    {
    public:
        virtual ~Listener() = default;
        virtual void presetAdded(const juce::String& /*name*/) {}
        virtual void presetRemoved(const juce::String& /*name*/) {}
        virtual void presetListChanged() {}
    };

    // ---- Construction -------------------------------------------------------

    explicit PresetManager(const juce::File& presetDirectory)
        : presetDirectory_(presetDirectory)
    {
        if (!presetDirectory_.exists())
            presetDirectory_.createDirectory();
    }

    // ---- Rack presets -------------------------------------------------------

    void                          saveRackPreset(const RackPreset& preset);
    std::optional<RackPreset>     loadRackPreset(const juce::String& name) const;
    void                          deleteRackPreset(const juce::String& name);
    std::vector<RackPreset>       getAllRackPresets() const;
    std::vector<RackPreset>       searchRackPresets(const juce::String& query) const;
    std::vector<RackPreset>       getRackPresetsByTag(PresetTag tag) const;

    // ---- Module presets -----------------------------------------------------

    void                          saveModulePreset(const ModulePreset& preset);
    std::optional<ModulePreset>   loadModulePreset(const juce::String& moduleTypeId,
                                                   const juce::String& presetName) const;
    void                          deleteModulePreset(const juce::String& moduleTypeId,
                                                     const juce::String& presetName);
    std::vector<ModulePreset>     getPresetsForModule(const juce::String& moduleTypeId) const;

    // ---- Apply / capture ----------------------------------------------------

    void         applyRackPreset(RackProcessor& rack, const RackPreset& preset);
    RackPreset   captureRackState(const RackProcessor& rack, const PresetMetadata& metadata);
    void         applyModulePreset(AudioModule& module, const ModulePreset& preset);
    ModulePreset captureModuleState(const AudioModule& module, const PresetMetadata& metadata);

    // ---- JSON import / export -----------------------------------------------

    bool                          exportToJson(const RackPreset& preset,
                                               const juce::File& file) const;
    std::optional<RackPreset>     importFromJson(const juce::File& file) const;
    bool                          exportModulePresetToJson(const ModulePreset& preset,
                                                           const juce::File& file) const;
    std::optional<ModulePreset>   importModulePresetFromJson(const juce::File& file) const;

    // ---- Persistence --------------------------------------------------------

    void scanPresetDirectory();
    void initializeFactoryPresets();

    // ---- Listener management ------------------------------------------------

    void addListener(Listener* listener)    { listeners_.add(listener); }
    void removeListener(Listener* listener) { listeners_.remove(listener); }

private:
    juce::File                                                    presetDirectory_;
    std::vector<RackPreset>                                       rackPresets_;
    std::unordered_map<juce::String, std::vector<ModulePreset>>   modulePresets_;
    juce::CriticalSection                                         presetLock_;
    juce::ListenerList<Listener>                                  listeners_;
};

} // namespace revithion::rack
