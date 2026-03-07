---
name: skill-design-patterns
description: Universal guide for designing effective AI agent skills — structure, examples, cross-referencing, composition, and testing.
tags: [meta, agent-skills, design-patterns, documentation]
version: "1.0"
depends-on: []
---

# Skill Design Patterns

A well-designed skill improves agent output. A poorly designed one wastes context tokens.

## 1 — YAML Frontmatter Best Practices

Every skill starts with YAML frontmatter. Required: `name`, `description`. Optional: `tags`, `version`, `depends-on`.

```yaml
---
name: my-new-skill            # kebab-case, matches directory name
description: Short imperative sentence under 200 chars explaining what the skill teaches.
tags: [cpp, audio, dsp]       # lowercase for agent discovery
version: "1.0"                # quoted to avoid YAML float parsing
depends-on:
  - ../cmake-fundamentals/SKILL.md
---
```

## 2 — Section Structure

Follow this order so agents can predict where information lives: **Concepts** (why) → **Code Examples** (how) → **Patterns** (when) → **Anti-Patterns** (avoid) → **Checklist** (verify).

```markdown
## Concepts
Why this technique exists and when it matters.
## Code Examples
\`\`\`python
def greet(name: str) -> str:
    return f"Hello, {name}"
\`\`\`
## Anti-Patterns
- Never do Z because it causes W.
## Checklist
- [ ] Item validated
```

## 3 — Code Example Density

Target one code example every 20–30 lines. Examples must be runnable. **Good:**
```cpp
#include <vector>
#include <algorithm>
std::vector<int> sorted(std::vector<int> v) {
    std::sort(v.begin(), v.end());
    return v;
}
```

**Bad:**
```text
sort the vector somehow
v.sort()  // not real C++
```

Always tag the language on the opening fence.

## 4 — Language-Specific Fence Tags

```python
import numpy as np
signal = np.sin(np.linspace(0, 2 * np.pi, 44100))
```
```cmake
find_package(JUCE REQUIRED)
target_link_libraries(MyPlugin PRIVATE juce::juce_audio_processors)
```
```sql
SELECT skill_name, load_count FROM skill_usage
WHERE load_count > 10 ORDER BY load_count DESC;
```

Never use a bare ` ``` ` without a language tag. Use `text` for plain text.

## 5 — Cross-Referencing Skills

Link related skills with relative paths so agents can build dependency graphs:

```markdown
**See also:**
- [SIMD Optimization](../simd-optimization/SKILL.md) — vectorized DSP loops
- [CMake Fundamentals](../cmake-fundamentals/SKILL.md) — build system setup
```

Always use relative paths. Add a dash-separated gloss after each link.

## 6 — Skill Composition

Layer skills basic → advanced. Declare prerequisites in `depends-on`, import by reference:

```yaml
depends-on:
  - ../basic-dsp/SKILL.md          # level 1
  - ../intermediate-dsp/SKILL.md   # level 2
```
```cpp
// Builds on BiquadFilter from basic-dsp prerequisite
#include "BiquadFilter.h"
std::array<BiquadFilter, 8> bank;
for (int i = 0; i < 8; ++i)
    bank[i].setFrequency(100.0 * std::pow(2.0, i));
```

## 7 — Skill Testing

Validate that a skill improves agent output with A/B measurement:

```python
results_a = run_agent(task="implement biquad filter", skills=[])
results_b = run_agent(task="implement biquad filter", skills=["basic-dsp"])
score_a, score_b = evaluate(results_a), evaluate(results_b)
assert score_b > score_a, "Skill did not improve output"
```

Run identical tasks with and without the skill. Measure compilation success, test pass rate, or review score.

## Anti-Patterns

| Anti-Pattern | Why It Fails |
|---|---|
| Wall of text, no code | Agents lose signal; code is higher-density information |
| Vague descriptions ("do it well") | Agents need concrete, actionable instructions |
| Over 300 lines | Exceeds context budget — split into layers |
| Under 30 lines | Too shallow to change agent behavior |
| Missing anti-patterns section | Agents repeat mistakes without "don't" guidance |
| No checklist | No verification that the skill was applied |
| Absolute paths in cross-refs | Breaks on different machines |
| Untagged code fences | Agents cannot validate syntax |

```markdown
<!-- BAD: wall of text -->
Do the thing correctly. Use best practices. Avoid mistakes.
<!-- GOOD: structured with code -->
Biquad filters are second-order IIR filters for audio EQ.
\`\`\`cpp
BiquadFilter eq;
eq.setType(BiquadType::Peak);
eq.setFrequency(1000.0);
\`\`\`
```

## Checklist

- [ ] Frontmatter is valid YAML (`python -c "import yaml; yaml.safe_load(open('SKILL.md'))"`)
- [ ] `name` is kebab-case matching directory name
- [ ] `description` is under 200 characters
- [ ] Every section has at least one code example
- [ ] Code fences have language tags (`cpp`, `python`, `cmake`, `yaml`, `sql`)
- [ ] Anti-patterns section present with concrete examples
- [ ] Checklist section present as final section
- [ ] Cross-references use relative paths only
- [ ] File is under 200 lines total
- [ ] Skill tested or reviewed against a real agent task
