import React from 'react'
import ReactDOM from 'react-dom/client'
import { BrowserRouter, Routes, Route } from 'react-router-dom'
import App from './App'
import GamificationDashboard from './pages/GamificationDashboard'
import OnboardingWizard from './pages/OnboardingWizard'
import GenUIPlayground from './pages/GenUIPlayground'
import ImmersiveLanding from './pages/ImmersiveLanding'
import ProjectRoadmap from './pages/ProjectRoadmap'
import NodeGraphEditor from './pages/NodeGraphEditor'
import VideoStorytelling from './pages/VideoStorytelling'
import SemanticKnowledgeHub from './pages/SemanticKnowledgeHub'
import './index.css'

ReactDOM.createRoot(document.getElementById('root')!).render(
  <React.StrictMode>
    <BrowserRouter basename="/DevTeam6">
      <Routes>
        <Route path="/" element={<App />} />
        <Route path="/gamification" element={<GamificationDashboard />} />
        <Route path="/onboarding" element={<OnboardingWizard />} />
        <Route path="/genui" element={<GenUIPlayground />} />
        <Route path="/landing" element={<ImmersiveLanding />} />
        <Route path="/roadmap" element={<ProjectRoadmap />} />
        <Route path="/node-graph" element={<NodeGraphEditor />} />
        <Route path="/videos" element={<VideoStorytelling />} />
        <Route path="/knowledge-hub" element={<SemanticKnowledgeHub />} />
      </Routes>
    </BrowserRouter>
  </React.StrictMode>,
)
