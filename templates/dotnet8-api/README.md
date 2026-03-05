# .NET 8 Web API Starter

A complete ASP.NET Core 8 Web API template with Swagger/OpenAPI documentation.

## Getting Started

```bash
dotnet restore
dotnet run
```

The API will be available at `http://localhost:5000` with Swagger UI at `http://localhost:5000/swagger`.

## Features

- 🚀 .NET 8 Web API
- 📝 Swagger/OpenAPI documentation
- 🔒 CORS enabled for development
- 🎯 Example WeatherForecast controller
- 📦 Minimal API setup with controllers

## Available Commands

- `dotnet restore` - Restore dependencies
- `dotnet run` - Run the application
- `dotnet build` - Build the project
- `dotnet test` - Run tests (add test project)
- `dotnet watch run` - Run with hot reload

## Project Structure

```
Controllers/          # API controllers
Models/              # Data models
Properties/          # Launch settings
Program.cs           # Application entry point
appsettings.json     # Configuration
```

## API Endpoints

- `GET /api/weatherforecast` - Get weather forecast data

## Development

The API includes:
- Automatic Swagger documentation
- CORS policy for cross-origin requests
- Logging configuration
- Development and production settings

Visit `/swagger` when running to explore the API interactively.
