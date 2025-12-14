# 🤖 GitHub Copilot Agent Command Cheatsheet

> **Quick reference for GitHub Copilot agent commands and context limits**

---

## 📋 Table of Contents

1. [Core Agent Commands](#core-agent-commands)
2. [Context Window Limits](#context-window-limits)
3. [Quick Tips](#quick-tips)

---

## 🎯 Core Agent Commands

### Basic Syntax

```bash
@agent-name <your prompt here>
```

### Built-in GitHub Copilot Agents

| Agent | Command | Purpose | Context Access |
|-------|---------|---------|----------------|
| **Workspace** | `@workspace` | Repository-wide operations, multi-file changes | Entire repo |
| **Terminal** | `@terminal` | Execute CLI commands, run scripts | Shell access |
| **VSCode** | `@vscode` | IDE operations, settings, extensions | VS Code API |

### Examples

```bash
# Workspace agent
@workspace Explain the project structure
@workspace Find all React components using hooks
@workspace Refactor the auth module to use async/await

# Terminal agent  
@terminal npm install
@terminal Run the test suite
@terminal git status

# VSCode agent
@vscode Open settings
@vscode Install the Prettier extension
@vscode Show all keybindings
```

### Custom Repository Agents

```bash
# List available custom agents
ls -la .github/agents/

# Invoke custom agent (example)
@your-agent-name <prompt>
```

---

## 🧠 Context Window Limits

### Context Limits Per Request

| Model/Agent | Context Window | Usable Context | Tokens/Request |
|-------------|----------------|----------------|----------------|
| **GPT-4** | 128K tokens | ~100K tokens | ~400K characters |
| **GPT-4 Turbo** | 128K tokens | ~100K tokens | ~400K characters |
| **Claude 3.5 Sonnet** | 200K tokens | ~180K tokens | ~720K characters |
| **Claude 3 Opus** | 200K tokens | ~180K tokens | ~720K characters |
| **GPT-3.5 Turbo** | 16K tokens | ~12K tokens | ~48K characters |

### Token Estimation

```
Rough Conversion:
• 1 token ≈ 4 characters
• 1 token ≈ 0.75 words
• 100 lines of code ≈ 500-1000 tokens
• 1 page of documentation ≈ 500-800 tokens
```

### Context Usage Examples

```
Small Request:    < 1K tokens     ~4K characters      Single file edit
Medium Request:   1K - 10K tokens  ~4K - 40K chars    Multi-file changes
Large Request:    10K - 50K tokens ~40K - 200K chars  Feature implementation
Huge Request:     50K - 100K tokens ~200K - 400K chars Architecture changes
Too Large:        > 100K tokens    > 400K chars       Split into parts
```

### Request Size Guidelines

| Content Type | Approximate Tokens | Fits in Context |
|--------------|-------------------|-----------------|
| Single function | 50-200 tokens | ✅ Always |
| One file (small) | 200-500 tokens | ✅ Always |
| One file (large) | 1K-5K tokens | ✅ Usually |
| Multiple files (5-10) | 5K-20K tokens | ✅ Yes |
| Module/directory | 10K-50K tokens | ⚠️ Depends on model |
| Entire small repo | 50K-100K tokens | ⚠️ Large models only |
| Entire large repo | 100K+ tokens | ❌ Will truncate |

### Truncation Warning Signs

```
⚠️ Your context is truncated when:
• Agent asks questions you already answered
• Agent loses track of previous conversation
• Responses become generic or incomplete
• Agent says "I don't have enough context"
```

---

## 💡 Quick Tips

### Effective Prompts

```bash
# ❌ Bad: Vague request
@workspace Fix the code

# ✅ Good: Specific with context
@workspace Refactor /app/auth.ts to use async/await and add TypeScript types
```

### Managing Large Contexts

```bash
# Break large tasks into chunks
@workspace Analyze the /api directory structure

# Then in a new request
@workspace Create a new endpoint for /api/users following the existing pattern
```

### Best Practices

**DO:**
- Be specific about files and requirements
- Break large tasks into smaller requests  
- Reference exact file paths
- Specify your tech stack constraints

**DON'T:**
- Assume unlimited context memory
- Mix multiple unrelated tasks
- Be vague about what you want
- Expect the agent to remember earlier sessions



---

## 📖 Resources

- [GitHub Copilot Docs](https://docs.github.com/en/copilot)
- [Custom Agents Guide](https://gh.io/customagents/config)
- [DevTeam6 Context7 System](./context7/agents.md)

---

<div align="center">

**Made with 💙 by DevTeam6**

*Quick reference v1.0 | Last updated: 2025-12-14*

</div>
