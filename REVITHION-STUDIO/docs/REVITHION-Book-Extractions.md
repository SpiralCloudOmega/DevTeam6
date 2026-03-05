# REVITHION STUDIO — Technical Book Extractions
## Source References for Neural Amp Modeling & Target Amp Catalog

**Extracted from:**
1. **Yee-King, M.** — *Build AI-Enhanced Audio Plugins with C++* (2024, Routledge, 362 pages)
2. **Yek (Alexander van Engelen)** — *Guide to the Fractal Audio Amplifier Models* (301 pages, Q7.02 firmware)

---

# PART 1: YEE-KING — AI-Enhanced Audio Plugins with C++

## Book Structure

| Part | Chapters | Topic | Relevance to REVITHION |
|------|----------|-------|----------------------|
| I | 1-9 | Getting Started: JUCE, CMake, libtorch, Python, Basic Plugin Dev | Build system foundation |
| II | 10-19 | ML-Powered Plugin Control (Meta-Controller, Torchknob) | Smart parameter control |
| III | 20-25 | Autonomous Music Improviser (Markov, MIDI sequencing) | Not directly relevant |
| **IV** | **26-38** | **Neural Audio Effects** | **PRIMARY — Neural Amp Modeling** |

---

## 1.1 JUCE Plugin Architecture (Ch 8-9)

### Plugin Lifecycle
1. `PluginProcessor` constructor — initialize variables (audio system NOT available yet)
2. `prepareToPlay()` — audio system ready; sample rate and block size now known
3. `processBlock()` — called repeatedly; generate/process audio, fill output buffer
4. `releaseResources()` — audio system shutting down
5. `PluginProcessor` destructor

### Key Classes
```
AudioProcessor (JUCE base)
  └── TestPluginAudioProcessor (your plugin)
       ├── processBlock(AudioBuffer<float>& buffer, MidiBuffer& midi)
       ├── prepareToPlay(double sampleRate, int samplesPerBlock)
       └── releaseResources()

AudioProcessorEditor (JUCE base)
  └── TestPluginAudioProcessorEditor (your UI)
       ├── paint(Graphics& g)
       ├── resized()
       └── sliderValueChanged(Slider* slider)
```

### processBlock Pattern
```cpp
for (int channel = 0; channel < totalNumInputChannels; ++channel) {
    auto* outData = buffer.getWritePointer(channel);
    auto* inData = buffer.getReadPointer(channel);
    for (auto i = 0; i < buffer.getNumSamples(); ++i) {
        outData[i] = processSample(inData[i]); // your DSP here
    }
}
```

### Plugin Parameters (AudioProcessorValueTreeState)
- Parameters exposed to DAW automation
- Thread-safe parameter access
- FM Synth example: frequency, modIndex, modDepth as AudioParameterFloat

---

## 1.2 CMake Build System (Ch 4-5)

### Minimal JUCE + CMake Plugin
```cmake
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(my-plugin)
add_subdirectory(../../JUCE ./JUCE)

juce_add_plugin(my-plugin
    PLUGIN_CODE Mypg
    PRODUCT_NAME "My Plugin"
    IS_SYNTH FALSE
    FORMATS VST3 Standalone
)

target_sources(my-plugin PRIVATE
    src/PluginEditor.cpp
    src/PluginProcessor.cpp
)

target_link_libraries(my-plugin
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags
)
```

### Adding libtorch
```cmake
set(CMAKE_PREFIX_PATH "../../src_resources/libtorch/libtorch")
find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")
# Then in target_link_libraries:
target_link_libraries(my-plugin PRIVATE ${TORCH_LIBRARIES})
```

### Adding RTNeural
```cmake
add_subdirectory(../RTNeural ${CMAKE_CURRENT_BINARY_DIR}/RTNeural)
include_directories(../RTNeural)
# Link in target_link_libraries
```

### Adding juce_dsp (for convolution)
```cmake
target_link_libraries(convolution_plugin
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags
)
```

---

## 1.3 Torchknob Concept (Ch 14-15)

The **torchknob** maps a single knob to multiple synth parameters via a trained neural network:

