#!/usr/bin/env node
// index.mjs  (ES-module, no build needed)
import { Server } from '@modelcontextprotocol/sdk/server/index.js';
import { StdioServerTransport } from '@modelcontextprotocol/sdk/server/stdio.js';
import { CallToolRequestSchema, ListToolsRequestSchema } from '@modelcontextprotocol/sdk/types.js';
import puppeteer from 'puppeteer';
import sharp from 'sharp';
import { readFileSync } from 'fs';

const server = new Server(
  { name: 'npm-mcp', version: '1.0.0' },
  { capabilities: { tools: {} } }
);

server.setRequestHandler(ListToolsRequestSchema, async () => ({
  tools: [
    {
      name: 'screenshot',
      description: 'Take a PNG screenshot of a web page',
      inputSchema: {
        type: 'object',
        properties: { url: { type: 'string' } },
        required: ['url']
      }
    },
    {
      name: 'resize_image',
      description: 'Resize local image (keep aspect ratio)',
      inputSchema: {
        type: 'object',
        properties: {
          inputPath: { type: 'string' },
          outputPath: { type: 'string' },
          width: { type: 'integer', minimum: 1 }
        },
        required: ['inputPath', 'outputPath', 'width']
      }
    }
  ]
}));

server.setRequestHandler(CallToolRequestSchema, async (req) => {
  const { name, arguments: args } = req.params;
  if (name === 'screenshot') {
    const browser = await puppeteer.launch({ headless: true });
    const page = await browser.newPage();
    await page.goto(args.url, { waitUntil: 'networkidle2' });
    const png = await page.screenshot({ encoding: 'base64', fullPage: true });
    await browser.close();
    return { content: [{ type: 'image', data: png, mimeType: 'image/png' }] };
  }
  if (name === 'resize_image') {
    await sharp(args.inputPath)
      .resize({ width: args.width, withoutEnlargement: true })
      .toFile(args.outputPath);
    const out = readFileSync(args.outputPath, { encoding: 'base64' });
    return { content: [{ type: 'image', data: out, mimeType: 'image/png' }] };
  }
  throw new Error('Unknown tool');
});

const transport = new StdioServerTransport();
await server.connect(transport);