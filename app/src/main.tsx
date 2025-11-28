import React from 'react'
import ReactDOM from 'react-dom/client'
import { BrowserRouter, Routes, Route } from 'react-router-dom'
import App from './App'
import GamificationDashboard from './pages/GamificationDashboard'
import OnboardingWizard from './pages/OnboardingWizard'
import './index.css'

ReactDOM.createRoot(document.getElementById('root')!).render(
  <React.StrictMode>
    <BrowserRouter basename="/DevTeam6">
      <Routes>
        <Route path="/" element={<App />} />
        <Route path="/gamification" element={<GamificationDashboard />} />
        <Route path="/onboarding" element={<OnboardingWizard />} />
      </Routes>
    </BrowserRouter>
  </React.StrictMode>,
)
