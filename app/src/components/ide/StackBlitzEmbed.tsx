import { useEffect, useRef, useState } from 'react';
import sdk from '@stackblitz/sdk';

interface Props {
  template: 'react' | 'vue' | 'dotnet';
  height?: string;
}

// Template configurations for StackBlitz
const templateConfig = {
  react: {
    repo: 'SpiralCloudOmega/DevTeam6',
    path: 'templates/react-starter',
    openFile: 'src/App.tsx',
    title: 'React Starter - DevTeam6',
  },
  vue: {
    repo: 'SpiralCloudOmega/DevTeam6',
    path: 'templates/vue3-starter',
    openFile: 'src/App.vue',
    title: 'Vue 3 Starter - DevTeam6',
  },
  dotnet: {
    repo: 'SpiralCloudOmega/DevTeam6',
    path: 'templates/dotnet8-api',
    openFile: 'Program.cs',
    title: '.NET 8 API - DevTeam6',
  },
};

export function StackBlitzEmbed({ template, height = '100%' }: Props) {
  const containerRef = useRef<HTMLDivElement>(null);
  const [isLoading, setIsLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    const embedStackBlitz = async () => {
      if (!containerRef.current) return;

      try {
        setIsLoading(true);
        setError(null);

        const config = templateConfig[template];

        // Embed GitHub repository in StackBlitz
        await sdk.embedGithubProject(containerRef.current, `${config.repo}/tree/main/${config.path}`, {
          openFile: config.openFile,
          theme: 'dark',
          view: 'default', // Shows both editor and preview
          showSidebar: true,
          terminalHeight: 50,
          devToolsHeight: 33,
          hideNavigation: false,
          forceEmbedLayout: true,
          clickToLoad: false,
        });

        setIsLoading(false);
      } catch (err) {
        console.error('StackBlitz embed error:', err);
        setError('Failed to load StackBlitz. Please try again.');
        setIsLoading(false);
      }
    };

    embedStackBlitz();
  }, [template]);

  return (
    <div className="relative w-full" style={{ height }}>
      {isLoading && (
        <div className="absolute inset-0 flex items-center justify-center bg-gray-950">
          <div className="text-center">
            <div className="inline-block w-12 h-12 border-4 border-indigo-500 border-t-transparent rounded-full animate-spin mb-4"></div>
            <p className="text-gray-400">Loading StackBlitz...</p>
          </div>
        </div>
      )}

      {error && (
        <div className="absolute inset-0 flex items-center justify-center bg-gray-950">
          <div className="text-center px-8">
            <p className="text-red-400 mb-4">{error}</p>
            <button
              onClick={() => window.location.reload()}
              className="px-4 py-2 bg-indigo-500 text-white rounded-lg hover:bg-indigo-600 transition-colors"
            >
              Retry
            </button>
          </div>
        </div>
      )}

      <div ref={containerRef} className="w-full h-full"></div>
    </div>
  );
}

export default StackBlitzEmbed;
