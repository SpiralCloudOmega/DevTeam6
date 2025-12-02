import { useState } from 'react'

interface Step {
  id: number
  title: string
  description: string
  icon: string
  action: string
  code?: string
  link?: string
  completed: boolean
}

const initialSteps: Step[] = [
  {
    id: 1,
    title: 'Star the Repository',
    description: 'Show your support by starring DevTeam6 on GitHub!',
    icon: '⭐',
    action: 'Star on GitHub',
    link: 'https://github.com/SpiralCloudOmega/DevTeam6',
    completed: false,
  },
  {
    id: 2,
    title: 'Fork the Repository',
    description: 'Create your own copy to start contributing.',
    icon: '🍴',
    action: 'Fork Repository',
    link: 'https://github.com/SpiralCloudOmega/DevTeam6/fork',
    completed: false,
  },
  {
    id: 3,
    title: 'Clone Your Fork',
    description: 'Clone the repository to your local machine.',
    icon: '📥',
    action: 'Copy Command',
    code: 'git clone https://github.com/{YOUR_GITHUB_USERNAME}/DevTeam6.git\ncd DevTeam6',
    completed: false,
  },
  {
    id: 4,
    title: 'Install Dependencies',
    description: 'Install the required packages for the 3D demo.',
    icon: '📦',
    action: 'Copy Command',
    code: 'cd app\nnpm install',
    completed: false,
  },
  {
    id: 5,
    title: 'Run Development Server',
    description: 'Start the local development server.',
    icon: '🚀',
    action: 'Copy Command',
    code: 'npm run dev',
    completed: false,
  },
  {
    id: 6,
    title: 'Make Your Changes',
    description: 'Edit files and add your improvements!',
    icon: '✏️',
    action: 'View Guidelines',
    link: 'https://github.com/SpiralCloudOmega/DevTeam6#contributing',
    completed: false,
  },
  {
    id: 7,
    title: 'Create a Branch',
    description: 'Create a feature branch for your changes.',
    icon: '🌿',
    action: 'Copy Command',
    code: 'git checkout -b feature/your-awesome-feature',
    completed: false,
  },
  {
    id: 8,
    title: 'Commit Your Changes',
    description: 'Commit with a descriptive message.',
    icon: '💾',
    action: 'Copy Command',
    code: 'git add .\ngit commit -m "feat: add your awesome feature"',
    completed: false,
  },
  {
    id: 9,
    title: 'Push to GitHub',
    description: 'Push your branch to your fork.',
    icon: '📤',
    action: 'Copy Command',
    code: 'git push origin feature/your-awesome-feature',
    completed: false,
  },
  {
    id: 10,
    title: 'Create Pull Request',
    description: 'Open a PR to merge your changes!',
    icon: '🔀',
    action: 'Create PR',
    link: 'https://github.com/SpiralCloudOmega/DevTeam6/compare',
    completed: false,
  },
]