### Architecture
```
Torchknob (single control)
    → Neural Network (Linear → Softmax)
        → slider 1 (e.g. Mod Ratio)
        → slider 2 (e.g. Mod Index)
        → ... (any number of target params)
```

### Workflow (Wekinator-style)
1. Set target parameter sliders to desired position
2. Set torchknob control to desired position
3. Store mapping as training data point
4. Repeat for more input→output associations
5. Train neural network on collected data
6. Move torchknob → network infers all parameters

### NeuralNetwork Class
```cpp
class NeuralNetwork : torch::nn::Module {
public:
    NeuralNetwork(int64_t n_inputs, int64_t n_outputs);
    std::vector<float> forward(const std::vector<float>& inputs);
    void addTrainingData(std::vector<float> inputs, std::vector<float> outputs);
    void runTraining(int epochs);
private:
    int64_t n_inputs;
    int64_t n_outputs;
    torch::nn::Linear linear{nullptr};
    torch::Tensor forward(const torch::Tensor& input);
};
```

**REVITHION Application**: Torchknob concept → "tone word" knob that maps descriptive text to amp model parameters (gain, bass, mid, treble, presence).

---

## 1.4 Neural Amp Emulation Theory (Ch 26, 31)

### Audio Effects History → Neural Effects
- **Era 1**: Tape manipulation (1900s-1950s)
- **Era 2**: Electro-mechanical (Leslie speaker, spring reverb)
- **Era 3**: Analogue signal processing (distortion, compression, delay)
- **Era 4**: DSP (digital effects, VST plugins from 1996)
- **Era 5 (Neural)**: Neural networks emulating analog circuits (2010s+)

### Modeling Approaches
| Approach | Description | Limitation |
|----------|-------------|------------|
| **White-box** | Circuit-level modeling (Wave Digital) | Requires circuit knowledge, computationally expensive |
| **Grey-box** | Chained black-box modules (preamp→tone→cab) | Limited by component models |
| **Black-box** | Input→output only (convolution, neural) | Traditional black-box can't model non-linear behavior |
| **Neural** | Learned black-box (LSTM, CNN) | Best of both: models non-linear behavior via training |

### Key Insight: Neural Networks ↔ DSP
> "Convolutional neural networks (CNNs) can be thought of as a type of **non-linear FIR filter**, and recurrent neural networks (RNNs) as a type of **non-linear IIR filter**." — Native Instruments team
>
> Neural network activation functions = **Waveshaping**

### The DSP Trinity
1. **FIR filters** → Convolution (linear, time-invariant) → CNNs
2. **IIR filters** → Feedback/recursive (linear) → RNNs/LSTMs
3. **Waveshapers** → Non-linear distortion → Activation functions

### Training Pipeline (4 stages)
1. **Capture**: Pass test signal through real amp → record output
2. **Forward**: Pass same signal through neural network
3. **Error**: Compute difference between amp output and network output
4. **Backpropagate**: Adjust network parameters to minimize error → back to step 2

---

## 1.5 LSTM Neural Network for Amp Emulation (Ch 32-33)

### Why LSTM?
- LSTM = Long Short-Term Memory (Hochreiter & Schmidhuber, 1997)
- Acts as **non-linear IIR filter** — remembers previous inputs/outputs
- 4 parameterized gates: input, forget, output, update
- Single LSTM unit already creates **harmonic distortion** on sine waves
- Can model the "stateful" behavior of valve amplifiers

### Basic LSTM in Python
```python
import torch
# LSTM(input_size, hidden_size, num_layers)
my_lstm = torch.nn.LSTM(1, 1, 1)  # 1 input, 1 hidden unit, 1 layer

# Pass audio sample-by-sample or as sequence
in_t = torch.tensor([[1.0], [0], [0], [0], [0], [0]])  # impulse
out_t, hx = my_lstm.forward(in_t)
# Output: [[0.0318], [0.1224], [0.1459], [0.1563], [0.1615], [0.1642]]
```

