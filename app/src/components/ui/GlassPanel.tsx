import { ReactNode } from 'react';
import { motion } from 'framer-motion';

interface Props {
  children: ReactNode;
  className?: string;
  hover?: boolean;
}

export function GlassPanel({ children, className = '', hover = false }: Props) {
  return (
    <motion.div
      className={`bg-white/5 backdrop-blur-md border border-white/10 rounded-xl p-4 ${className}`}
      whileHover={hover ? { scale: 1.02, y: -2 } : undefined}
      initial={{ opacity: 0, y: 20 }}
      animate={{ opacity: 1, y: 0 }}
    >
      {children}
    </motion.div>
  );
}

export default GlassPanel;
