import { ReactNode } from 'react'

interface ButtonProps {
  onClick?: () => void
  children: ReactNode
}

export default function Button({ onClick, children }: ButtonProps) {
  return (
    <button onClick={onClick}>
      {children}
    </button>
  )
}