### Passing Audio Through LSTM
```python
import torch
import numpy as np
from scipy.io.wavfile import write

my_lstm = torch.nn.LSTM(1, 1, 1)  # minimal: 1 hidden unit
# For real amp modeling: torch.nn.LSTM(1, 32, 1) or (1, 64, 1)

# Synthesize test signal
freq = 400
clean_sine = np.sin(np.arange(0, np.pi*freq*2, (np.pi*freq*2)/44100), dtype=np.float32)
clean_sine = np.reshape(clean_sine, (len(clean_sine), 1))

in_t = torch.tensor(clean_sine)
out_t, hx = my_lstm.forward(in_t)

write('sine_400.wav', 44100, clean_sine * 0.5)
write('sine_400-lstm.wav', 44100, out_t.detach().numpy())
```

### LSTM State Management (Critical for Plugins!)

**Problem**: Block-based processing resets LSTM state → audible glitches at block boundaries.

**Solution**: Retain hidden state between processBlock calls:
```python
# WRONG - state resets each block:
output, _ = my_lstm.forward(input)

# CORRECT - retain state:
output, state = my_lstm.forward(input)
# ... next block:
output, state = my_lstm.forward(input, state)  # pass state back in
```

### JUCE LSTM Plugin Setup
```cpp
// PluginProcessor.h
#include <torch/script.h>

// Private members:
torch::jit::script::Module lstmModel;
std::vector<float> inBuffer;
std::vector<float> outBuffer;
void processBlockNN(torch::jit::script::Module& model,
                    std::vector<float>& inBlock,
                    std::vector<float>& outBlock,
                    int numSamples);
```

### processBlockNN Implementation (TorchScript)
```cpp
void AudioPluginAudioProcessor::processBlockNN(
    torch::jit::script::Module& model,
    std::vector<float>& inBlock,
    std::vector<float>& outBlock,
    int numSamples)
{
    // Convert input block to tensor
    torch::Tensor in_t = torch::from_blob(inBlock.data(),
        {static_cast<int64_t>(numSamples)});
    // Reshape to 3D: [batch=1, sequence_length, channels=1]
    in_t = in_t.View({1, -1, 1});
    // Prepare inputs
    std::vector<torch::jit::IValue> inputs;
    inputs.push_back(in_t);
    // Forward pass
    torch::jit::IValue out_ival = model.forward(inputs);
    // Extract output
    torch::Tensor out_t = out_ival.toTensor();
    out_t = out_t.View({-1});
    // Copy to output block
    float* data_ptr = out_t.data_ptr<float>();
    std::copy(data_ptr, data_ptr + inBlock.size(), outBlock.begin());
}
```

---

## 1.6 Training Pipeline (Ch 34-36)

### Training Script Components
1. `myk_data.py` — Data preparation functions
2. `myk_evaluate.py` — Testing trained networks
3. `myk_loss.py` — Loss function definitions
4. `myk_models.py` — Model definitions (SimpleLSTM)
5. `myk_train.py` — Train and update functions
6. `train.py` — Controller program and parameters

### Required Python Packages
```bash
pip install torch torchaudio scipy numpy tensorboard soundfile packaging
```

### Training Data Preparation
- **Input folder**: Clean guitar signal WAVs (the test signal)
- **Output folder**: Processed signal WAVs (recorded from amp)
- **Sequence length**: 0.5s default (22,050 samples at 44.1kHz) — how far back LSTM "sees"
- **Batch size**: 50 default — how many sequences per parameter update
- Audio concatenated → chopped into sub-sequences → TensorDataset

### Dataset Shape
```
(num_sequences, sequence_length, 1)  # for mono audio
```

### Data Capture Setup
- Play varied guitar through amp (1-15 minutes, per Neural Amp Modeller author Steve Atkinson)
- Record from amp line-out (preamp+tone modeling) or mic (full chain including cab)
- Clean input = training input, recorded output = training target

### Latency Consideration
- DAW automatically applies latency compensation based on audio buffer size
- If using external capture, may need to manually align clean/processed signals

### Model Architecture
```python
class SimpleLSTM(torch.nn.Module):
    def __init__(self, hidden_size=32):
        super().__init__()
        self.lstm = torch.nn.LSTM(1, hidden_size, 1)  # 1 input, N hidden, 1 layer
        self.dense = torch.nn.Linear(hidden_size, 1)    # mix down to 1 output
        # Internal state management
```

### Training Loop
```python
# Pseudocode
for epoch in range(num_epochs):
    for batch_input, batch_target in dataloader:
        output = model(batch_input)
        loss = loss_function(output, batch_target)
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()
    # Validate and save best model
```

