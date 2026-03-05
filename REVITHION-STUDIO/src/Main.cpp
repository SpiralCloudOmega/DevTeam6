#include <JuceHeader.h>
#include "ai/ACEStepBridge.h"
#include "engine/BoxGraph.h"
#include "engine/AudioGraph.h"

using namespace revithion::ai;
using namespace revithion::engine;

//==============================================================================
// REVITHION STUDIO — Main Application
// All-in-one audio production suite
// SpiralCloudOmega © 2026
//==============================================================================

namespace {
    // SpiralCloudOmega dark theme palette
    constexpr uint32_t kBgColor      = 0xFF121220;
    constexpr uint32_t kSurfaceColor = 0xFF1A1A2E;
    constexpr uint32_t kPanelColor   = 0xFF16162A;
    constexpr uint32_t kAccent       = 0xFF00D4FF;
    constexpr uint32_t kSecondary    = 0xFFFF6B35;
    constexpr uint32_t kSuccess      = 0xFF00FF88;
    constexpr uint32_t kTextDim      = 0xFF8888AA;
    constexpr uint32_t kGridLine     = 0xFF1E1E38;
    constexpr uint32_t kGridLineBeat = 0xFF2A2A48;
}

//==============================================================================
class RevithionMainComponent : public juce::Component,
                                public juce::Timer
{
public:
    RevithionMainComponent()
    {
        setSize(1920, 1080);

        setupTopBar();
        setupTransportControls();
        setupACEStepPanel();
        setupBottomBar();

        // Initialize engine subsystems
        boxGraph = std::make_unique<BoxGraph>();
        audioGraph = std::make_unique<AudioGraph>();

        // Check ACE-STEP health on startup (background thread)
        std::thread([this]() {
            bool healthy = aceBridge.checkHealth();
            juce::MessageManager::callAsync([this, healthy]() {
                aceConnected = healthy;
                updateConnectionStatus();
            });
        }).detach();

        // Poll ACE-STEP health every 10 seconds
        startTimer(10000);
    }

    ~RevithionMainComponent() override
    {
        stopTimer();
    }

    //==========================================================================
    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colour(kBgColor));

        // Draw center area grid (arrangement placeholder)
        auto centerArea = getCenterArea();
        g.setColour(juce::Colour(kPanelColor));
        g.fillRect(centerArea);

        // Vertical grid lines (beats)
        g.setColour(juce::Colour(kGridLine));
        for (int x = centerArea.getX(); x < centerArea.getRight(); x += 40)
            g.drawVerticalLine(x, static_cast<float>(centerArea.getY()),
                               static_cast<float>(centerArea.getBottom()));

        // Stronger lines every 4 beats (bars)
        g.setColour(juce::Colour(kGridLineBeat));
        for (int x = centerArea.getX(); x < centerArea.getRight(); x += 160)
            g.drawVerticalLine(x, static_cast<float>(centerArea.getY()),
                               static_cast<float>(centerArea.getBottom()));

        // Horizontal track lanes
        g.setColour(juce::Colour(kGridLine));
        for (int y = centerArea.getY(); y < centerArea.getBottom(); y += 60)
            g.drawHorizontalLine(y, static_cast<float>(centerArea.getX()),
                                 static_cast<float>(centerArea.getRight()));

        // Center overlay text
        g.setColour(juce::Colour(kTextDim).withAlpha(0.3f));
        g.setFont(juce::FontOptions(18.0f));
        g.drawText("ARRANGEMENT VIEW", centerArea, juce::Justification::centred);
    }

    void resized() override
    {
        auto bounds = getLocalBounds();

        // Top bar
        auto topBar = bounds.removeFromTop(48);
        layoutTopBar(topBar);

        // Transport bar
        auto transportBar = bounds.removeFromTop(40);
        layoutTransportBar(transportBar);

        // Bottom bar
        auto bottomBar = bounds.removeFromBottom(32);
        layoutBottomBar(bottomBar);

        // Main content: left panel + center
        auto content = bounds;
        auto leftPanel = content.removeFromLeft(300);
        layoutACEStepPanel(leftPanel);
    }

    void timerCallback() override
    {
        // Poll ACE-STEP health in background
        std::thread([this]() {
            bool healthy = aceBridge.checkHealth();
            juce::MessageManager::callAsync([this, healthy]() {
                aceConnected = healthy;
                updateConnectionStatus();
            });
        }).detach();
    }

