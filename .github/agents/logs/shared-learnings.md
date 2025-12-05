# 🧠 Shared Learnings

> **Cross-Agent Knowledge Repository**
> *Insights that benefit all agents*

---

## Learning Categories

- 🔧 **Technical** - Code patterns, tools, techniques
- 📋 **Process** - Workflows, procedures, protocols
- ⚠️ **Pitfalls** - Common mistakes to avoid
- 💡 **Insights** - General observations

---

## Recent Learnings

### 2025-12-05

#### 🔧 Technical: Edge Case Handling in Utilities

**Source:** @react (TASK-001)  
**Learning:** Always handle empty/null inputs at the start of utility functions.

```typescript
// Pattern: Early return for edge cases
export function filterNodesByCluster(nodes: GraphNode[], clusterId: string): GraphNode[] {
  if (!nodes || nodes.length === 0) return []
  if (!clusterId || clusterId === 'all') return nodes
  return nodes.filter(node => node.cluster === clusterId)
}
```

**Impact:** Prevents runtime errors and improves robustness.

---

#### 📋 Process: Test Coverage Standards

**Source:** @react (TASK-002)  
**Learning:** Aim for 80%+ test coverage with focus on:
- Edge cases (empty inputs, boundary values)
- Error conditions
- Integration points

**Metrics:**
- Before: 28 tests
- After: 44 tests (+57%)

---

#### 💡 Insight: Documentation Comments

**Source:** Code review feedback  
**Learning:** JSDoc comments should accurately describe behavior, not implementation details.

```typescript
// ❌ Misleading
* Returns empty array if start node has no outgoing edges

// ✅ Accurate
* Returns empty array if start node does not exist in the graph
```

---

## Knowledge Index

| Topic | Learning | Date | Source |
|-------|----------|------|--------|
| Edge Cases | Early return pattern | 2025-12-05 | @react |
| Testing | Coverage standards | 2025-12-05 | @react |
| Documentation | Accurate comments | 2025-12-05 | Review |

---

## Contributing

When adding learnings:
1. Include source agent and task
2. Provide code example if applicable
3. Explain the impact
4. Add to the knowledge index

---

*Last Updated: 2025-12-05*