### Loss Functions
- Standard MSE loss + additional spectral/perceptual losses
- Monitor via **TensorBoard** (`tensorboard --logdir runs`)
- Models saved as `.pth` files with loss in filename: `lstm_size_32_epoch_3_loss_0.6968.pth`

---

## 1.7 Model Export & Deployment (Ch 37)

### TorchScript Export
```python
import torch
import myk_models

saved_pth_path = 'lstm_size_32_epoch_3_loss_0.6968.pth'
export_pt_path = 'dist_32.ts'

model = torch.load(saved_pth_path)
model.eval()
scripted_model = torch.jit.script(model)  # NOT trace — script handles LSTM state logic
torch.jit.save(scripted_model, export_pt_path)
```

### Loading in JUCE Plugin
```cpp
// In PluginProcessor constructor:
std::string modelFolder{"C:\\temp\\models\\"};
std::string fp{modelFolder + "dist_32.ts"};
if (!std::filesystem::exists(fp)) {
    DBG("File " << fp << " not found");
    throw std::exception();
}
lstmModel = torch::jit::load(fp);
```

**Windows critical note**: Build mode (Release/Debug) MUST match libtorch version.

### Plugin processBlock Integration
```cpp
// prepareToPlay:
inBuffer.resize((size_t)samplesPerBlock);
outBuffer.resize((size_t)samplesPerBlock);

// processBlock:
for (int channel = 0; channel < totalNumInputChannels; ++channel) {
    auto* input = buffer.getReadPointer(channel);
    std::copy(input, input + inBuffer.size(), inBuffer.begin());
    processBlockNN(lstmModel, inBuffer, outBuffer, buffer.getNumSamples());
    auto* output = buffer.getWritePointer(channel);
    std::copy(outBuffer.begin(), outBuffer.begin() + inBuffer.size(), output);
}
```

---

## 1.8 RTNeural — Fast Inference Engine (Ch 38)

### What is RTNeural?
- C++ neural network inference library by **Jatin Chowdhury** (2021)
- **2-3x faster** than TorchScript for same model
- Implements LSTM, Dense, Conv layers in optimized C++
- Compiler can further optimize the visible C++ code
- **Inference only** — no training (train in PyTorch, export weights as JSON)

### Performance Comparison
| Framework | Speed | Training | Deployment |
|-----------|-------|----------|------------|
| PyTorch (Python) | Slow for inference | ✅ Full training | Development only |
| TorchScript | Baseline | ❌ Inference only | ✅ C++ plugins |
| **RTNeural** | **2-3x faster** | ❌ Inference only | ✅ **C++ plugins (preferred)** |
| ONNX | Variable | ❌ Inference only | ✅ Alternative option |

### Model Type Definition
```cpp
#include <RTNeural/RTNeural.h>

const int lstm_units = 32;  // or 64
using RTLSTMModel32 = RTNeural::ModelT<float, 1, 1,
    RTNeural::LSTMLayerT<float, 1, lstm_units>,
    RTNeural::DenseT<float, lstm_units, 1>>;
```

**Template breakdown**: `ModelT<float, 1_input, 1_output, Layer1, Layer2, ...>`

### Weight Export from Python
```python
# In myk_models.SimpleLSTM:
model.save_for_rtneural()  # Generates JSON file with layer weights
```

JSON format:
```json
{
  "lstm.weight_ih_l0": [[...], [...]],
  "lstm.weight_hh_l0": [[...], [...]],
  "lstm.bias_ih_l0": [...],
  "lstm.bias_hh_l0": [...],
  "dense.weight": [[...]],
  "dense.bias": [...]
}
```

### Weight Loading in C++
```cpp
RTLSTMModel32 model;

// Load JSON
std::ifstream jsonStream("lstm_weights.json", std::ifstream::binary);
nlohmann::json modelJson;
jsonStream >> modelJson;

// Load LSTM layer weights
auto& lstm = model.get<0>();
RTNeural::torch_helpers::loadLSTM<float>(modelJson, "lstm.", lstm);

// Load Dense layer weights
auto& dense = model.get<1>();
RTNeural::torch_helpers::loadDense<float>(modelJson, "dense.", dense);
```

