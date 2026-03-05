#include <JuceHeader.h>

//==============================================================================
// REVITHION STUDIO — Main Application
// All-in-one audio production suite
// SpiralCloudOmega © 2026
//==============================================================================

class RevithionMainComponent : public juce::Component,
                                public juce::Button::Listener
{
public:
    RevithionMainComponent()
    {
        setSize(1280, 800);

        // Title
        titleLabel.setText("REVITHION STUDIO", juce::dontSendNotification);
        titleLabel.setFont(juce::FontOptions(32.0f, juce::Font::bold));
        titleLabel.setJustificationType(juce::Justification::centred);
        titleLabel.setColour(juce::Label::textColourId, juce::Colour(0x00, 0xD4, 0xFF));
        addAndMakeVisible(titleLabel);

        // Version
        versionLabel.setText("v0.1.0 — Foundation Build", juce::dontSendNotification);
        versionLabel.setFont(juce::FontOptions(14.0f));
        versionLabel.setJustificationType(juce::Justification::centred);
        versionLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
        addAndMakeVisible(versionLabel);

        // ACE-STEP Generate Button
        generateButton.setButtonText("Generate Music (ACE-STEP)");
        generateButton.addListener(this);
        generateButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0x1A, 0x1A, 0x2E));
        generateButton.setColour(juce::TextButton::textColourOnId, juce::Colour(0x00, 0xD4, 0xFF));
        addAndMakeVisible(generateButton);

        // Status
        statusLabel.setText("ACE-STEP API: localhost:8001", juce::dontSendNotification);
        statusLabel.setFont(juce::FontOptions(12.0f));
        statusLabel.setJustificationType(juce::Justification::centred);
        statusLabel.setColour(juce::Label::textColourId, juce::Colours::green);
        addAndMakeVisible(statusLabel);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colour(0x12, 0x12, 0x20));
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(40);
        titleLabel.setBounds(area.removeFromTop(50));
        versionLabel.setBounds(area.removeFromTop(30));
        area.removeFromTop(40);
        generateButton.setBounds(area.removeFromTop(50).withSizeKeepingCentre(300, 50));
        area.removeFromTop(20);
        statusLabel.setBounds(area.removeFromTop(30));
    }

    void buttonClicked(juce::Button* button) override
    {
        if (button == &generateButton)
        {
            statusLabel.setText("Generating... (calling ACE-STEP API)", juce::dontSendNotification);
            statusLabel.setColour(juce::Label::textColourId, juce::Colours::yellow);
            // TODO: Call ACEStepBridge::generate() here
        }
    }

private:
    juce::Label titleLabel;
    juce::Label versionLabel;
    juce::TextButton generateButton;
    juce::Label statusLabel;

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
            centreWithSize(1280, 800);
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
