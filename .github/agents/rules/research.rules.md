# 📚 Research Specialist Rules

> **@research agent guidelines for Documentation/Research/Knowledge Management**

---

## 🎯 Expertise Areas

- Technical Documentation
- Research & Analysis
- Knowledge Base Management
- API Documentation
- README & Guides
- Change Logs & Release Notes
- Information Architecture

---

## 📋 Documentation Standards

### README Structure
```markdown
# Project Name

> Brief tagline/description

![Status Badge](badge-url)
![License](license-badge)

## 🚀 Quick Start

\`\`\`bash
npm install
npm run dev
\`\`\`

## 📖 Table of Contents

- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [API Reference](#api-reference)
- [Contributing](#contributing)
- [License](#license)

## ✨ Features

- Feature 1
- Feature 2
- Feature 3

## 📦 Installation

### Prerequisites
- Node.js 20+
- npm 9+

### Steps
1. Clone the repository
2. Install dependencies
3. Configure environment

## 🔧 Usage

### Basic Example
\`\`\`typescript
import { feature } from 'package'

const result = feature()
\`\`\`

## 📚 API Reference

See [API Documentation](./docs/api.md)

## 🤝 Contributing

See [CONTRIBUTING.md](./CONTRIBUTING.md)

## 📄 License

MIT License - see [LICENSE](./LICENSE)
```

### API Documentation Pattern
```markdown
## `functionName(params)`

Brief description of what the function does.

### Parameters

| Name | Type | Required | Description |
|------|------|----------|-------------|
| `param1` | `string` | Yes | Description |
| `param2` | `number` | No | Description (default: 10) |

### Returns

`ReturnType` - Description of return value

### Throws

- `ErrorType` - When condition occurs

### Example

\`\`\`typescript
const result = functionName('value', 42)
// => expected output
\`\`\`

### Notes

- Important note 1
- Important note 2
```

---

## 🔍 Research Methodology

### Information Gathering
1. **Primary Sources**
   - Official documentation
   - Source code
   - API specifications
   - Author publications

2. **Secondary Sources**
   - Technical blogs
   - Stack Overflow
   - GitHub issues
   - Community forums

3. **Verification**
   - Cross-reference multiple sources
   - Test claims when possible
   - Note version/date of information
   - Flag uncertainties

### Source Quality Assessment
| Quality | Indicators |
|---------|------------|
| High | Official docs, peer-reviewed, recent |
| Medium | Reputable blogs, active repos |
| Low | Outdated, unverified, anecdotal |

---

## 📝 Writing Guidelines

### Tone & Style
- Clear and concise
- Active voice preferred
- Technical but accessible
- Consistent terminology
- Inclusive language

### Formatting Rules
| Element | Convention |
|---------|------------|
| Headings | Title Case |
| Code | Backticks for inline |
| Commands | Code blocks with language |
| Notes | Blockquotes or callouts |
| Lists | Consistent punctuation |

### Visual Elements
- Use diagrams for complex flows
- Include code examples
- Add screenshots for UI
- Create tables for comparisons

---

## 🗂️ Knowledge Management

### Information Architecture
```
docs/
├── getting-started/
│   ├── installation.md
│   ├── quick-start.md
│   └── configuration.md
├── guides/
│   ├── basic-usage.md
│   ├── advanced-features.md
│   └── best-practices.md
├── api/
│   ├── overview.md
│   ├── endpoints.md
│   └── types.md
├── reference/
│   ├── glossary.md
│   ├── faq.md
│   └── troubleshooting.md
└── contributing/
    ├── code-style.md
    ├── pull-requests.md
    └── testing.md
```

### Linking Strategy
- Use relative links for internal docs
- Include anchors for sections
- Maintain a sitemap
- Cross-reference related topics

---

## 📊 Change Documentation

### Changelog Format
```markdown
# Changelog

All notable changes documented here.
Format based on [Keep a Changelog](https://keepachangelog.com/).

## [Unreleased]

### Added
- New feature description

### Changed
- Modification description

### Deprecated
- Soon-to-be-removed features

### Removed
- Removed features

### Fixed
- Bug fix description

### Security
- Security improvement

## [1.0.0] - 2025-12-05

### Added
- Initial release
```

### Commit Message Format
```
type(scope): description

[optional body]

[optional footer]
```

Types: `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`

---

## 🔄 Review Process

### Documentation Review Checklist
- [ ] Accurate information
- [ ] Clear structure
- [ ] Proper formatting
- [ ] Working links
- [ ] Code examples tested
- [ ] Grammar & spelling
- [ ] Version consistency
- [ ] Accessibility

---

## 🚫 Anti-Patterns to Avoid

1. **Don't** assume prior knowledge
2. **Don't** use jargon without explanation
3. **Don't** skip examples
4. **Don't** leave broken links
5. **Don't** neglect updates

---

*Version: 1.0.0*
*Last Updated: 2025-12-05*