### RTNeural JUCE Plugin — Complete Pattern
```cpp
// PluginProcessor.h:
#include <RTNeural/RTNeural.h>
using RTLSTMModel32 = RTNeural::ModelT<float, 1, 1,
    RTNeural::LSTMLayerT<float, 1, 32>,
    RTNeural::DenseT<float, 32, 1>>;
// Private member:
RTLSTMModel32 lstmModel;

// Constructor: load weights
setupModel(lstmModel, "path-to-json-file");

// Set mono:
.withInput("Input", juce::AudioChannelSet::mono(), ...)
.withOutput("Output", juce::AudioChannelSet::mono(), ...)

// processBlock:
for (int channel = 0; channel < totalNumInputChannels; ++channel) {
    auto* outData = buffer.getWritePointer(channel);
    auto* inData = buffer.getReadPointer(channel);
    for (auto i = 0; i < buffer.getNumSamples(); ++i) {
        outData[i] = lstmModel.forward(&inData[i]);  // Single sample!
    }
}
```

### Adding Tone Controls to Neural Effects
**Easy approach**: Add IIR filter before/after neural network (not emulated, but functional).

**Proper approach**: Train network with tone control as additional input channel:
1. Record test signal through amp at tone=0, tone=5, tone=10
2. Adjust training data: each sample = [audio_sample, tone_setting]
3. Adjust network: input_size=2 instead of 1
4. In plugin: pass [audio, knob_value] to network each sample

---

## 1.9 Other Neural Effects Architectures

### Mentioned in the Book
- **WaveNet**: Generative model for raw audio (van den Oord et al., 2016)
- **DDSP**: Differentiable Digital Signal Processing (Google Magenta)
- **CNN-based**: Non-linear FIR filter equivalent
- **ONNX Runtime**: Alternative inference engine
- **TensorFlow.js**: WebGL-accelerated inference for browser plugins
- **Neural Reverb**: Based on Steinmetz's work (experimental code in repo)

### Neural Amp Modeller (NAM) Reference
- Created by Steve Atkinson
- Similar LSTM-based approach
- Training data: 3-15 minutes of varied guitar playing
- Open-source, widely adopted in guitar community

---

# PART 2: YEKS GUIDE — Complete Amp Model Catalog

## Catalog Overview
**103+ unique amp models** covering all major manufacturers, documented with:
- Fractal Audio model name → Real amp equivalent
- Circuit type, power rating, tube complement
- Recommended speaker/cabinet pairings (IRs)
- Tone tips and settings recommendations

---

## 2.1 FENDER Circuits

| Circuit | Amp | Fractal Model | Era |
|---------|-----|---------------|-----|
| 5E3 | Tweed Deluxe | DELUXE TWEED | Tweed |
| 5F1 | Tweed Champ | 5F1 TWEED | Tweed |
| 5F2-A | Tweed Princeton | PRINCE TONE | Tweed |
| 5F6-A | '59 Tweed Bassman | 59 BASSGUY | Tweed |
| 5F8 | Tweed Twin (High Power) | 5F8 TWEED | Tweed |
| 6G4 | '60 Brown Super | 6G4 SUPER | Brown |
| 6G11 | Vibrolux 1963 | VIBRATO LUX | Brown |
| 6G12 | '60 Brown Concert | 6G12 CONCERT | Brown |
| AA763 | Tremolux / Princeton / Princeton Rev | TREMOLO LUX / PRINCE TONE NR / REV | Blackface |
| AB165 | '65 Bassman / '68 Bandmaster | 65 BASSGUY / BAND-COMMANDER | Blackface |
| AB763 | Deluxe Reverb / Super Reverb / Twin '66 | DELUXE VERB / SUPER VERB / DOUBLE VERB | Blackface |
| AC561 | Twin Reverb '71 | DOUBLE VERB SF | Silverface |

