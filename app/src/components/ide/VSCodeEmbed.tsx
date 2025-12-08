import { useState } from 'react';

interface Props {
  path?: string;
  height?: string;
}

export function VSCodeEmbed({ path = '', height = '100%' }: Props) {
  const [isLoading, setIsLoading] = useState(true);

  // Construct VS Code URL
  const baseUrl = 'https://vscode.dev/github/SpiralCloudOmega/DevTeam6';
  const fullUrl = path ? `${baseUrl}/${path}` : baseUrl;

  return (
    <div className="relative w-full" style={{ height }}>
      {isLoading && (
        <div className="absolute inset-0 flex items-center justify-center bg-gray-950 z-10">
          <div className="text-center">
            <div className="inline-block w-12 h-12 border-4 border-indigo-500 border-t-transparent rounded-full animate-spin mb-4"></div>
            <p className="text-gray-400">Loading VS Code...</p>
            <p className="text-gray-500 text-sm mt-2">Opening DevTeam6 repository</p>
          </div>
        </div>
      )}

      <iframe
        src={fullUrl}
        className="w-full h-full border-0"
        title="VS Code Web"
        allow="cross-origin-isolated"
        onLoad={() => setIsLoading(false)}
        sandbox="allow-scripts allow-same-origin allow-forms allow-modals allow-popups"
      />
    </div>
  );
}

export default VSCodeEmbed;
