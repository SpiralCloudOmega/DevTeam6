# 🎯 Master Rules - Universal Agent Guidelines

> **Applies to ALL agents in the DevTeam6 ecosystem**
> *Forked from awesome-cursorrules pattern*

---

## 🧠 Core Principles

### 1. Internet-First Architecture
- **ALWAYS** search for real, up-to-date information before generating
- Prefer official documentation over cached knowledge
- Verify API endpoints and library versions are current
- Cross-reference multiple sources for accuracy

### 2. Code Quality Standards
- Write clean, readable, maintainable code
- Follow language-specific best practices
- Include meaningful comments for complex logic
- Use consistent naming conventions
- Prefer explicit over implicit

### 3. Security First
- Never expose secrets or credentials
- Validate all inputs
- Sanitize outputs
- Use secure defaults
- Follow principle of least privilege

### 4. Performance Awareness
- Consider computational complexity
- Optimize for the common case
- Profile before optimizing
- Cache expensive operations appropriately

---

## 📋 Response Guidelines

### Structure
1. **Acknowledge** - Confirm understanding of the request
2. **Plan** - Outline approach before implementing
3. **Execute** - Implement with clear explanations
4. **Verify** - Test and validate the solution
5. **Document** - Provide usage examples if applicable

### Format
- Use markdown for all responses
- Include code blocks with language specifiers
- Use tables for structured data
- Add diagrams (Mermaid) for complex flows

### Length
- Be concise but complete
- Avoid unnecessary verbosity
- Include all relevant details
- Omit obvious information

---

## 🔄 Handoff Protocol

### When to Hand Off
- Task requires different expertise
- Task blocked waiting for external input
- Task exceeds time allocation
- Error requires escalation

### Handoff Requirements
1. Current state summary
2. Work completed so far
3. Pending items list
4. Blockers identified
5. Recommended next steps

---

## 📝 Logging Requirements

### Always Log
- Task start and completion
- Errors and exceptions
- Decisions and rationale
- External API calls
- File modifications

### Log Format
```
[2025-12-05T20:00:00Z] [@agent-id] [INFO] [TASK-XXX] Message here
```

---

## 🚫 Prohibited Actions

1. **Never** commit secrets to repository
2. **Never** execute destructive operations without confirmation
3. **Never** bypass security checks
4. **Never** ignore error handling
5. **Never** make breaking changes without notice

---

## ✅ Required Actions

1. **Always** validate inputs
2. **Always** handle errors gracefully
3. **Always** write tests for new code
4. **Always** update documentation
5. **Always** follow the established patterns

---

## 🎨 Theme Compliance

All UI/visual elements must use the cyberpunk theme:
- Primary: Cyan `#00f0ff`
- Secondary: Magenta `#ff00ff`
- Accent: Green `#00ff88`
- Background: Dark `#0a0a1a`

---

*Version: 1.0.0*
*Last Updated: 2025-12-05*
