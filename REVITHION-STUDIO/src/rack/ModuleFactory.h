#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <unordered_map>

#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>

#include "AudioModule.h"

namespace revithion::rack
{

// Factory function type for creating module instances
using ModuleCreator = std::function<std::unique_ptr<AudioModule>()>;

struct ModuleRegistration
{
    ModuleInfo info;
    ModuleCreator creator;
    juce::Time registeredAt;
    bool isBuiltIn = false;
};

class ModuleFactory
{
public:
    static ModuleFactory& getInstance()
    {
        static ModuleFactory instance;
        return instance;
    }

    ModuleFactory(const ModuleFactory&) = delete;
    ModuleFactory& operator=(const ModuleFactory&) = delete;

    // ── Registration ────────────────────────────────────────────────────

    void registerModule(const ModuleInfo& info, ModuleCreator creator, bool builtIn = false)
    {
        const juce::ScopedLock lock(registryLock_);

        ModuleRegistration reg;
        reg.info = info;
        reg.creator = std::move(creator);
        reg.registeredAt = juce::Time::getCurrentTime();
        reg.isBuiltIn = builtIn;

        registry_.insert_or_assign(info.uniqueId, std::move(reg));
        listeners_.call([&](Listener& l) { l.moduleRegistered(info); });
    }

    void unregisterModule(const juce::String& uniqueId)
    {
        const juce::ScopedLock lock(registryLock_);

        if (registry_.erase(uniqueId) > 0)
            listeners_.call([&](Listener& l) { l.moduleUnregistered(uniqueId); });
    }

    bool isRegistered(const juce::String& uniqueId) const
    {
        const juce::ScopedLock lock(registryLock_);
        return registry_.count(uniqueId) > 0;
    }

    // Template helper — T must derive from AudioModule and expose getStaticInfo()
    template<typename T>
    void registerModule(bool builtIn = false)
    {
        static_assert(std::is_base_of_v<AudioModule, T>,
                      "T must derive from AudioModule");

        registerModule(
            T::getStaticInfo(),
            []() -> std::unique_ptr<AudioModule> { return std::make_unique<T>(); },
            builtIn);
    }

    // ── Creation ────────────────────────────────────────────────────────

    std::unique_ptr<AudioModule> createModule(const juce::String& uniqueId) const
    {
        const juce::ScopedLock lock(registryLock_);

        auto it = registry_.find(uniqueId);
        if (it == registry_.end())
            return nullptr;

        return it->second.creator();
    }

    std::unique_ptr<AudioModule> createModuleWithState(const juce::String& uniqueId,
                                                       const juce::ValueTree& state) const
    {
        auto module = createModule(uniqueId);

        if (module != nullptr)
            module->restoreState(state);

        return module;
    }

    // ── Queries ─────────────────────────────────────────────────────────

    std::vector<ModuleInfo> getAvailableModules() const
    {
        const juce::ScopedLock lock(registryLock_);

        std::vector<ModuleInfo> result;
        result.reserve(registry_.size());

        for (const auto& [id, reg] : registry_)
            result.push_back(reg.info);

        return result;
    }

    std::vector<ModuleInfo> getModulesByCategory(ModuleCategory category) const
    {
        const juce::ScopedLock lock(registryLock_);

        std::vector<ModuleInfo> result;

        for (const auto& [id, reg] : registry_)
        {
            if (reg.info.category == category)
                result.push_back(reg.info);
        }

        return result;
    }

    std::vector<ModuleInfo> searchModules(const juce::String& query) const
    {
        const juce::ScopedLock lock(registryLock_);

        auto lowerQuery = query.toLowerCase();
        std::vector<ModuleInfo> result;

        for (const auto& [id, reg] : registry_)
        {
            if (reg.info.displayName.toLowerCase().contains(lowerQuery))
            {
                result.push_back(reg.info);
                continue;
            }

            for (const auto& tag : reg.info.tags)
            {
                if (tag.toLowerCase().contains(lowerQuery))
                {
                    result.push_back(reg.info);
                    break;
                }
            }
        }

        return result;
    }

    const ModuleRegistration* getRegistration(const juce::String& uniqueId) const
    {
        const juce::ScopedLock lock(registryLock_);

        auto it = registry_.find(uniqueId);
        if (it == registry_.end())
            return nullptr;

        return &it->second;
    }

    int getRegisteredCount() const
    {
        const juce::ScopedLock lock(registryLock_);
        return static_cast<int>(registry_.size());
    }

    // ── Built-in module bootstrap ───────────────────────────────────────

    void registerBuiltInModules();

    // ── Listener ────────────────────────────────────────────────────────

    class Listener
    {
    public:
        virtual ~Listener() = default;
        virtual void moduleRegistered(const ModuleInfo& /*info*/) {}
        virtual void moduleUnregistered(const juce::String& /*uniqueId*/) {}
    };

    void addListener(Listener* listener)       { listeners_.add(listener); }
    void removeListener(Listener* listener)    { listeners_.remove(listener); }

private:
    ModuleFactory() = default;

    std::unordered_map<juce::String, ModuleRegistration> registry_;
    mutable juce::CriticalSection registryLock_;
    juce::ListenerList<Listener> listeners_;
};

} // namespace revithion::rack

#define REVITHION_REGISTER_MODULE(ModuleClass) \
    revithion::rack::ModuleFactory::getInstance().registerModule<ModuleClass>(true)