**Key Fender models for REVITHION** (most iconic clean/breakup tones):
- **59 BASSGUY** — "Holy grail" '59 Bassman, "grandfather of rock and roll". 50W, 6L6. Turn down bass, turn up mids.
- **DELUXE TWEED** (5E3) — Classic breakup tone, beloved recording amp. 15W, 6V6.
- **DELUXE VERB** — '65 Blackface Deluxe Reverb. THE studio clean/edge-of-breakup tone.
- **DOUBLE VERB** — Fender Twin Reverb. Pristine cleans, massive headroom.
- **PRINCE TONE REVERB** — Blackface Princeton Reverb. Compact, sweet breakup.
- **SUPER VERB** — '64 Super Reverb, AB763 circuit. Same family as Deluxe Reverb.

---

## 2.2 MARSHALL / British

| Fractal Model | Real Amp | Character |
|---------------|----------|-----------|
| **1959SLP** | Marshall SLP1959 (Plexi) | Classic Plexi rock/blues |
| **1987X** | Marshall 1987X | Plexi 50W re-issue |
| **BRIT 800** | Marshall JCM 800 (2204 50W) | Classic '80s crunch/lead |
| **BRIT 800 MOD** | Marshall JCM 800 modded | Higher gain JCM800 |
| **BRIT 800 #34** | Marshall JCM 800 (#34 mod) | José mod style |
| **BRIT AFS100** | Marshall AFD100 (Slash signature) | Appetite for Destruction tone |
| **BRIT JM45** | Marshall JTM 45 | Original Marshall, KT66/EL34 |
| **BRIT JVM** | Marshall JVM410H | Modern 4-channel |
| **BRIT PRE** | Marshall JMP-1 preamp | Rack preamp |
| **BRIT SILVER** | Marshall Silver Jubilee | '87 anniversary, popular for hard rock |
| **PLEXI 100W** | Marshall Plexi 100W | Multiple channel variants |
| **PLEXI 50W** | Marshall Plexi 50W | Various speaker/channel combos |

**Speaker pairings**: 4x12 Marshall cabinet with Celestion G12M (greenbacks) or G12H (Heritage).

---

## 2.3 VOX-type Amps (No Negative Feedback, EL84)

| Manufacturer | Amp | Fractal Model |
|-------------|-----|---------------|
| VOX | AC15 Top Boost | CLASS-A 15W TB |
| VOX | AC30 | CLASS-A 30W / BRT / HOT / TB |
| Orange | AD30HTC | CITRUS A30 CLN / DRTY |
| Matchless | DC-30 | MATCHBOX D-30 |
| Matchless | Chieftain | BOUTIQUE 1 / 2 |
| Morgan | AC20 Deluxe | AC-20 (12AX7 / EF86 variants) |
| Budda | Twinmaster | BUTTERY |
| Paul Ruby | Rocket | RUBY ROCKET |
| Trainwreck | Rocket | WRECKER ROCKET |

---

## 2.4 High-Gain / Modern

| Fractal Model | Real Amp | Notes |
|---------------|----------|-------|
| **5153** | EVH 5150-III | 3 channels: Green (clean), Blue (crunch), Red (lead) |
| **PVH 6160 BLOCK** | Block letter Peavey 5150/6505 | THE metal rhythm tone |
| **PVH 6160+** | Peavey 6160+/EVH 5150-II | Updated version |
| **RECTO1/RECTO2** | MESA Dual Rectifier | Two-channel and three-channel versions |
| **USA CLEAN/LEAD/RHYTHM** | MESA/Boogie Mark IV | Rhythm 1 (clean), Rhythm II (fat), Lead (fat/bright) |
| **USA IIC+** | MESA/Boogie Mark IIC+ | Legendary lead tone. Class-A, two power tubes |
| **USA IIC++** | Modded MESA Mark IIC+ | Even more gain |
| **EURO BLUE/RED** | Bogner Ecstasy (XTC) | 20th Anniversary. Structure/Old-New/Excursion switches |
| **EURO UBER** | Bogner Uberschall | 100W, EL34, single input. Extreme gain |
| **DAS METALL / DIZZY V4** | Diezel VH4 | Legendary German high-gain |
| **HERBIE** | Diezel Herbert | Three channels, different voicings |
| **ANGLE SEVERE** | Engl Savage 120 | Two 6550 tubes, 120W |
| **ENERGYBALL** | Engl Powerball | Channel 2 modeled |
| **SOLO 100** | Soldano SLO-100 | THE '80s/'90s high-gain reference |
| **FRIEDMAN BE/HBE** | Friedman BE-100 | Modern Plexi-derived high-gain |
| **DIRTY SHIRLEY** | Friedman Dirty Shirley | Marshall-inspired medium gain |
| **SPAWN NITROUS** | Splawn Nitro | KT88 option |
| **SPAWN QROD** | Splawn Quickrod | 100W, EL34 |

