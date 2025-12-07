import React from 'react'
import ReactDOM from 'react-dom/client'
import { BrowserRouter, Routes, Route, Navigate } from 'react-router-dom'
import App from './App'
import GamificationDashboard from './pages/GamificationDashboard'
import OnboardingWizard from './pages/OnboardingWizard'
import GenUIPlayground from './pages/GenUIPlayground'
import ImmersiveLanding from './pages/ImmersiveLanding'
import ProjectRoadmap from './pages/ProjectRoadmap'
import NodeGraphEditor from './pages/NodeGraphEditor'
import VideoStorytelling from './pages/VideoStorytelling'
import SemanticKnowledgeHub from './pages/SemanticKnowledgeHub'
import RepoPilot from './pages/RepoPilot'
import Repomind from './pages/Repomind'
import ControlDeck from './pages/ControlDeck'
import AIPlayground from './pages/AIPlayground'
import CodeWorkstation from './pages/CodeWorkstation'
import Dashboard from './pages/Dashboard'
import './index.css'

// Handle SPA redirect from 404.html
// This checks if we were redirected from the 404 page and navigates to the correct route
function getSpaRedirectPath(): string | null {
  const redirectPath = sessionStorage.getItem('spa-redirect-path')
  if (redirectPath) {
    sessionStorage.removeItem('spa-redirect-path')
    return redirectPath
  }
  return null
}

// Component that handles the redirect
function SpaRedirect() {
  const redirectPath = getSpaRedirectPath()
  if (redirectPath && redirectPath !== '/') {
    return <Navigate to={redirectPath} replace />
  }
  return <Dashboard />
}

ReactDOM.createRoot(document.getElementById('root')!).render(
  <React.StrictMode>
    <BrowserRouter basename="/DevTeam6">
      <Routes>
        <Route path="/" element={<SpaRedirect />} />
        <Route path="/gamification" element={<GamificationDashboard />} />
        <Route path="/onboarding" element={<OnboardingWizard />} />
        <Route path="/genui" element={<GenUIPlayground />} />
        <Route path="/landing" element={<ImmersiveLanding />} />
        <Route path="/roadmap" element={<ProjectRoadmap />} />
        <Route path="/node-graph" element={<NodeGraphEditor />} />
        <Route path="/videos" element={<VideoStorytelling />} />
        <Route path="/knowledge-hub" element={<SemanticKnowledgeHub />} />
        <Route path="/repopilot" element={<RepoPilot />} />
        <Route path="/repomind" element={<Repomind />} />
        <Route path="/control-deck" element={<ControlDeck />} />
        <Route path="/ai" element={<AIPlayground />} />
        <Route path="/code" element={<CodeWorkstation />} />
      </Routes>
    </BrowserRouter>
  </React.StrictMode>,
)