function OnboardingWizard() {
  const [steps, setSteps] = useState<Step[]>(initialSteps)
  const [currentStep, setCurrentStep] = useState(0)
  const [copiedIndex, setCopiedIndex] = useState<number | null>(null)

  const completedCount = steps.filter(s => s.completed).length
  const progress = (completedCount / steps.length) * 100

  const toggleStep = (stepId: number) => {
    setSteps(prev => prev.map(step => 
      step.id === stepId ? { ...step, completed: !step.completed } : step
    ))
  }

  const copyToClipboard = async (code: string, index: number) => {
    try {
      if (!navigator.clipboard) {
        // Fallback for older browsers or HTTP contexts
        const textArea = document.createElement('textarea')
        textArea.value = code
        document.body.appendChild(textArea)
        textArea.select()
        document.execCommand('copy')
        document.body.removeChild(textArea)
        setCopiedIndex(index)
        setTimeout(() => setCopiedIndex(null), 2000)
        return
      }
      await navigator.clipboard.writeText(code)
      setCopiedIndex(index)
      setTimeout(() => setCopiedIndex(null), 2000)
    } catch (err) {
      console.error('Failed to copy:', err)
      // Show error feedback to user
      alert('Failed to copy to clipboard. Please copy the command manually.')
    }
  }

  const goToStep = (index: number) => {
    setCurrentStep(index)
  }

  const nextStep = () => {
    if (currentStep < steps.length - 1) {
      setCurrentStep(currentStep + 1)
    }
  }

  const prevStep = () => {
    if (currentStep > 0) {
      setCurrentStep(currentStep - 1)
    }
  }

  return (
    <div className="onboarding-wizard">
      <header className="wizard-header">
        <h1>🚀 Contributor Onboarding</h1>
        <p>Complete these steps to become a DevTeam6 contributor!</p>
      </header>

      <div className="progress-section">
        <div className="progress-bar">
          <div 
            className="progress-fill" 
            style={{ width: `${progress}%` }}
          />
        </div>
        <div className="progress-text">
          <span>{completedCount} of {steps.length} steps completed</span>
          <span className="percentage">{Math.round(progress)}%</span>
        </div>
      </div>

      <div className="wizard-content">
        <nav className="step-navigation">
          {steps.map((step, index) => (
            <button
              key={step.id}
              className={`step-nav-item ${index === currentStep ? 'active' : ''} ${step.completed ? 'completed' : ''}`}
              onClick={() => goToStep(index)}
            >
              <span className="step-icon">{step.completed ? '✅' : step.icon}</span>
              <span className="step-number">Step {index + 1}</span>
            </button>
          ))}
        </nav>

        <main className="step-content">
          <div className="step-card">
            <div className="step-header">
              <span className="step-icon-large">{steps[currentStep].icon}</span>
              <div className="step-info">
                <h2>Step {currentStep + 1}: {steps[currentStep].title}</h2>
                <p>{steps[currentStep].description}</p>
              </div>
            </div>

            <div className="step-body">
              {steps[currentStep].code && (
                <div className="code-block">
                  <pre>{steps[currentStep].code}</pre>
                  <button 
                    className="copy-btn"
                    onClick={() => copyToClipboard(steps[currentStep].code!, currentStep)}
                  >
                    {copiedIndex === currentStep ? '✅ Copied!' : '📋 Copy'}
                  </button>
                </div>
              )}

              {steps[currentStep].link && (
                <a 
                  href={steps[currentStep].link} 
                  target="_blank" 
                  rel="noopener noreferrer"
                  className="action-link"
                >
                  {steps[currentStep].action} →
                </a>
              )}
            </div>

            <div className="step-actions">
              <label className="completion-checkbox">
                <input
                  type="checkbox"
                  checked={steps[currentStep].completed}
                  onChange={() => toggleStep(steps[currentStep].id)}
                />
                <span>Mark as completed</span>
              </label>
            </div>
          </div>

          <div className="navigation-buttons">
            <button 
              className="nav-btn prev"
              onClick={prevStep}
              disabled={currentStep === 0}
            >
              ← Previous
            </button>
            <button 
              className="nav-btn next"
              onClick={nextStep}
              disabled={currentStep === steps.length - 1}
            >
              Next →
            </button>
          </div>
        </main>
      </div>

      {completedCount === steps.length && (
        <div className="completion-celebration">
          <h2>🎉 Congratulations!</h2>
          <p>You've completed all onboarding steps! Welcome to DevTeam6!</p>
          <div className="celebration-badges">
            <span className="badge">🏆 Onboarding Complete</span>
            <span className="badge">⭐ New Contributor</span>
            <span className="badge">🚀 Ready to Code</span>
          </div>
        </div>
      )}

      <footer className="wizard-footer">
        <a href="/DevTeam6/" className="back-link">← Back to 3D Demo</a>
        <a href="/DevTeam6/gamification" className="next-link">View Leaderboard →</a>
      </footer>
    </div>
  )
}

export default OnboardingWizard
