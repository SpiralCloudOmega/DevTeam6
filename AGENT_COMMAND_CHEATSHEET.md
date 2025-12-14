# 🤖 GitHub Copilot Agent Command Cheatsheet

> **Complete reference for GitHub Copilot agent commands and context limits - Feed this to any Copilot to help them understand their own command structure**

---

## 📋 Table of Contents

1. [All GitHub Copilot Commands](#all-github-copilot-commands)
2. [Core Agent Commands](#core-agent-commands)
3. [Context Window Limits](#context-window-limits)
4. [Quick Tips](#quick-tips)

---

## 🎯 All GitHub Copilot Commands

### Complete Command Structure

GitHub Copilot supports the following command patterns:

```bash
# Agent invocation pattern
@agent-name <prompt>

# Slash commands (in chat)
/explain <code or selection>
/fix <problem description>
/tests <code to test>
/help <topic>
/clear (clear conversation)

# File references
@workspace /path/to/file.ts: <prompt>
#file:/path/to/file.ts <prompt>

# Inline suggestions
# Just start typing and Copilot suggests completions

# Chat panel commands
Ctrl+I / Cmd+I (inline chat)
Ctrl+Shift+I / Cmd+Shift+I (chat panel)
```

### All Available Agents

| Agent | Command | Purpose | Scope | When to Use |
|-------|---------|---------|-------|-------------|
| **@workspace** | `@workspace <prompt>` | Multi-file operations, architecture | Entire repository | Code search, refactoring across files, project-wide questions |
| **@terminal** | `@terminal <command>` | CLI execution, shell operations | Terminal/shell | Running builds, tests, git commands, npm/pip installs |
| **@vscode** | `@vscode <action>` | IDE operations | VS Code API | Settings, extensions, keybindings, editor actions |
| **Custom agents** | `@agent-name <prompt>` | Specialized tasks | Defined in .github/agents/ | Domain-specific tasks (e.g., @react-agent, @python-agent) |

### Slash Commands Reference

| Command | Syntax | Purpose | Example |
|---------|--------|---------|---------|
| **/explain** | `/explain` | Explain selected code | `/explain how this function works` |
| **/fix** | `/fix` | Fix bugs or issues | `/fix the memory leak in this component` |
| **/tests** | `/tests` | Generate test cases | `/tests for the authentication module` |
| **/help** | `/help [topic]` | Get help on Copilot features | `/help agents` |
| **/clear** | `/clear` | Clear chat history | `/clear` |
| **/new** | `/new` | Start new project/file | `/new React component` |

---

## 🎯 Core Agent Commands

### Basic Syntax

```bash
@agent-name <your prompt here>
```

### Built-in GitHub Copilot Agents (Detailed)

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
| **GPT-5.2** | 200K tokens | ~180K tokens | ~720K characters |
| **Claude Opus 4.5** | 200K tokens | ~180K tokens | ~720K characters |
| **Claude Sonnet 4.5** | 200K tokens | ~180K tokens | ~720K characters |
| **Gemini 3 Thinking** | 2M tokens | ~1.8M tokens | ~7.2M characters |

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

## 💡 Quick Tips & Prompting Guide

### Understanding Copilot's Architecture

**GitHub Copilot consists of:**
1. **Inline Suggestions** - Code completions as you type
2. **Chat Interface** - Conversational coding assistant
3. **Agent System** - Specialized agents for different scopes (@workspace, @terminal, @vscode)
4. **Custom Agents** - Repository-specific agents defined in .github/agents/
5. **Slash Commands** - Quick actions (/explain, /fix, /tests)

### Effective Prompts (Feed Format)

```bash
# ❌ Bad: Vague request
@workspace Fix the code

# ✅ Good: Specific with context
@workspace Refactor /app/auth.ts to use async/await and add TypeScript types

# ❌ Bad: No scope
Explain the database code

# ✅ Good: With file reference
@workspace Explain the database connection pooling in /db/connection.ts

# ❌ Bad: Too broad
@workspace Review everything

# ✅ Good: Focused scope
@workspace Review authentication logic in /auth/ directory for security issues
```

### Managing Large Contexts

```bash
# IMPORTANT: Context limits are PER REQUEST, not per session
# Each new message to Copilot starts with limited context window

# Strategy 1: Break tasks into chunks
@workspace Analyze the /api directory structure
# Then in NEW request:
@workspace Create a new endpoint for /api/users following the existing pattern

# Strategy 2: Explicit file references
@workspace Focus only on these files:
- /app/auth.ts
- /app/session.ts
Refactor to use JWT tokens

# Strategy 3: Progressive refinement
# Request 1: High-level
@workspace What's the best approach for caching?
# Request 2: Implementation
@workspace Implement Redis caching for user sessions
# Request 3: Details
@workspace Add error handling and retry logic to Redis cache
```

### Best Practices (Copilot Command Structure)

**DO:**
- Use `@workspace` for multi-file or repo-wide operations
- Use `@terminal` for running commands, builds, tests
- Use `@vscode` for IDE-specific actions
- Reference files explicitly: `/path/to/file.ts`
- Break tasks into digestible chunks (< 50K tokens per request)
- Specify your tech stack: "Using React 19, TypeScript, Tailwind"
- Use slash commands for quick actions: `/explain`, `/fix`, `/tests`

**DON'T:**
- Mix agents (don't ask @terminal for code explanations)
- Assume infinite context (each request has token limits)
- Be vague ("fix it", "make it better")
- Expect memory across separate chat sessions
- Overload a single request with too many tasks
- Forget that Copilot can't see your screen (describe what you see)

### Context Window Best Practices

**For GPT-5.2, Claude Opus/Sonnet 4.5 (~180K tokens):**
- Can handle: 20-30 medium files, 1 large module, full feature analysis
- Break up: Entire large codebases, multi-module refactors

**For Gemini 3 Thinking (~1.8M tokens):**
- Can handle: Entire small-to-medium repos, comprehensive architecture analysis
- Break up: Very large monorepos, enterprise-scale codebases



---

## 📋 Copilot System Prompt Feed

**Copy this section to feed to another Copilot to teach it about its own command structure:**

```
SYSTEM: GitHub Copilot Command Structure & Context Limits

AVAILABLE COMMANDS:
- @workspace <prompt> - Multi-file operations, entire repository scope
- @terminal <command> - Execute shell commands, run builds/tests
- @vscode <action> - IDE operations, settings, extensions
- Custom: @agent-name <prompt> - Repository-specific agents in .github/agents/

SLASH COMMANDS:
- /explain - Explain code
- /fix - Fix bugs or issues
- /tests - Generate tests
- /help - Get help
- /clear - Clear chat history
- /new - Create new files/components

CONTEXT LIMITS PER REQUEST:
- GPT-5.2: 180K tokens (~720K chars) - Use for comprehensive multi-file analysis
- Claude Opus 4.5: 180K tokens (~720K chars) - Use for complex reasoning tasks
- Claude Sonnet 4.5: 180K tokens (~720K chars) - Use for balanced speed/capacity
- Gemini 3 Thinking: 1.8M tokens (~7.2M chars) - Use for entire codebase analysis

TOKEN CONVERSION:
1 token ≈ 4 characters ≈ 0.75 words
100 lines code ≈ 500-1000 tokens
Single file ≈ 200-5000 tokens
Module/directory ≈ 10K-50K tokens

IMPORTANT CONSTRAINTS:
- Context limit is PER REQUEST, not per session
- Each new message starts fresh with limited context window
- Agent cannot remember previous chat sessions
- Agent cannot see your screen - must describe visual issues
- Break large tasks into chunks to stay within token limits
- Always specify file paths explicitly
- Declare tech stack constraints upfront

PROMPTING BEST PRACTICES:
✓ Specific: "@workspace Refactor /app/auth.ts to use async/await"
✗ Vague: "@workspace Fix the code"
✓ Scoped: "@workspace Review /auth/ directory for security"
✗ Broad: "@workspace Review everything"
✓ Sequential: Break into multiple focused requests
✗ Overloaded: Put 10 tasks in one request

AGENT SELECTION:
- Use @workspace for: code search, multi-file refactoring, architecture questions
- Use @terminal for: npm install, git commands, running tests, building
- Use @vscode for: changing settings, installing extensions, editor actions
- Use custom agents for: domain-specific tasks (React, Python, DevOps, etc.)
```

---

## 📖 Resources

- [GitHub Copilot Docs](https://docs.github.com/en/copilot)
- [Custom Agents Guide](https://gh.io/customagents/config)
- [DevTeam6 Context7 System](./context7/agents.md)

---

<div align="center">

**Made with 💙 by DevTeam6**

*v2.0 - Comprehensive Feed Format | Updated: 2025-12-14*

</div>