private:
    //==========================================================================
    // Top Bar
    //==========================================================================
    void setupTopBar()
    {
        titleLabel.setText("REVITHION STUDIO", juce::dontSendNotification);
        titleLabel.setFont(juce::FontOptions(22.0f, juce::Font::bold));
        titleLabel.setJustificationType(juce::Justification::centredLeft);
        titleLabel.setColour(juce::Label::textColourId, juce::Colour(kAccent));
        addAndMakeVisible(titleLabel);

        versionLabel.setText("v0.1.0", juce::dontSendNotification);
        versionLabel.setFont(juce::FontOptions(12.0f));
        versionLabel.setJustificationType(juce::Justification::centredLeft);
        versionLabel.setColour(juce::Label::textColourId, juce::Colour(kTextDim));
        addAndMakeVisible(versionLabel);

        connectionLabel.setText("ACE-STEP: Checking...", juce::dontSendNotification);
        connectionLabel.setFont(juce::FontOptions(12.0f));
        connectionLabel.setJustificationType(juce::Justification::centredRight);
        connectionLabel.setColour(juce::Label::textColourId, juce::Colour(kTextDim));
        addAndMakeVisible(connectionLabel);
    }

    void layoutTopBar(juce::Rectangle<int> area)
    {
        area = area.reduced(12, 0);
        titleLabel.setBounds(area.removeFromLeft(220));
        versionLabel.setBounds(area.removeFromLeft(80));
        connectionLabel.setBounds(area.removeFromRight(250));
    }

    void updateConnectionStatus()
    {
        if (aceConnected)
        {
            connectionLabel.setText(juce::String(juce::CharPointer_UTF8("\xe2\x97\x8f")) + " ACE-STEP: Connected",
                                    juce::dontSendNotification);
            connectionLabel.setColour(juce::Label::textColourId, juce::Colour(kSuccess));
        }
        else
        {
            connectionLabel.setText(juce::String(juce::CharPointer_UTF8("\xe2\x97\x8f")) + " ACE-STEP: Offline",
                                    juce::dontSendNotification);
            connectionLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFFF4444));
        }
    }

    //==========================================================================
    // Transport Controls
    //==========================================================================
    void setupTransportControls()
    {
        auto setupTransportBtn = [this](juce::TextButton& btn, const juce::String& text, uint32_t colour)
        {
            btn.setButtonText(text);
            btn.setColour(juce::TextButton::buttonColourId, juce::Colour(kSurfaceColor));
            btn.setColour(juce::TextButton::textColourOnId, juce::Colour(colour));
            btn.setColour(juce::TextButton::textColourOffId, juce::Colour(colour));
            addAndMakeVisible(btn);
        };

        setupTransportBtn(playButton, "PLAY", kSuccess);
        setupTransportBtn(stopButton, "STOP", kAccent);
        setupTransportBtn(recordButton, "REC", kSecondary);

        playButton.onClick = [this]() {
            statusLabel.setText("Transport: Playing", juce::dontSendNotification);
        };
        stopButton.onClick = [this]() {
            statusLabel.setText("Transport: Stopped", juce::dontSendNotification);
            positionLabel.setText("00:00:00.000", juce::dontSendNotification);
        };
        recordButton.onClick = [this]() {
            statusLabel.setText("Transport: Recording", juce::dontSendNotification);
        };
    }

    void layoutTransportBar(juce::Rectangle<int> area)
    {
        area = area.reduced(12, 4);

        auto transportGroup = area.withSizeKeepingCentre(300, area.getHeight());
        playButton.setBounds(transportGroup.removeFromLeft(90));
        transportGroup.removeFromLeft(8);
        stopButton.setBounds(transportGroup.removeFromLeft(90));
        transportGroup.removeFromLeft(8);
        recordButton.setBounds(transportGroup.removeFromLeft(90));
    }

    //==========================================================================
    // ACE-STEP Panel (left sidebar)
    //==========================================================================
    void setupACEStepPanel()
    {
        // Section label
        acePanelLabel.setText("ACE-STEP", juce::dontSendNotification);
        acePanelLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
        acePanelLabel.setColour(juce::Label::textColourId, juce::Colour(kAccent));
        acePanelLabel.setJustificationType(juce::Justification::centredLeft);
        addAndMakeVisible(acePanelLabel);

        // Prompt editor
        auto setupEditorLabel = [this](juce::Label& lbl, const juce::String& text) {
            lbl.setText(text, juce::dontSendNotification);
            lbl.setFont(juce::FontOptions(11.0f));
            lbl.setColour(juce::Label::textColourId, juce::Colour(kTextDim));
            addAndMakeVisible(lbl);
        };

        setupEditorLabel(promptLabel, "Prompt");
        promptEditor.setMultiLine(true, true);
        promptEditor.setReturnKeyStartsNewLine(true);
        promptEditor.setColour(juce::TextEditor::backgroundColourId, juce::Colour(kSurfaceColor));
        promptEditor.setColour(juce::TextEditor::textColourId, juce::Colours::white);
        promptEditor.setColour(juce::TextEditor::outlineColourId, juce::Colour(kGridLineBeat));
        promptEditor.setTextToShowWhenEmpty("Describe the music you want to generate...",
                                            juce::Colour(kTextDim));
        addAndMakeVisible(promptEditor);

        // Lyrics editor
        setupEditorLabel(lyricsLabel, "Lyrics");
        lyricsEditor.setMultiLine(true, true);
        lyricsEditor.setReturnKeyStartsNewLine(true);
        lyricsEditor.setColour(juce::TextEditor::backgroundColourId, juce::Colour(kSurfaceColor));
        lyricsEditor.setColour(juce::TextEditor::textColourId, juce::Colours::white);
        lyricsEditor.setColour(juce::TextEditor::outlineColourId, juce::Colour(kGridLineBeat));
        lyricsEditor.setTextToShowWhenEmpty("Enter lyrics (optional)...",
                                            juce::Colour(kTextDim));
        addAndMakeVisible(lyricsEditor);

        // Sliders
        auto setupSlider = [this](juce::Slider& slider, juce::Label& label,
                                  const juce::String& text, double min, double max,
                                  double defaultVal, double interval)
        {
            label.setText(text, juce::dontSendNotification);
            label.setFont(juce::FontOptions(11.0f));
            label.setColour(juce::Label::textColourId, juce::Colour(kTextDim));
            addAndMakeVisible(label);

            slider.setRange(min, max, interval);
            slider.setValue(defaultVal, juce::dontSendNotification);
            slider.setSliderStyle(juce::Slider::LinearHorizontal);
            slider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
            slider.setColour(juce::Slider::backgroundColourId, juce::Colour(kSurfaceColor));
            slider.setColour(juce::Slider::thumbColourId, juce::Colour(kAccent));
            slider.setColour(juce::Slider::trackColourId, juce::Colour(kAccent).withAlpha(0.5f));
            slider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
            slider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(kSurfaceColor));
            slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
            addAndMakeVisible(slider);
        };

        setupSlider(bpmSlider, bpmLabel, "BPM", 40.0, 300.0, 120.0, 1.0);
        setupSlider(durationSlider, durationLabel, "Duration (s)", 5.0, 180.0, 30.0, 1.0);
        setupSlider(stepsSlider, stepsLabel, "Steps", 4.0, 50.0, 8.0, 1.0);
        setupSlider(guidanceSlider, guidanceLabel, "Guidance", 1.0, 15.0, 7.0, 0.1);

        // Generate button
        generateButton.setButtonText("Generate");
        generateButton.setColour(juce::TextButton::buttonColourId, juce::Colour(kAccent));
        generateButton.setColour(juce::TextButton::textColourOnId, juce::Colour(kBgColor));
        generateButton.setColour(juce::TextButton::textColourOffId, juce::Colour(kBgColor));
        addAndMakeVisible(generateButton);

        generateButton.onClick = [this]() { onGenerateClicked(); };
    }

    void layoutACEStepPanel(juce::Rectangle<int> area)
    {
        area = area.reduced(10, 6);
        constexpr int sliderRowHeight = 22;
        constexpr int labelHeight = 16;
        constexpr int gap = 4;

        acePanelLabel.setBounds(area.removeFromTop(24));
        area.removeFromTop(gap);

        promptLabel.setBounds(area.removeFromTop(labelHeight));
        promptEditor.setBounds(area.removeFromTop(90));
        area.removeFromTop(gap);

        lyricsLabel.setBounds(area.removeFromTop(labelHeight));
        lyricsEditor.setBounds(area.removeFromTop(90));
        area.removeFromTop(gap + 2);

        auto layoutSliderRow = [&](juce::Label& lbl, juce::Slider& slider) {
            lbl.setBounds(area.removeFromTop(labelHeight));
            slider.setBounds(area.removeFromTop(sliderRowHeight));
            area.removeFromTop(gap);
        };

        layoutSliderRow(bpmLabel, bpmSlider);
        layoutSliderRow(durationLabel, durationSlider);
        layoutSliderRow(stepsLabel, stepsSlider);
        layoutSliderRow(guidanceLabel, guidanceSlider);

        area.removeFromTop(6);
        generateButton.setBounds(area.removeFromTop(36));
    }

    //==========================================================================
    // Bottom Bar
    //==========================================================================
    void setupBottomBar()
    {
        statusLabel.setText("Ready", juce::dontSendNotification);
        statusLabel.setFont(juce::FontOptions(12.0f));
        statusLabel.setJustificationType(juce::Justification::centredLeft);
        statusLabel.setColour(juce::Label::textColourId, juce::Colour(kTextDim));
        addAndMakeVisible(statusLabel);

        positionLabel.setText("00:00:00.000", juce::dontSendNotification);
        positionLabel.setFont(juce::FontOptions(12.0f));
        positionLabel.setJustificationType(juce::Justification::centredRight);
        positionLabel.setColour(juce::Label::textColourId, juce::Colour(kAccent));
        addAndMakeVisible(positionLabel);
    }

    void layoutBottomBar(juce::Rectangle<int> area)
    {
        area = area.reduced(12, 0);
        statusLabel.setBounds(area.removeFromLeft(area.getWidth() / 2));
        positionLabel.setBounds(area);
    }

    //==========================================================================
    // Center area bounds helper
    //==========================================================================
    juce::Rectangle<int> getCenterArea() const
    {
        auto bounds = getLocalBounds();
        bounds.removeFromTop(88);   // top bar + transport
        bounds.removeFromBottom(32); // bottom bar
        bounds.removeFromLeft(300);  // ACE-STEP panel
        return bounds;
    }

    //==========================================================================
    // Generate action
    //==========================================================================
    void onGenerateClicked()
    {
        if (!aceConnected)
        {
            statusLabel.setText("Error: ACE-STEP not connected", juce::dontSendNotification);
            statusLabel.setColour(juce::Label::textColourId, juce::Colour(kSecondary));
            return;
        }

        GenerationRequest request;
        request.caption        = promptEditor.getText().toStdString();
        request.lyrics         = lyricsEditor.getText().toStdString();
        request.bpm            = static_cast<float>(bpmSlider.getValue());
        request.duration       = static_cast<float>(durationSlider.getValue());
        request.inferenceSteps = static_cast<int>(stepsSlider.getValue());
        request.guidanceScale  = static_cast<float>(guidanceSlider.getValue());

        generateButton.setEnabled(false);
        statusLabel.setText("Generating...", juce::dontSendNotification);
        statusLabel.setColour(juce::Label::textColourId, juce::Colour(kSecondary));

        aceBridge.generateMusic(request, [this](const GenerationResult& result) {
            juce::MessageManager::callAsync([this, result]() {
                generateButton.setEnabled(true);

                if (result.success)
                {
                    statusLabel.setText("Generated successfully! (" +
                        juce::String(result.audioData.size() / 1024) + " KB)",
                        juce::dontSendNotification);
                    statusLabel.setColour(juce::Label::textColourId, juce::Colour(kSuccess));
                }
                else
                {
                    statusLabel.setText("Error: " + juce::String(result.error),
                                        juce::dontSendNotification);
                    statusLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFFF4444));
                }
            });
        });
    }

    //==========================================================================
    // Members
    //==========================================================================

    // Engine subsystems
    ACEStepBridge                     aceBridge;
    std::unique_ptr<BoxGraph>         boxGraph;
    std::unique_ptr<AudioGraph>       audioGraph;
    bool                              aceConnected = false;

    // Top bar
    juce::Label titleLabel;
    juce::Label versionLabel;
    juce::Label connectionLabel;

    // Transport
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::TextButton recordButton;

    // ACE-STEP panel
    juce::Label      acePanelLabel;
    juce::Label      promptLabel;
    juce::TextEditor promptEditor;
    juce::Label      lyricsLabel;
    juce::TextEditor lyricsEditor;
    juce::Label      bpmLabel;
    juce::Slider     bpmSlider;
    juce::Label      durationLabel;
    juce::Slider     durationSlider;
    juce::Label      stepsLabel;
    juce::Slider     stepsSlider;
    juce::Label      guidanceLabel;
    juce::Slider     guidanceSlider;
    juce::TextButton generateButton;

    // Bottom bar
    juce::Label statusLabel;
    juce::Label positionLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RevithionMainComponent)
};

//==============================================================================
class RevithionApplication : public juce::JUCEApplication
{
public:
    RevithionApplication() {}

    const juce::String getApplicationName() override    { return "REVITHION STUDIO"; }
    const juce::String getApplicationVersion() override { return "0.1.0"; }
    bool moreThanOneInstanceAllowed() override           { return false; }

    void initialise(const juce::String&) override
    {
        mainWindow.reset(new MainWindow(getApplicationName()));
    }

    void shutdown() override
    {
        mainWindow = nullptr;
    }

    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(juce::String name)
            : DocumentWindow(name,
                juce::Colour(0x12, 0x12, 0x20),
                DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new RevithionMainComponent(), true);
            setResizable(true, true);
            centreWithSize(1920, 1080);
            setVisible(true);
        }

        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION(RevithionApplication)