---

## 2.5 Boutique / D-type (Dumble-style)

| Fractal Model | Real Amp | Notes |
|---------------|----------|-------|
| **ODS-100** | Dumble Overdrive Special | Ultra-rare, legendary tone |
| **BLUDOJAI** | Bludotone Ojai | Dumble-inspired, G12-65 speakers |
| **CA OD-2** | Carol-Ann OD2 | Clean (ch.A) + overdrive channels |
| **CA TRIPTIK** | Carol-Ann Triptik | Three channels |
| **CA TUCANA** | Carol-Ann Tucana 3 | High-end boutique |
| **FOX ODS** | Fuchs Overdrive Supreme | Dumble-style, fully tube signal path |
| **TWO STONE J35** | Two-Rock Jet 35 | Treble (pull:Bright), Master (pull:Lead) |

---

## 2.6 Other Notable Amps

| Fractal Model | Real Amp | Character |
|---------------|----------|-----------|
| **JAZZ 120** | Roland Jazz Chorus 120 | Iconic clean, solid-state, no Master Volume |
| **SV BASS** | Ampeg SVT | THE bass amp. Massive rig |
| **USA BASS 400** | MESA Bass 400 | Bass tube amp |
| **HIPOWER** | Hiwatt DR103 | The Who, Pink Floyd. Paired with Fane speakers |
| **SUPREMO TREM** | Supro 1964T Dual-Tone | Jimmy Page, Link Wray |
| **WRECKER EXPRESS** | Trainwreck Express | Ultra-rare, no negative feedback |
| **WRECKER LIVERPOOL** | Trainwreck Liverpool | Express preamp + Rocket power amp fusion |
| **COMET 60** | Komet 60 | Hand-wired, G12H/G12M speakers |
| **CAMERON CCV** | Cameron CCV-100 | Channel 1 (Punch, Gain, Bright controls) |
| **ATOMICA** | Cameron Atomica | 100W, EL34, L/M/H gain |
| **NUCLEAR-TONE** | Swart Atomic Space Tone | Excellent breakup |
| **CAR ROAMER** | Carr Rambler | Boutique 1x12/2x12/2x10 |
| **TX STAR** | MESA Lone Star | 6L6, versatile clean-to-lead |

---

## 2.7 FAS Custom Models (Fractal Audio Originals)

| Model | Description |
|-------|-------------|
| FAS 6160 | FAS take on 5150 |
| FAS BASS | Custom bass amp |
| FAS BROOTALZ | Extreme high-gain |
| FAS BROWN | Custom "brown sound" |
| FAS CLASS-A | Custom class-A |
| FAS CRUNCH | Custom crunch |
| FAS HOT ROD | Custom hot rod |
| FAS LEAD 1 | Lead tone 1 |
| FAS LEAD 2 | Lead tone 2 |
| FAS MODERN / II / III | Modern high-gain variants |
| FAS RHYTHM | Custom rhythm |
| FAS WRECK | Custom Trainwreck-inspired |
| BIG HAIR | '80s style |
| SUPERTWEED | Custom tweed-inspired |

---

## 2.8 Preamps (Rack Units)

| Manufacturer | Preamp | Model |
|-------------|--------|-------|
| Bogner | Fish | BOGFISH BROWN / STRATO |
| Marshall | JMP-1 | BRIT PRE / JMPRE-1 OD1/OD2 |
| CAE | 3+ SE | CA3+ CLEAN/LEAD/RHY |
| Soldano | X88R | SOLO 88 CLEAN/LEAD/RHYTHM |
| Soldano | X99 | SOLO 99 CLEAN/LEAD |
| MESA/Boogie | TriAxis | USA PRE CLEAN/LD1/LD2 variants |

---

## 2.9 REVITHION Training Target Priority List

Based on cultural significance, genre coverage, and modeling demand:

