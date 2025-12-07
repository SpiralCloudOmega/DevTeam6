import { useState } from 'react';
import { motion } from 'framer-motion';

type Theme = 'dark' | 'light' | 'midnight';

const themes = {
  dark: { icon: '🌙', label: 'Dark', bg: '#0f0f23' },
  light: { icon: '☀️', label: 'Light', bg: '#f8fafc' },
  midnight: { icon: '🌌', label: 'Midnight', bg: '#020617' },
};

export function ThemeSwitcher() {
  const [currentTheme, setCurrentTheme] = useState<Theme>('dark');

  const handleThemeChange = (theme: Theme) => {
    setCurrentTheme(theme);
    document.documentElement.style.setProperty('--color-bg', themes[theme].bg);
  };

  return (
    <div className="flex gap-1 p-1 bg-white/10 rounded-lg">
      {Object.entries(themes).map(([key, { icon, label }]) => (
        <motion.button
          key={key}
          onClick={() => handleThemeChange(key as Theme)}
          className={`px-4 py-2 rounded-md text-lg transition-colors ${
            currentTheme === key ? 'bg-white/20' : 'hover:bg-white/5'
          }`}
          whileHover={{ scale: 1.1 }}
          whileTap={{ scale: 0.95 }}
          title={label}
        >
          {icon}
        </motion.button>
      ))}
    </div>
  );
}

export default ThemeSwitcher;