### Tier 1 — Essential (Model First)
1. **MESA IIC+** — The lead tone. Metallica, Petrucci, Santana
2. **Soldano SLO-100** — '80s/'90s high-gain reference
3. **Marshall Plexi** — Classic rock foundation
4. **Fender Twin Reverb** — Clean standard
5. **Fender Deluxe Reverb** — Studio edge-of-breakup
6. **Marshall JCM 800** — '80s crunch/lead standard
7. **EVH 5150-III** — Modern metal standard
8. **MESA Dual Rectifier** — Nu-metal, modern heavy
9. **VOX AC30** — British clean/chime (Beatles, Queen, U2)
10. **Dumble ODS** — Holy grail boutique

### Tier 2 — Important
11. Bogner Ecstasy (XTC)
12. Friedman BE-100
13. Diezel VH4
14. Fender '59 Bassman
15. Marshall Silver Jubilee
16. Peavey 5150/6505
17. VOX AC15
18. Fender Princeton Reverb
19. Bogner Uberschall
20. MESA Mark IV

### Tier 3 — Genre-Specific / Niche
- Trainwreck Express/Liverpool/Rocket
- Hiwatt DR103
- Orange Rockerverb/Tiny Terror
- Matchless DC-30
- Engl Savage/Powerball
- Suhr Badger
- Carol-Ann OD2
- Two-Rock Jet 35

---

# PART 3: CROSS-REFERENCE — Books → REVITHION Architecture

## Complete Neural Amp Plugin Pipeline

```
[Training Phase - Python]
  1. Capture audio: clean_signal → real_amp → recorded_output
  2. Prepare dataset: input/output WAVs → TensorDataset (0.5s sequences, batch=50)
  3. Define model: SimpleLSTM(hidden_size=32 or 64) + Dense(hidden→1)
  4. Train: MSE + spectral loss, monitor via TensorBoard
  5. Export: .pth → torch.jit.script() → .ts (TorchScript)
     OR: .pth → save_for_rtneural() → .json (RTNeural weights)

[Deployment Phase - C++/JUCE]
  Option A: TorchScript (larger binary, slower, simpler weight loading)
    - torch::jit::load("model.ts")
    - Tensor reshaping: view({1, -1, 1}) for 3D batch input
    - Extract via toTuple()->elements()[0].toTensor()

  Option B: RTNeural (smaller, 2-3x faster, preferred for real-time)
    - Define: RTNeural::ModelT<float, 1, 1, LSTMLayerT, DenseT>
    - Load JSON weights per-layer via torch_helpers
    - Forward: lstmModel.forward(&inData[i]) — single sample!

[Plugin Integration]
  - PluginProcessor: mono I/O, load model in constructor
  - prepareToPlay: allocate buffers
  - processBlock: copy audio → forward through model → copy back
  - Tone control: IIR filter pre/post network, or train with control input
```

## Key Technical Parameters

| Parameter | Recommended Value | Notes |
|-----------|------------------|-------|
| LSTM hidden units | 32-64 | 32 = faster, 64 = more accurate |
| Sequence length | 0.5s (22,050 samples) | How far back LSTM "sees" |
| Batch size | 50 | Sequences per parameter update |
| Sample rate | 44,100 Hz | Standard audio |
| Training data | 3-15 minutes | Varied guitar playing |
| Loss function | MSE + spectral | Prevents "error-avoiding" solutions |
| Inference engine | RTNeural | 2-3x faster than TorchScript |

## Repository Structure (Yee-King's code)
Key project references from Chapter 39:
- **39.2.2**: CMake JUCE plugin starter
- **39.2.11**: JUCE + libtorch starter
- **39.5.16**: LSTM audio processing (Python)
- **39.5.19**: WAV through LSTM (C++)
- **39.5.21**: Block-based LSTM processing
- **39.5.23**: Python training program
- **39.5.24**: JUCE TorchScript LSTM plugin
- **39.5.25**: RTNeural command-line (export, validate, performance)
- **39.5.26**: JUCE RTNeural LSTM plugin
- **39.5.27**: Neural reverb (experimental)

**Book website**: www.yeeking.net/book

---

*Document generated for REVITHION STUDIO reference. Sources: Yee-King (2024) and Yek's Guide (Q7.02).*
